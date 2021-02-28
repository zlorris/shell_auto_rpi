// Auto-generated. Do not edit!

// (in-package airsim_car_ros_pkgs.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class Collision {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.collision_detected = null;
      this.collision_count = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('collision_detected')) {
        this.collision_detected = initObj.collision_detected
      }
      else {
        this.collision_detected = false;
      }
      if (initObj.hasOwnProperty('collision_count')) {
        this.collision_count = initObj.collision_count
      }
      else {
        this.collision_count = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type Collision
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [collision_detected]
    bufferOffset = _serializer.bool(obj.collision_detected, buffer, bufferOffset);
    // Serialize message field [collision_count]
    bufferOffset = _serializer.uint32(obj.collision_count, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type Collision
    let len;
    let data = new Collision(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [collision_detected]
    data.collision_detected = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [collision_count]
    data.collision_count = _deserializer.uint32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    return length + 5;
  }

  static datatype() {
    // Returns string type for a message object
    return 'airsim_car_ros_pkgs/Collision';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '132785abdef999140dfbb59e22a1f16f';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    std_msgs/Header header
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new Collision(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.collision_detected !== undefined) {
      resolved.collision_detected = msg.collision_detected;
    }
    else {
      resolved.collision_detected = false
    }

    if (msg.collision_count !== undefined) {
      resolved.collision_count = msg.collision_count;
    }
    else {
      resolved.collision_count = 0
    }

    return resolved;
    }
};

module.exports = Collision;
