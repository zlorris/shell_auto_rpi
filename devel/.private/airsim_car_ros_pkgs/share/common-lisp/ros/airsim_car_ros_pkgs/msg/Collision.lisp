; Auto-generated. Do not edit!


(cl:in-package airsim_car_ros_pkgs-msg)


;//! \htmlinclude Collision.msg.html

(cl:defclass <Collision> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (collision_detected
    :reader collision_detected
    :initarg :collision_detected
    :type cl:boolean
    :initform cl:nil)
   (collision_count
    :reader collision_count
    :initarg :collision_count
    :type cl:integer
    :initform 0))
)

(cl:defclass Collision (<Collision>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Collision>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Collision)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name airsim_car_ros_pkgs-msg:<Collision> is deprecated: use airsim_car_ros_pkgs-msg:Collision instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <Collision>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader airsim_car_ros_pkgs-msg:header-val is deprecated.  Use airsim_car_ros_pkgs-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'collision_detected-val :lambda-list '(m))
(cl:defmethod collision_detected-val ((m <Collision>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader airsim_car_ros_pkgs-msg:collision_detected-val is deprecated.  Use airsim_car_ros_pkgs-msg:collision_detected instead.")
  (collision_detected m))

(cl:ensure-generic-function 'collision_count-val :lambda-list '(m))
(cl:defmethod collision_count-val ((m <Collision>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader airsim_car_ros_pkgs-msg:collision_count-val is deprecated.  Use airsim_car_ros_pkgs-msg:collision_count instead.")
  (collision_count m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Collision>) ostream)
  "Serializes a message object of type '<Collision>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'collision_detected) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'collision_count)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'collision_count)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'collision_count)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'collision_count)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Collision>) istream)
  "Deserializes a message object of type '<Collision>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:setf (cl:slot-value msg 'collision_detected) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'collision_count)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'collision_count)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'collision_count)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'collision_count)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Collision>)))
  "Returns string type for a message object of type '<Collision>"
  "airsim_car_ros_pkgs/Collision")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Collision)))
  "Returns string type for a message object of type 'Collision"
  "airsim_car_ros_pkgs/Collision")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Collision>)))
  "Returns md5sum for a message object of type '<Collision>"
  "132785abdef999140dfbb59e22a1f16f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Collision)))
  "Returns md5sum for a message object of type 'Collision"
  "132785abdef999140dfbb59e22a1f16f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Collision>)))
  "Returns full string definition for message of type '<Collision>"
  (cl:format cl:nil "std_msgs/Header header~%bool collision_detected~%uint32 collision_count~%# string vehicle_name~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Collision)))
  "Returns full string definition for message of type 'Collision"
  (cl:format cl:nil "std_msgs/Header header~%bool collision_detected~%uint32 collision_count~%# string vehicle_name~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Collision>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     1
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Collision>))
  "Converts a ROS message object to a list"
  (cl:list 'Collision
    (cl:cons ':header (header msg))
    (cl:cons ':collision_detected (collision_detected msg))
    (cl:cons ':collision_count (collision_count msg))
))
