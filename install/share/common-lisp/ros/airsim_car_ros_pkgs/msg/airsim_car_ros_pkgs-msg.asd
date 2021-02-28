
(cl:in-package :asdf)

(defsystem "airsim_car_ros_pkgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "CarCmd" :depends-on ("_package_CarCmd"))
    (:file "_package_CarCmd" :depends-on ("_package"))
    (:file "Collision" :depends-on ("_package_Collision"))
    (:file "_package_Collision" :depends-on ("_package"))
    (:file "GPSYaw" :depends-on ("_package_GPSYaw"))
    (:file "_package_GPSYaw" :depends-on ("_package"))
    (:file "GimbalAngleEulerCmd" :depends-on ("_package_GimbalAngleEulerCmd"))
    (:file "_package_GimbalAngleEulerCmd" :depends-on ("_package"))
    (:file "GimbalAngleQuatCmd" :depends-on ("_package_GimbalAngleQuatCmd"))
    (:file "_package_GimbalAngleQuatCmd" :depends-on ("_package"))
    (:file "VelCmd" :depends-on ("_package_VelCmd"))
    (:file "_package_VelCmd" :depends-on ("_package"))
    (:file "VelCmdGroup" :depends-on ("_package_VelCmdGroup"))
    (:file "_package_VelCmdGroup" :depends-on ("_package"))
  ))