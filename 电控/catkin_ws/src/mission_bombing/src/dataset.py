import torch
import torch.nn as nn
from torch.utils.data import DataLoader, Dataset
from torchvision import transforms as T
import matplotlib.pyplot as plt
import os
from PIL import Image
import re
import numpy as np
import random


class Number(Dataset):
    def __init__(self, root, train=True, transforms=None, test=False):
        imgs = []
        for path in os.listdir(root):
            path_name = path.split(".")[0]
            path_prefix = path_name[-2:]
            if re.match(r'^\d+$', path_prefix):  # 检查是否只包含数字
                # label = path_prefix
                label = int(path_prefix)
                # childpath = os.path.join(root, path)
                # for imgpath in os.listdir(childpath):
                imgs.append((os.path.join(root, path), label))

        if test == False:
            train_path_list, val_path_list = self._split_data_set(imgs)
            if train:
                self.imgs = train_path_list
            else:
                self.imgs = val_path_list
        else:
            self.imgs = imgs
        if transforms is None:
            normalize = T.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])

            self.transforms = T.Compose([
                T.Resize(256),
                T.CenterCrop(224),
                T.ToTensor(),
                normalize
            ])
        else:
            self.transforms = transforms

    def __getitem__(self, index):
        img_path = self.imgs[index][0]
        label = self.imgs[index][1]

        data = Image.open(img_path)
        if data.mode != "RGB":
            data = data.convert("RGB")
        data = self.transforms(data)
        # print(label)
        # print(img_path)
        # print("------------------------------------")
        return data, label

    def __len__(self):
        return len(self.imgs)

    def _split_data_set(self, imags):
        val_path_list = imags[::5]
        train_path_list = []
        for item in imags:
            if item not in val_path_list:
                train_path_list.append(item)
        return train_path_list, val_path_list


if __name__ == "__main__":
    root = "/home/sihcl3/datasets/Dataset/img"
    train_dataset = Number(root, train=True)
    train_dataloader = DataLoader(train_dataset, batch_size=32, shuffle=True)
    for date, label in train_dataloader:
        print("1")

