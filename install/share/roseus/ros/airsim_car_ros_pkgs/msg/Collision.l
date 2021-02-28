;; Auto-generated. Do not edit!


(when (boundp 'airsim_car_ros_pkgs::Collision)
  (if (not (find-package "AIRSIM_CAR_ROS_PKGS"))
    (make-package "AIRSIM_CAR_ROS_PKGS"))
  (shadow 'Collision (find-package "AIRSIM_CAR_ROS_PKGS")))
(unless (find-package "AIRSIM_CAR_ROS_PKGS::COLLISION")
  (make-package "AIRSIM_CAR_ROS_PKGS::COLLISION"))

(in-package "ROS")
;;//! \htmlinclude Collision.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass airsim_car_ros_pkgs::Collision
  :super ros::object
  :slots (_header _collision_detected _collision_count ))

(defmethod airsim_car_ros_pkgs::Collision
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:collision_detected __collision_detected) nil)
    ((:collision_count __collision_count) 0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _collision_detected __collision_detected)
   (setq _collision_count (round __collision_count))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:collision_detected
   (&optional __collision_detected)
   (if __collision_detected (setq _collision_detected __collision_detected)) _collision_detected)
  (:collision_count
   (&optional __collision_count)
   (if __collision_count (setq _collision_count __collision_count)) _collision_count)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; bool _collision_detected
    1
    ;; uint32 _collision_count
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; bool _collision_detected
       (if _collision_detected (write-byte -1 s) (write-byte 0 s))
     ;; uint32 _collision_count
       (write-long _collision_count s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; bool _collision_detected
     (setq _collision_detected (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; uint32 _collision_count
     (setq _collision_count (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get airsim_car_ros_pkgs::Collision :md5sum-) "132785abdef999140dfbb59e22a1f16f")
(setf (get airsim_car_ros_pkgs::Collision :datatype-) "airsim_car_ros_pkgs/Collision")
(setf (get airsim_car_ros_pkgs::Collision :definition-)
      "std_msgs/Header header
bool collision_detected
uint32 collision_count
# string vehicle_name
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

")



(provide :airsim_car_ros_pkgs/Collision "132785abdef999140dfbb59e22a1f16f")


