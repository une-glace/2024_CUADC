;; Auto-generated. Do not edit!


(when (boundp 'tkdnn_ros::bboxes)
  (if (not (find-package "TKDNN_ROS"))
    (make-package "TKDNN_ROS"))
  (shadow 'bboxes (find-package "TKDNN_ROS")))
(unless (find-package "TKDNN_ROS::BBOXES")
  (make-package "TKDNN_ROS::BBOXES"))

(in-package "ROS")
;;//! \htmlinclude bboxes.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass tkdnn_ros::bboxes
  :super ros::object
  :slots (_header _bboxes ))

(defmethod tkdnn_ros::bboxes
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:bboxes __bboxes) (let (r) (dotimes (i 0) (push (instance tkdnn_ros::bbox :init) r)) r))
    )
   (send-super :init)
   (setq _header __header)
   (setq _bboxes __bboxes)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:bboxes
   (&rest __bboxes)
   (if (keywordp (car __bboxes))
       (send* _bboxes __bboxes)
     (progn
       (if __bboxes (setq _bboxes (car __bboxes)))
       _bboxes)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; tkdnn_ros/bbox[] _bboxes
    (apply #'+ (send-all _bboxes :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; tkdnn_ros/bbox[] _bboxes
     (write-long (length _bboxes) s)
     (dolist (elem _bboxes)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; tkdnn_ros/bbox[] _bboxes
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _bboxes (let (r) (dotimes (i n) (push (instance tkdnn_ros::bbox :init) r)) r))
     (dolist (elem- _bboxes)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get tkdnn_ros::bboxes :md5sum-) "6ce0fdf9895bce90aca959558c92d32d")
(setf (get tkdnn_ros::bboxes :datatype-) "tkdnn_ros/bboxes")
(setf (get tkdnn_ros::bboxes :definition-)
      "std_msgs/Header header
bbox[] bboxes

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

================================================================================
MSG: tkdnn_ros/bbox
float64 score
int64 x
int64 y
int64 width
int64 height
int16 id
string Class

")



(provide :tkdnn_ros/bboxes "6ce0fdf9895bce90aca959558c92d32d")


