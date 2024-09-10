# -*- coding:UTF-8 -*-
import torch
import cv2
import rospy
from geometry_msgs.msg import PoseStamped
import numpy as np
from torchvision import transforms
from resnet import ResNet50, Bottleneck
from PIL import Image

local_pos = None
class f:
    def __init__(self):
        self.img=None
        self.pos=None
        self.result=[]
flist=[]
def loc_pos_callback(msg):
    global local_pos
    local_pos = msg.pose

def process_frame(frame, model, device, transform , threshold=0.990):
    # 将OpenCV图像转换为PIL图像
    image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    image = Image.fromarray(image)

    # 应用预处理
    image = transform(image).unsqueeze(0).to(device)

    # 进行预测
    with torch.no_grad():
        output = model(image)
        _, pred = torch.max(output, 1)
        pred_confidence = torch.softmax(output, dim=1).max().item()
        print(pred_confidence)

    if pred_confidence < threshold:
        return -1
    else:
        return pred.item()


def main():
    print("enter main")
    num_classes = 100
    model = ResNet50(Bottleneck, [3, 4, 6, 3], num_classes)
    model.load_state_dict(torch.load("/home/amov/桌面/Resnet/weights/best_add_virtual.pt", map_location=torch.device('cpu')))
    model.eval()

    # 设备配置
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = model.to(device)

    # 预处理配置
    transform = transforms.Compose([
        transforms.Resize(256),
        transforms.CenterCrop(224),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
    ])
    print("transform success")
    # activate ros node
    rospy.init_node('vision_process', anonymous = True)
    rospy.Subscriber('/mavros/local_position/pose', PoseStamped, loc_pos_callback)
    rate = rospy.Rate(30)

    # 打开摄像头
    # for i in range (10):
    #     try:
    #         print(i)
    #         cap = cv2.VideoCapture(i)  # 通常0是默认摄像头
    #     except:
    #         continue
    cap = cv2.VideoCapture(6)
    if not cap.isOpened():
        print("无法打开摄像头")
        exit()
    print("camera opened")
    cap.set(cv2.CAP_PROP_FPS , 330)#此处可能要修改
    print("set 330 success")
    while not rospy.is_shutdown():
        print("reading cap...")
        ret, frame = cap.read()
        print("read cap success")
        # cv2.imshow('Video Stream with Predictions', frame)
        print("read frame success ")
        print(local_pos)
        f1=f()
        f1.img=frame
        f1.pos=local_pos
        if not ret:
            print("无法接收帧（流可能已结束）")
            break

            # 预测帧的类别
        predicted_class = process_frame(f1.img, model, device, transform)

        # 在帧上添加预测结果文本
        cv2.putText(frame, f'Predicted: {predicted_class}', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        # f1.pre
        # 显示帧
        # cv2.imshow('Video Stream with Predictions', frame)

        # 如果按下'q'键，退出循环
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

        # match the frame rate of the camera
        rate.sleep()


    # 释放摄像头并关闭窗口
    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
