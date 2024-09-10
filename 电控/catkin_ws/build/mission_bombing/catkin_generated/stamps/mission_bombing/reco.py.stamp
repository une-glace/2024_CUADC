import os, cv2, time, math
import numpy as np
from ultralytics import YOLO
import torch
from torchvision import transforms
from resnet import ResNet50, Bottleneck
from PIL import Image
# import pytesseract
import re  # 导入Python的正则表达式库re
import rospy
from geometry_msgs.msg import PoseStamped

delta = 0

SOURCEDIR = r"/home/amov/桌面"  #yolo模型文件
MODELNAMEOBB = "best_yolo.pt"
# MODELNAMEDETECT = "best2.pt"
# VIDEOPATH = "raw.mp4"
# OUTPUTDIR = "outputs"
CAMERAINDEX = 0

streamInput = True

posx = 0.0
posy = 0.0

def auto_rotate(image):
    # 将图像转换为HSV色彩空间
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    # 定义红色在HSV色彩空间的范围
    lower_red1 = np.array([0, 120, 70])
    upper_red1 = np.array([10, 255, 255])
    lower_red2 = np.array([170, 120, 70])
    upper_red2 = np.array([180, 255, 255])

    # 创建红色五边形的掩膜
    mask1 = cv2.inRange(hsv, lower_red1, upper_red1)
    mask2 = cv2.inRange(hsv, lower_red2, upper_red2)
    mask = cv2.bitwise_or(mask1, mask2)

    # 进行形态学操作以去除噪声
    kernel = np.ones((3, 3), np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)  # 进行开运算（先腐蚀后膨胀）

    # 寻找轮廓
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # 遍历轮廓，寻找五边形
    for cnt in contours:
        # 计算轮廓的近似多边形
        epsilon = 0.02 * cv2.arcLength(cnt, True)  # 指定的精度，也即是原始曲线与近似曲线之间的最大距离。
        approx = cv2.approxPolyDP(cnt, epsilon, True)


        # 检查多边形的顶点数是否为5
        if len(approx) == 5 and cv2.contourArea(cnt) > 30000:
            # 绘制五边形
            # cv2.drawContours(image, [approx], 0, (0, 255, 0), 3)

            # 计算五边形的边界框
            x, y, w, h = cv2.boundingRect(approx)
            crop_roi = image[y:y + h, x:x + w]  # 裁切框出的矩形部分，此处10是留一些余量，需要调试
#            cv2.imshow("crop draw five", crop_roi)

            if crop_roi is not None and crop_roi.size != 0:
                hsv_crop_roi = cv2.cvtColor(crop_roi, cv2.COLOR_BGR2HSV)

                # 定义白色在HSV色彩空间的范围
                lower_white = np.array([0, 0, 10])
                upper_white = np.array([180, 150, 255])

                # 创建白色的掩膜
                white_mask = cv2.inRange(hsv_crop_roi, lower_white, upper_white)

                # 进行形态学操作以去除噪声
                kernel = np.ones((5, 5), np.uint8)
                white_mask = cv2.morphologyEx(white_mask, cv2.MORPH_OPEN, kernel)  # 进行开运算（先腐蚀后膨胀）

                # 寻找白色区域的轮廓
                white_contours, _ = cv2.findContours(white_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

                # 在原图的五边形区域上绘制白色轮廓
                for white_cnt in white_contours:
                    # 计算轮廓的边界框
                    x1, y1, w1, h1 = cv2.boundingRect(white_cnt)

                    # 检查边界框是否在五边形区域内
                    if x1 + w1 <= image.shape[1] and y1 + h1 <= image.shape[0]:
                        # 检查是否是近似的矩形或方形
                        peri = cv2.arcLength(white_cnt, True)
                        approx_cur = cv2.approxPolyDP(white_cnt, 0.05 * peri, True)

                        # print(len(approx_cur)) 此句用来调试
                        # 如果轮廓点数为4，即认为是矩形或方形
                        if len(approx_cur) == 4 and cv2.contourArea(white_cnt) > 10000:
                            # 在原图上绘制轮廓框
                            # print("The area is" + str(cv2.contourArea(white_cnt)))  # 此句用来调试
                            # cv2.drawContours(crop_roi, [white_cnt], 0, (255, 0, 0), 2)  # 使用红色线条

                            M_white = cv2.moments(white_cnt)

                            #计算重心
                            white_height, white_width, white_channels = crop_roi.shape

                            cx = int(M_white['m10'] / M_white['m00'])
                            cy = int(M_white['m01'] / M_white['m00'])

                            cx_1 = cx / white_width
                            cy_1 = cy / white_height
                            print(cx, cy, cx_1, cy_1)
                            # 在图像上绘制重心
                            # cv2.circle(crop_roi, (cx, cy), 5, (0, 0, 255), -1)

                            if cy_1 < 0.5:
                                image = cv2.rotate(crop_roi, cv2.ROTATE_180)
                                return image
                            return crop_roi # image




def process_frame(frame, model, device, transform, threshold=0.990):
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


def cropTarget(rawImage, cropTensor, width, height):
    # 将Tensor转换为列表(该列表内有四个元素，每一个元素是一个坐标)
    cropTensorList = cropTensor.tolist()

    # 检查列表长度是否为4，如果不是，则可能存在问题
    if len(cropTensorList) != 4:
        raise ValueError("cropTensor must contain exactly 4 elements")

    # 根据条件选择不同的点集合
    if (cropTensorList[0][0] - cropTensorList[1][0]) ** 2 + (cropTensorList[0][1] - cropTensorList[1][1]) ** 2 > (
            cropTensorList[1][0] - cropTensorList[2][0]) ** 2 + (cropTensorList[1][1] - cropTensorList[2][1]) ** 2:
        rectPoints = np.array([cropTensorList[0], cropTensorList[1], cropTensorList[2], cropTensorList[3]],
                              dtype=np.float32)
    else:
        rectPoints = np.array([cropTensorList[3], cropTensorList[0], cropTensorList[1], cropTensorList[2]],
                              dtype=np.float32)

    global posx
    global posy
    posx = (cropTensorList[0][0] + cropTensorList[1][0] + cropTensorList[2][0] + cropTensorList[3][0]) / 4
    posy = (cropTensorList[0][1] + cropTensorList[1][1] + cropTensorList[2][1] + cropTensorList[3][1]) / 4

    dstPoints = np.array([[0, 0], [0, height], [width, height], [width, 0]], dtype=np.float32)

    affineMatrix = cv2.getAffineTransform(rectPoints[:3], dstPoints[:3])

    return cv2.warpAffine(rawImage, affineMatrix, (width, height))


def enhanceTarget(rawImage, limit):
    imgr = rawImage[:, :, 0]
    imgg = rawImage[:, :, 1]
    imgb = rawImage[:, :, 2]

#    # cv2.imshow("Red" + str(i), imgr)
#    # cv2.imshow("Green" + str(i), imgg)
#    # cv2.imshow("Blue" + str(i), imgb)

    claher = cv2.createCLAHE(clipLimit=limit, tileGridSize=(9, 9))
    claheg = cv2.createCLAHE(clipLimit=limit, tileGridSize=(9, 9))
    claheb = cv2.createCLAHE(clipLimit=limit, tileGridSize=(9, 9))

    cllr = claher.apply(imgr)
    cllg = claheg.apply(imgg)
    cllb = claheb.apply(imgb)

    return np.dstack((cllr, cllg, cllb))


def getSquare(rawImage, squareError, varianceThreshold, A):
    threshold_value = 150
    max_value = 255
    '''height1, width1, channels = rawImage.shape
    gray1 = cv2.cvtColor(rawImage, cv2.COLOR_BGR2GRAY)
    rawImage1 = cv2.resize(gray1, (height1, width1), interpolation=cv2.INTER_CUBIC)

    # 应用阈值化
    _, binary_image = cv2.threshold(rawImage1, 150, 255, cv2.THRESH_BINARY_INV)

    x1, y1 = 80, 390  # 80, 390
    x2, y2 = 220, 520  # 220, 520
    crop_img2 = rawImage[y1: y2, x1: x2]'''

    # 使用现成的库进行机器学习
    '''text = pytesseract.image_to_string(binary_image, lang='eng', config='--psm 6')  # crop_img2
    #cv2.waitKey(20)
    numbers = re.findall(r'\d+', text)  # 提取所有数字
    my_string = ' '.join(map(str, numbers))
    #print("The number is:" + str(numbers))
    cv2.putText(rawImage, f'Predicted: {numbers}', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, fontScale= 1, color= (0, 255, 0), thickness= 2)'''
    if rawImage is not None and rawImage.size != 0:
#        # cv2.imshow("binary", binary_image)
#        # cv2.imshow("www", crop_img2)  # 展示裁剪后的图像

        kernel = np.ones((5, 5), np.uint8)  # 控制膨胀的大小

        # rawimage为放大后的彩色图像
        frame = rawImage
#        predicted_class = process_frame(frame, model, device, transform)

        # 在帧上添加预测结果文本
#        # cv2.putText(frame, f'Predicted: {predicted_class}', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        height2, width2, chaenls2 = frame.shape

        cv2.resize(frame, (height2 * 3, width2 * 3))
        # 显示帧
#        #cv2.imshow('Video Stream with Predictions', frame)

        gray = cv2.cvtColor(rawImage, cv2.COLOR_BGR2GRAY)
#        # cv2.imshow("gray", gray)
        ###############################################################################
        #blurred = cv2.GaussianBlur(gray, (7, 7), 0)
        edged = cv2.Canny(gray, 22, 50)  # 重要的参数调节

#        #cv2.imshow("Canny Frame", edged)

        imgdilate = cv2.dilate(edged,kernel,iterations=1)

#        #cv2.imshow("fushi", imgdilate)

        contours, _ = cv2.findContours(edged.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

        max_area = 1000
        best_rect = None
        best_warped = None

        for contour in contours:
            epsilon = 0.1 * cv2.arcLength(contour, True)
            approx = cv2.approxPolyDP(contour, epsilon, True)

            if len(approx) == 4:
                rect = cv2.minAreaRect(approx)
                (x, y), (width, height), angle = rect

                if min(width, height) == 0:
                    continue

                aspect_ratio = max(width, height) / min(width, height)
                if abs(aspect_ratio - 1) <= squareError:


                    # 获取矩形的四个顶点
                    box = cv2.boxPoints(rect)
                    box = np.intp(box)

                    # 计算中心点
                    center = np.mean(box, axis=0)

                    # 寻找最左下角的点
                    leftmost_index = np.argmax(box[:, 1] - center[1] + (box[:, 0] - center[0]) * 1e-6)

                    # 调整顶点顺序，使其从最左下角的点开始按顺时针排序
                    sorted_box = np.roll(box, -leftmost_index, axis=0)

                    # 设置目标点
                    width = int(max(width, height))
                    src_pts = sorted_box.astype("float32")
                    dst_pts = np.array([[0, width-1],
                                        [0, 0],
                                        [width-1, 0],
                                        [width-1, width-1]], dtype="float32")

                    # 计算透视变换矩阵
                    M = cv2.getPerspectiveTransform(src_pts, dst_pts)

                    # 进行透视变换
                    warped = cv2.warpPerspective(gray, M, (width, width))

                    variance = np.var(warped)

                    if variance > varianceThreshold and width * height > max_area:
                        print("find best one!And the area is" + str(width * height))
                        print("find best one!And the var is" + str(variance))
                        max_area = width * height
                        best_rect = rect
                        best_warped = warped

        if best_rect:
            final_image = cv2.resize(best_warped, (A, A))
            return final_image

        return None


# 此段代码貌似没有用（从文件读取图像）

'''
if streamInput == False:
    if os.path.exists(os.path.join(SOURCEDIR, OUTPUTDIR)) :
        count = 1
        while os.path.exists(os.path.join(SOURCEDIR, OUTPUTDIR) + str(count)):
            count += 1
        os.makedirs(os.path.join(SOURCEDIR, OUTPUTDIR) + str(count))
        OUTPUTDIR = OUTPUTDIR + str(count)
    else:
        os.makedirs(os.path.join(SOURCEDIR, OUTPUTDIR))
    currentDir = os.path.join(SOURCEDIR, OUTPUTDIR)
'''

modelObb = YOLO(os.path.join(SOURCEDIR, MODELNAMEOBB))  # 通常是pt模型的文件

num_classes = 100
model = ResNet50(Bottleneck, [3, 4, 6, 3], num_classes)
#model.load_state_dict(torch.load(r"/home/amov/桌面/best.pt"))  # 模型加载需要较长的时间
#model.eval()

local_pos = PoseStamped()
target_pos = PoseStamped()

def pos_cb(msg):
    global local_pos
    local_pos = msg

rospy.init_node("reco", anonymous=True)
local_pos_subscriber = rospy.Subscriber("/mavros/local_position/pose", PoseStamped, pos_cb)
target_pos_publisher = rospy.Publisher("target_pos", PoseStamped, queue_size=10)
rate = rospy.Rate(20) 
print("set rate success")
# 设备配置
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

model = model.to(device)

#坐标转换
def caculate(u, v, height):
    # 1448.401886375289 -0.510931415778886 1333.87910646133
    # 0                 1479.38139407477   741.225045761609
    # 0                 0                  1


    #0.000690416112687176 0.000000238447829170133 -0.921108370980764
    #0                    0.000675958210644806    -0.501037155618132
    #0                    0                       1

    a11 = 0.000690416112687176
    a12 = 0.000000238447829170133
    a13 = -0.921108370980764
    a21 = 0
    a22 = 0.000675958210644806
    a23 = -0.501037155618132
    a31 = 0
    a32 = 0
    a33 = 1

    k1 = -0.0735
    k2 = 0.0398
    p1 = -0.0016
    p2 = -0.0019

    Xc = (u * a11 + v * a12 + 1 * a13)  * height
    Yc = (u * a21 + v * a22 + 1 * a23)  * height
    Zc = (u * a31 + v * a32 + 1 * a33)  * height

    yaw = 30

    b11 = np.cos(np.deg2rad(yaw))
    b12 = np.sin(np.deg2rad(yaw))
    b13 = 0
    b21 = -np.sin(np.deg2rad(yaw))
    b22 = np.cos(np.deg2rad(yaw))
    b23 = 0
    b31 = 0
    b32 = 0
    b33 = -1

    Xw = Xc * b11 + Yc * b12 + Zc * b13
    Yw = Xc * b21 + Yc * b22 + Zc * b23

    global target_pos
    # target_pos = PoseStamped()
    # target_pos.pose.position.x = Xw
    # target_pos.pose.position.y = Yw
    # target_pos.pose.position.z = 20

# 预处理配置
transform = transforms.Compose([
    transforms.Resize(256),
    transforms.CenterCrop(224),
    transforms.ToTensor(),
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
])
print("transform success")
for i in range (10):
    try:
        print(i)
        cap = cv2.VideoCapture(i)  # 通常0是默认摄像头
        state,frame=cap.read()
        if frame is not None:
            print(f"camera {i} is opened" )
            print(frame)
            break
    except:
        continue

if streamInput:
    print("Camera: ", CAMERAINDEX, " is being used...")
    # cap = cv2.VideoCapture(CAMERAINDEX)

    cap.set(cv2.CAP_PROP_FPS, 330)  # 设置帧率
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)  # 设置像素宽度
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)  # 设置像素高度

    frameWidth = cap.get(cv2.CAP_PROP_FRAME_WIDTH)
    frameHeight = cap.get(cv2.CAP_PROP_FRAME_HEIGHT)
    frameFps = cap.get(cv2.CAP_PROP_FPS)  # 帧率

    print(frameWidth, frameHeight, frameFps)

    while cap.isOpened():

        rate.sleep()
        
        success, currentFrame = cap.read()

        if success == False:
            continue
        else:
            # target_pos.pose.position.x += local_pos.pose.position.x
            # target_pos.pose.position.y += local_pos.pose.position.y
            target_pos.pose.position.x = local_pos.pose.position.x
            target_pos.pose.position.y = local_pos.pose.position.y
            target_pos_publisher.publish(target_pos)
            results = modelObb.predict(
                source=currentFrame,
                # imgsz = 640,
                # half=True,
                iou=0.4,
                conf=0.6,
                device='cpu',  # '0'使用GPU运行
                max_det=1,
                # augment = True
            )
            result = results[0]

            annotatedFrame = result.plot()  # 获取框出的图像
#            cv2.imshow("annotated Frame", annotatedFrame)  # 绘制有框的大图片
            cv2.waitKey(20)

            cropTensors = result.obb.xyxyxyxy.cpu()  # 矩形的四个坐标

            for i, cropTensor in enumerate(cropTensors):
                framet = cropTarget(result.orig_img, cropTensor, 320, 640)
#                # cv2.imshow("cropped Frame" + str(i), framet)

                # framet = enhanceTarget(framet, 3)
#                # cv2.imshow("CLAHE enhanced" + str(i), framet)
                if framet is not None and framet.size != 0:
                    # 预测帧的类别
#                    predicted_class = process_frame(framet, model, device, transform)

                    # 在帧上添加预测结果文本
#                    cv2.putText(framet, f'Predicted: {predicted_class}', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.6,
                                # (0, 255, 0), 2)

                    # 显示帧
#                    cv2.imshow("no crop", framet)
                    framet = auto_rotate(framet)

                    if framet is not None and framet.size != 0:
#                        # cv2.imshow("show square", framet)  # 下面对framet进行预测


                        # 预测帧的类别
#                        predicted_class = process_frame(framet, model, device, transform)

                        # 在帧上添加预测结果文本
#                        cv2.putText(framet, f'Predicted: {predicted_class}', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.6,
                                    # (0, 255, 0), 2)

                        # 显示帧
#                        cv2.imshow('Video Stream with Predictions', framet)

                        x1, y1 = 20, 200  # 80, 390
                        x2, y2 = 250, 520  # 220, 520
                        framet = framet[y1: y2, x1: x2]

                        # 预测帧的类别
#                        predicted_class = process_frame(framet, model, device, transform)

                        # 在帧上添加预测结果文本
#                        cv2.putText(framet, f'Predicted: {predicted_class}', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.6,
                                    # (0, 255, 0), 2)

                        # 显示帧
#                        cv2.imshow('crop image', framet)


                        width_, height_, channels = framet.shape

                        framet = cv2.resize(framet, (height_ * 4, width_ * 4))

                        # 预测帧的类别
#                        '''predicted_class = process_frame(framet, model, device, transform)

                        # 在帧上添加预测结果文本
#                        cv2.putText(framet, f'Predicted: {predicted_class}', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.6,
                                    # (0, 255, 0), 2)

                        # 显示帧
#                        # cv2.imshow('crop improve image', framet)'''

                        # s = getSquare(framet, 10, 1000, 320)  # 会显示canny检测得到的图像
                # if s is not None:
#                    # cv2.imshow("cropped Square " + str(i), s)

                cv2.waitKey(1)
    cap.release()

else:
    exit()
