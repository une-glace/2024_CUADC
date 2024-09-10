; Auto-generated. Do not edit!


(cl:in-package tkdnn_ros-msg)


;//! \htmlinclude bboxes.msg.html

(cl:defclass <bboxes> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (bboxes
    :reader bboxes
    :initarg :bboxes
    :type (cl:vector tkdnn_ros-msg:bbox)
   :initform (cl:make-array 0 :element-type 'tkdnn_ros-msg:bbox :initial-element (cl:make-instance 'tkdnn_ros-msg:bbox))))
)

(cl:defclass bboxes (<bboxes>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <bboxes>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'bboxes)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name tkdnn_ros-msg:<bboxes> is deprecated: use tkdnn_ros-msg:bboxes instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <bboxes>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader tkdnn_ros-msg:header-val is deprecated.  Use tkdnn_ros-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'bboxes-val :lambda-list '(m))
(cl:defmethod bboxes-val ((m <bboxes>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader tkdnn_ros-msg:bboxes-val is deprecated.  Use tkdnn_ros-msg:bboxes instead.")
  (bboxes m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <bboxes>) ostream)
  "Serializes a message object of type '<bboxes>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'bboxes))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'bboxes))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <bboxes>) istream)
  "Deserializes a message object of type '<bboxes>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'bboxes) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'bboxes)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'tkdnn_ros-msg:bbox))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<bboxes>)))
  "Returns string type for a message object of type '<bboxes>"
  "tkdnn_ros/bboxes")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'bboxes)))
  "Returns string type for a message object of type 'bboxes"
  "tkdnn_ros/bboxes")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<bboxes>)))
  "Returns md5sum for a message object of type '<bboxes>"
  "6ce0fdf9895bce90aca959558c92d32d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'bboxes)))
  "Returns md5sum for a message object of type 'bboxes"
  "6ce0fdf9895bce90aca959558c92d32d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<bboxes>)))
  "Returns full string definition for message of type '<bboxes>"
  (cl:format cl:nil "std_msgs/Header header~%bbox[] bboxes~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: tkdnn_ros/bbox~%float64 score~%int64 x~%int64 y~%int64 width~%int64 height~%int16 id~%string Class~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'bboxes)))
  "Returns full string definition for message of type 'bboxes"
  (cl:format cl:nil "std_msgs/Header header~%bbox[] bboxes~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: tkdnn_ros/bbox~%float64 score~%int64 x~%int64 y~%int64 width~%int64 height~%int16 id~%string Class~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <bboxes>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'bboxes) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <bboxes>))
  "Converts a ROS message object to a list"
  (cl:list 'bboxes
    (cl:cons ':header (header msg))
    (cl:cons ':bboxes (bboxes msg))
))
