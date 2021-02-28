
(cl:in-package :asdf)

(defsystem "airsim_car_ros_pkgs-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Land" :depends-on ("_package_Land"))
    (:file "_package_Land" :depends-on ("_package"))
    (:file "LandGroup" :depends-on ("_package_LandGroup"))
    (:file "_package_LandGroup" :depends-on ("_package"))
    (:file "Reset" :depends-on ("_package_Reset"))
    (:file "_package_Reset" :depends-on ("_package"))
    (:file "SetGPSPosition" :depends-on ("_package_SetGPSPosition"))
    (:file "_package_SetGPSPosition" :depends-on ("_package"))
    (:file "SetLocalPosition" :depends-on ("_package_SetLocalPosition"))
    (:file "_package_SetLocalPosition" :depends-on ("_package"))
    (:file "Takeoff" :depends-on ("_package_Takeoff"))
    (:file "_package_Takeoff" :depends-on ("_package"))
    (:file "TakeoffGroup" :depends-on ("_package_TakeoffGroup"))
    (:file "_package_TakeoffGroup" :depends-on ("_package"))
  ))