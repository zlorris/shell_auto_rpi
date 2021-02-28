; Auto-generated. Do not edit!


(cl:in-package airsim_car_ros_pkgs-msg)


;//! \htmlinclude CarCmd.msg.html

(cl:defclass <CarCmd> (roslisp-msg-protocol:ros-message)
  ((throttle
    :reader throttle
    :initarg :throttle
    :type cl:float
    :initform 0.0)
   (steering
    :reader steering
    :initarg :steering
    :type cl:float
    :initform 0.0)
   (brake
    :reader brake
    :initarg :brake
    :type cl:float
    :initform 0.0)
   (handbrake
    :reader handbrake
    :initarg :handbrake
    :type cl:boolean
    :initform cl:nil)
   (is_manual_gear
    :reader is_manual_gear
    :initarg :is_manual_gear
    :type cl:boolean
    :initform cl:nil)
   (manual_gear
    :reader manual_gear
    :initarg :manual_gear
    :type cl:integer
    :initform 0)
   (gear_immediate
    :reader gear_immediate
    :initarg :gear_immediate
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass CarCmd (<CarCmd>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <CarCmd>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'CarCmd)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name airsim_car_ros_pkgs-msg:<CarCmd> is deprecated: use airsim_car_ros_pkgs-msg:CarCmd instead.")))

(cl:ensure-generic-function 'throttle-val :lambda-list '(m))
(cl:defmethod throttle-val ((m <CarCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader airsim_car_ros_pkgs-msg:throttle-val is deprecated.  Use airsim_car_ros_pkgs-msg:throttle instead.")
  (throttle m))

(cl:ensure-generic-function 'steering-val :lambda-list '(m))
(cl:defmethod steering-val ((m <CarCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader airsim_car_ros_pkgs-msg:steering-val is deprecated.  Use airsim_car_ros_pkgs-msg:steering instead.")
  (steering m))

(cl:ensure-generic-function 'brake-val :lambda-list '(m))
(cl:defmethod brake-val ((m <CarCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader airsim_car_ros_pkgs-msg:brake-val is deprecated.  Use airsim_car_ros_pkgs-msg:brake instead.")
  (brake m))

(cl:ensure-generic-function 'handbrake-val :lambda-list '(m))
(cl:defmethod handbrake-val ((m <CarCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader airsim_car_ros_pkgs-msg:handbrake-val is deprecated.  Use airsim_car_ros_pkgs-msg:handbrake instead.")
  (handbrake m))

(cl:ensure-generic-function 'is_manual_gear-val :lambda-list '(m))
(cl:defmethod is_manual_gear-val ((m <CarCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader airsim_car_ros_pkgs-msg:is_manual_gear-val is deprecated.  Use airsim_car_ros_pkgs-msg:is_manual_gear instead.")
  (is_manual_gear m))

(cl:ensure-generic-function 'manual_gear-val :lambda-list '(m))
(cl:defmethod manual_gear-val ((m <CarCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader airsim_car_ros_pkgs-msg:manual_gear-val is deprecated.  Use airsim_car_ros_pkgs-msg:manual_gear instead.")
  (manual_gear m))

(cl:ensure-generic-function 'gear_immediate-val :lambda-list '(m))
(cl:defmethod gear_immediate-val ((m <CarCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader airsim_car_ros_pkgs-msg:gear_immediate-val is deprecated.  Use airsim_car_ros_pkgs-msg:gear_immediate instead.")
  (gear_immediate m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <CarCmd>) ostream)
  "Serializes a message object of type '<CarCmd>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'throttle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'steering))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'brake))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'handbrake) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'is_manual_gear) 1 0)) ostream)
  (cl:let* ((signed (cl:slot-value msg 'manual_gear)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'gear_immediate) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <CarCmd>) istream)
  "Deserializes a message object of type '<CarCmd>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'throttle) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'steering) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'brake) (roslisp-utils:decode-double-float-bits bits)))
    (cl:setf (cl:slot-value msg 'handbrake) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'is_manual_gear) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'manual_gear) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:setf (cl:slot-value msg 'gear_immediate) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<CarCmd>)))
  "Returns string type for a message object of type '<CarCmd>"
  "airsim_car_ros_pkgs/CarCmd")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'CarCmd)))
  "Returns string type for a message object of type 'CarCmd"
  "airsim_car_ros_pkgs/CarCmd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<CarCmd>)))
  "Returns md5sum for a message object of type '<CarCmd>"
  "ba7c9338c4c7891d8ccba23fd757db82")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'CarCmd)))
  "Returns md5sum for a message object of type 'CarCmd"
  "ba7c9338c4c7891d8ccba23fd757db82")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<CarCmd>)))
  "Returns full string definition for message of type '<CarCmd>"
  (cl:format cl:nil "float64 throttle~%float64 steering~%float64 brake~%bool handbrake~%bool is_manual_gear~%int32 manual_gear~%bool gear_immediate~%# string vehicle_name~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'CarCmd)))
  "Returns full string definition for message of type 'CarCmd"
  (cl:format cl:nil "float64 throttle~%float64 steering~%float64 brake~%bool handbrake~%bool is_manual_gear~%int32 manual_gear~%bool gear_immediate~%# string vehicle_name~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <CarCmd>))
  (cl:+ 0
     8
     8
     8
     1
     1
     4
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <CarCmd>))
  "Converts a ROS message object to a list"
  (cl:list 'CarCmd
    (cl:cons ':throttle (throttle msg))
    (cl:cons ':steering (steering msg))
    (cl:cons ':brake (brake msg))
    (cl:cons ':handbrake (handbrake msg))
    (cl:cons ':is_manual_gear (is_manual_gear msg))
    (cl:cons ':manual_gear (manual_gear msg))
    (cl:cons ':gear_immediate (gear_immediate msg))
))
