import torch
from torch import nn

num_classes = 100
class Bottleneck(nn.Module):
    extention = 4

    def __init__(self, inplanes, planes, stride, downsample=None):
        super(Bottleneck, self).__init__()
        self.conv1 = nn.Conv2d(inplanes, planes, kernel_size=1, stride=stride, bias=False)
        self.bn1 = nn.BatchNorm2d(planes)

        self.conv2 = nn.Conv2d(planes, planes, kernel_size=3, stride=1, padding=1, bias=False)
        self.bn2 = nn.BatchNorm2d(planes)

        self.conv3 = nn.Conv2d(planes, planes * self.extention, kernel_size=1, stride=1, bias=False)
        self.bn3 = nn.BatchNorm2d(planes * self.extention)

        self.relu = nn.ReLU(inplace=True)

        self.downsample = downsample
        self.stride = stride

    def forward(self, x):
        shortcut = x

        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)

        out = self.conv2(out)
        out = self.bn2(out)
        out = self.relu(out)

        out = self.conv3(out)
        out = self.bn3(out)
        out = self.relu(out)

        if self.downsample is not None:
            shortcut = self.downsample(x)

        out = out + shortcut
        out = self.relu(out)
        return out


class ResNet50(nn.Module):
    def __init__(self, block, layers, num_class):
        self.inplane = 64
        super(ResNet50, self).__init__()

        self.block = block
        self.layers = layers

        self.conv1 = nn.Conv2d(3, self.inplane, kernel_size=7, stride=2, padding=3, bias=False)
        self.bn1 = nn.BatchNorm2d(self.inplane)
        self.relu = nn.ReLU()
        self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)

        self.stage1 = self.make_layer(self.block, 64, layers[0], stride=1)
        self.stage2 = self.make_layer(self.block, 128, layers[1], stride=2)
        self.stage3 = self.make_layer(self.block, 256, layers[2], stride=2)
        self.stage4 = self.make_layer(self.block, 512, layers[3], stride=2)

        self.avgpool = nn.AvgPool2d(7)
        self.fc = nn.Linear(512 * block.extention, num_class)

    def forward(self, x):
        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)
        out = self.maxpool(out)

        out = self.stage1(out)
        out = self.stage2(out)
        out = self.stage3(out)
        out = self.stage4(out)

        out = self.avgpool(out)
        out = torch.flatten(out, 1)
        out = self.fc(out)

        return out

    def make_layer(self, block, plane, block_num, stride=1):
        block_list = []
        downsample = None
        if (stride != 1 or self.inplane != plane * block.extention):
            downsample = nn.Sequential(
                nn.Conv2d(self.inplane, plane * block.extention, stride=stride, kernel_size=1, bias=False),
                nn.BatchNorm2d(plane * block.extention)
            )
        conv_block = block(self.inplane, plane, stride=stride, downsample=downsample)
        block_list.append(conv_block)
        self.inplane = plane * block.extention

        for i in range(1, block_num):
            block_list.append(block(self.inplane, plane, stride=1))

        return nn.Sequential(*block_list)


if __name__ == "__main__":
    resnet = ResNet50(Bottleneck, [3, 4, 6, 3], num_classes)
    x = torch.randn(64, 3, 224, 224)
    x = resnet(x)
    print(x.shape)
