;; Auto-generated. Do not edit!


(when (boundp 'airsim_car_ros_pkgs::CarCmd)
  (if (not (find-package "AIRSIM_CAR_ROS_PKGS"))
    (make-package "AIRSIM_CAR_ROS_PKGS"))
  (shadow 'CarCmd (find-package "AIRSIM_CAR_ROS_PKGS")))
(unless (find-package "AIRSIM_CAR_ROS_PKGS::CARCMD")
  (make-package "AIRSIM_CAR_ROS_PKGS::CARCMD"))

(in-package "ROS")
;;//! \htmlinclude CarCmd.msg.html


(defclass airsim_car_ros_pkgs::CarCmd
  :super ros::object
  :slots (_throttle _steering _brake _handbrake _is_manual_gear _manual_gear _gear_immediate ))

(defmethod airsim_car_ros_pkgs::CarCmd
  (:init
   (&key
    ((:throttle __throttle) 0.0)
    ((:steering __steering) 0.0)
    ((:brake __brake) 0.0)
    ((:handbrake __handbrake) nil)
    ((:is_manual_gear __is_manual_gear) nil)
    ((:manual_gear __manual_gear) 0)
    ((:gear_immediate __gear_immediate) nil)
    )
   (send-super :init)
   (setq _throttle (float __throttle))
   (setq _steering (float __steering))
   (setq _brake (float __brake))
   (setq _handbrake __handbrake)
   (setq _is_manual_gear __is_manual_gear)
   (setq _manual_gear (round __manual_gear))
   (setq _gear_immediate __gear_immediate)
   self)
  (:throttle
   (&optional __throttle)
   (if __throttle (setq _throttle __throttle)) _throttle)
  (:steering
   (&optional __steering)
   (if __steering (setq _steering __steering)) _steering)
  (:brake
   (&optional __brake)
   (if __brake (setq _brake __brake)) _brake)
  (:handbrake
   (&optional __handbrake)
   (if __handbrake (setq _handbrake __handbrake)) _handbrake)
  (:is_manual_gear
   (&optional __is_manual_gear)
   (if __is_manual_gear (setq _is_manual_gear __is_manual_gear)) _is_manual_gear)
  (:manual_gear
   (&optional __manual_gear)
   (if __manual_gear (setq _manual_gear __manual_gear)) _manual_gear)
  (:gear_immediate
   (&optional __gear_immediate)
   (if __gear_immediate (setq _gear_immediate __gear_immediate)) _gear_immediate)
  (:serialization-length
   ()
   (+
    ;; float64 _throttle
    8
    ;; float64 _steering
    8
    ;; float64 _brake
    8
    ;; bool _handbrake
    1
    ;; bool _is_manual_gear
    1
    ;; int32 _manual_gear
    4
    ;; bool _gear_immediate
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _throttle
       (sys::poke _throttle (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _steering
       (sys::poke _steering (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _brake
       (sys::poke _brake (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; bool _handbrake
       (if _handbrake (write-byte -1 s) (write-byte 0 s))
     ;; bool _is_manual_gear
       (if _is_manual_gear (write-byte -1 s) (write-byte 0 s))
     ;; int32 _manual_gear
       (write-long _manual_gear s)
     ;; bool _gear_immediate
       (if _gear_immediate (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _throttle
     (setq _throttle (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _steering
     (setq _steering (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _brake
     (setq _brake (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; bool _handbrake
     (setq _handbrake (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; bool _is_manual_gear
     (setq _is_manual_gear (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; int32 _manual_gear
     (setq _manual_gear (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; bool _gear_immediate
     (setq _gear_immediate (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(setf (get airsim_car_ros_pkgs::CarCmd :md5sum-) "ba7c9338c4c7891d8ccba23fd757db82")
(setf (get airsim_car_ros_pkgs::CarCmd :datatype-) "airsim_car_ros_pkgs/CarCmd")
(setf (get airsim_car_ros_pkgs::CarCmd :definition-)
      "float64 throttle
float64 steering
float64 brake
bool handbrake
bool is_manual_gear
int32 manual_gear
bool gear_immediate
# string vehicle_name

")



(provide :airsim_car_ros_pkgs/CarCmd "ba7c9338c4c7891d8ccba23fd757db82")


