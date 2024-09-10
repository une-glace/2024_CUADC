import numpy as np

import rospy 
# the following line depends upon the 
# type of message you are trying to publish 
from std_msgs.msg import String
from geometry_msgs.msg import PoseStamped
  
def publisher(): 
      
    # define the actions the publisher will make 
    pub = rospy.Publisher('target_pos', PoseStamped, queue_size=10)

    # initialize the publishing node 
    rospy.init_node('target_pos_pub', anonymous=True) 
      
    # define how many times per second 
    # will the data be published 
    # let's say 10 times/second or 10Hz 
    rate = rospy.Rate(10) 
    # to keep publishing as long as the core is running 
    while not rospy.is_shutdown(): 
        data =PoseStamped()
        data.pose.position.x = np.float64(1.0)
        # you could simultaneously display the data 
        # on the terminal and to the log file 
        rospy.loginfo(data) 
          
        # publish the data to the topic using publish() 
        pub.publish(data) 
          
        # keep a buffer based on the rate defined earlier 
        rate.sleep() 
  
  
if __name__ == '__main__': 
    # it is good practice to maintain 
    # a 'try'-'except' clause 
    try: 
        publisher() 
    except rospy.ROSInterruptException: 
        pass
    







# import os, cv2, time, math
# import numpy as np
# from ultralytics import YOLO
# import rospy

# SOURCEDIR = r"C:\Users\11\Desktop\\"
# MODELNAMEOBB = "best.pt"
# # MODELNAMEDETECT = "best2.pt"
# # VIDEOPATH = "raw.mp4"
# # OUTPUTDIR = "outputs"
# CAMERAINDEX = 0

# streamInput = True
# #geyiheng added 
# local_pos = None
# class f:
#     def __init__(self):
#         self.img=None
#         self.pos=None
#         self.result=[]
# flist=[]
# def loc_pos_callback(msg):
#     global local_pos
#     local_pos = msg.pose
# rospy.init_node('vision_process', anonymous = True)
# rospy.Subscriber('/mavros/local_position/pose', PoseStamped, loc_pos_callback)
# rate = rospy.Rate(30)
# #end geyiheng added
# def cropTarget(rawImage, cropTensor, width, height):
#     # 将Tensor转换为列表(该列表内有四个元素，每一个元素是一个坐标)
#     cropTensorList = cropTensor.tolist()

#     # 检查列表长度是否为4，如果不是，则可能存在问题
#     if len(cropTensorList) != 4:
#         raise ValueError("cropTensor must contain exactly 4 elements")

#     # 根据条件选择不同的点集合
#     if (cropTensorList[0][0] - cropTensorList[1][0]) ** 2 + (cropTensorList[0][1] - cropTensorList[1][1]) ** 2 > (cropTensorList[1][0] - cropTensorList[2][0]) ** 2 + (cropTensorList[1][1] - cropTensorList[2][1]) ** 2:
#         rectPoints = np.array([cropTensorList[0], cropTensorList[1], cropTensorList[2], cropTensorList[3]], dtype=np.float32)
#     else:
#         rectPoints = np.array([cropTensorList[3], cropTensorList[0], cropTensorList[1], cropTensorList[2]], dtype=np.float32)

#     dstPoints = np.array([[0, 0], [0, height], [width, height], [width, 0]], dtype=np.float32)

#     affineMatrix = cv2.getAffineTransform(rectPoints[:3], dstPoints[:3])

#     return cv2.warpAffine(rawImage, affineMatrix, (width, height))

# def enhanceTarget(rawImage, limit):
#     imgr = rawImage[:,:,0]
#     imgg = rawImage[:,:,1]
#     imgb = rawImage[:,:,2]

#     # cv2.imshow("Red" + str(i), imgr)
#     # cv2.imshow("Green" + str(i), imgg)
#     # cv2.imshow("Blue" + str(i), imgb)

#     claher = cv2.createCLAHE(clipLimit=limit, tileGridSize=(9,9))
#     claheg = cv2.createCLAHE(clipLimit=limit, tileGridSize=(9,9))
#     claheb = cv2.createCLAHE(clipLimit=limit, tileGridSize=(9,9))

#     cllr = claher.apply(imgr)
#     cllg = claheg.apply(imgg)
#     cllb = claheb.apply(imgb)

#     return np.dstack((cllr, cllg, cllb))


# def getSquare(rawImage, squareError, varianceThreshold, A):
#     gray = cv2.cvtColor(rawImage, cv2.COLOR_BGR2GRAY)
#     blurred = cv2.GaussianBlur(gray, (5, 5), 0)
#     edged = cv2.Canny(blurred, 10, 190)

#     cv2.imshow("Canny Frame", edged)

#     contours, _ = cv2.findContours(edged.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

#     max_area = -1
#     best_rect = None
#     best_warped = None

#     for contour in contours:
#         epsilon = 0.1 * cv2.arcLength(contour, True)
#         approx = cv2.approxPolyDP(contour, epsilon, True)

#         if len(approx) == 4:
#             rect = cv2.minAreaRect(approx)
#             (x, y), (width, height), angle = rect

#             if min(width, height) == 0:
#                 continue

#             aspect_ratio = max(width, height) / min(width, height)
#             if abs(aspect_ratio - 1) <= squareError:
#                 # 获取矩形的四个顶点
#                 box = cv2.boxPoints(rect)
#                 box = np.int0(box)

#                 # 计算中心点
#                 center = np.mean(box, axis=0)

#                 # 寻找最左下角的点
#                 leftmost_index = np.argmax(box[:, 1] - center[1] + (box[:, 0] - center[0]) * 1e-6)

#                 # 调整顶点顺序，使其从最左下角的点开始按顺时针排序
#                 sorted_box = np.roll(box, -leftmost_index, axis=0)

#                 # 设置目标点
#                 width = int(max(width, height))
#                 src_pts = sorted_box.astype("float32")
#                 dst_pts = np.array([[0, width-1],
#                                     [0, 0],
#                                     [width-1, 0],
#                                     [width-1, width-1]], dtype="float32")

#                 # 计算透视变换矩阵
#                 M = cv2.getPerspectiveTransform(src_pts, dst_pts)

#                 # 进行透视变换
#                 warped = cv2.warpPerspective(gray, M, (width, width))
                                
#                 variance = np.var(warped)
#                 if variance > varianceThreshold and width * height > max_area:
#                     max_area = width * height
#                     best_rect = rect
#                     best_warped = warped

#     if best_rect:
#         final_image = cv2.resize(best_warped, (A, A))
#         return final_image

#     return None

# # 此段代码貌似没有用（从文件读取图像）

# '''
# if streamInput == False:
#     if os.path.exists(os.path.join(SOURCEDIR, OUTPUTDIR)) :
#         count = 1
#         while os.path.exists(os.path.join(SOURCEDIR, OUTPUTDIR) + str(count)):
#             count += 1
#         os.makedirs(os.path.join(SOURCEDIR, OUTPUTDIR) + str(count))
#         OUTPUTDIR = OUTPUTDIR + str(count)
#     else:
#         os.makedirs(os.path.join(SOURCEDIR, OUTPUTDIR))
#     currentDir = os.path.join(SOURCEDIR, OUTPUTDIR)
# '''

# modelObb = YOLO(os.path.join(SOURCEDIR, MODELNAMEOBB))  # 通常是pt模型的文件

# class frameData:
#     def __init__(self):
#         self.img
#         self.pos
#         self.result
# if streamInput:
#     print("Camera: ", CAMERAINDEX, " is being used...")
    
#     cap = cv2.VideoCapture(CAMERAINDEX)
#     cap.set(cv2.CAP_PROP_FPS, 330)
#     frameWidth = cap.get(cv2.CAP_PROP_FRAME_WIDTH)
#     frameHeight = cap.get(cv2.CAP_PROP_FRAME_HEIGHT)
#     frameFps = cap.get(cv2.CAP_PROP_FPS)  # 帧率
                         
#     print(frameWidth,frameHeight,frameFps)

#     while cap.isOpened() :
#         success, currentFrame = cap.read()
#         fd1=frameData()
#         fd1.img=frame
#         fd1.pos=local_pos
#         if success == False:
#             continue
#         else:
#             results = modelObb.predict(
#                 source = currentFrame,
#                 #imgsz = 640,
#                 half = True,
#                 iou = 0.2,
#                 conf = 0.53,
#                 device = '0', #'0'使用GPU运行
#                 max_det = 2,
#                 # augment = True
#             )
#             result = results[0]
#             fd1.result=results
#             annotatedFrame = result.plot()  # 获取框出的图像
#             cv2.imshow("annotated Frame", annotatedFrame)  # 绘制框出部分的图像
#             cv2.waitKey(20)

#             cropTensors = result.obb.xyxyxyxy.cpu()  # 矩形的四个坐标

#             for i, cropTensor in enumerate(cropTensors):
                
#                 framet = cropTarget(result.orig_img, cropTensor, 320, 640)
#                 # cv2.imshow("cropped Frame" + str(i), framet)

#                 # framet = enhanceTarget(framet, 3)
#                 # cv2.imshow("CLAHE enhanced" + str(i), framet)

#                 s = getSquare(framet, 1, 150, 320)
#                 if s is not None:
#                     cv2.imshow("cropped Square " + str(i), s)
        
#                 cv2.waitKey(1)
#     cap.release()

# else:
#     exit()
    
