
(cl:in-package :asdf)

(defsystem "shell_simulation-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "CarCmd" :depends-on ("_package_CarCmd"))
    (:file "_package_CarCmd" :depends-on ("_package"))
  ))