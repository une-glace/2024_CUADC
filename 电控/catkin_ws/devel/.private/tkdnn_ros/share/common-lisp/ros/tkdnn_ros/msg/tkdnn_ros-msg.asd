
(cl:in-package :asdf)

(defsystem "tkdnn_ros-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "bbox" :depends-on ("_package_bbox"))
    (:file "_package_bbox" :depends-on ("_package"))
    (:file "bboxes" :depends-on ("_package_bboxes"))
    (:file "_package_bboxes" :depends-on ("_package"))
  ))