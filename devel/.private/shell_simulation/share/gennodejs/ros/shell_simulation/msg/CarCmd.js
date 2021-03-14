// Auto-generated. Do not edit!

// (in-package shell_simulation.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class CarCmd {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.throttle = null;
      this.steering = null;
      this.brake = null;
      this.handbrake = null;
      this.is_manual_gear = null;
      this.manual_gear = null;
      this.gear_immediate = null;
    }
    else {
      if (initObj.hasOwnProperty('throttle')) {
        this.throttle = initObj.throttle
      }
      else {
        this.throttle = 0.0;
      }
      if (initObj.hasOwnProperty('steering')) {
        this.steering = initObj.steering
      }
      else {
        this.steering = 0.0;
      }
      if (initObj.hasOwnProperty('brake')) {
        this.brake = initObj.brake
      }
      else {
        this.brake = 0.0;
      }
      if (initObj.hasOwnProperty('handbrake')) {
        this.handbrake = initObj.handbrake
      }
      else {
        this.handbrake = false;
      }
      if (initObj.hasOwnProperty('is_manual_gear')) {
        this.is_manual_gear = initObj.is_manual_gear
      }
      else {
        this.is_manual_gear = false;
      }
      if (initObj.hasOwnProperty('manual_gear')) {
        this.manual_gear = initObj.manual_gear
      }
      else {
        this.manual_gear = 0;
      }
      if (initObj.hasOwnProperty('gear_immediate')) {
        this.gear_immediate = initObj.gear_immediate
      }
      else {
        this.gear_immediate = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type CarCmd
    // Serialize message field [throttle]
    bufferOffset = _serializer.float64(obj.throttle, buffer, bufferOffset);
    // Serialize message field [steering]
    bufferOffset = _serializer.float64(obj.steering, buffer, bufferOffset);
    // Serialize message field [brake]
    bufferOffset = _serializer.float64(obj.brake, buffer, bufferOffset);
    // Serialize message field [handbrake]
    bufferOffset = _serializer.bool(obj.handbrake, buffer, bufferOffset);
    // Serialize message field [is_manual_gear]
    bufferOffset = _serializer.bool(obj.is_manual_gear, buffer, bufferOffset);
    // Serialize message field [manual_gear]
    bufferOffset = _serializer.int32(obj.manual_gear, buffer, bufferOffset);
    // Serialize message field [gear_immediate]
    bufferOffset = _serializer.bool(obj.gear_immediate, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type CarCmd
    let len;
    let data = new CarCmd(null);
    // Deserialize message field [throttle]
    data.throttle = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [steering]
    data.steering = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [brake]
    data.brake = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [handbrake]
    data.handbrake = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [is_manual_gear]
    data.is_manual_gear = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [manual_gear]
    data.manual_gear = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [gear_immediate]
    data.gear_immediate = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 31;
  }

  static datatype() {
    // Returns string type for a message object
    return 'shell_simulation/CarCmd';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'ba7c9338c4c7891d8ccba23fd757db82';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float64 throttle
    float64 steering
    float64 brake
    bool handbrake
    bool is_manual_gear
    int32 manual_gear
    bool gear_immediate
    # string vehicle_name
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new CarCmd(null);
    if (msg.throttle !== undefined) {
      resolved.throttle = msg.throttle;
    }
    else {
      resolved.throttle = 0.0
    }

    if (msg.steering !== undefined) {
      resolved.steering = msg.steering;
    }
    else {
      resolved.steering = 0.0
    }

    if (msg.brake !== undefined) {
      resolved.brake = msg.brake;
    }
    else {
      resolved.brake = 0.0
    }

    if (msg.handbrake !== undefined) {
      resolved.handbrake = msg.handbrake;
    }
    else {
      resolved.handbrake = false
    }

    if (msg.is_manual_gear !== undefined) {
      resolved.is_manual_gear = msg.is_manual_gear;
    }
    else {
      resolved.is_manual_gear = false
    }

    if (msg.manual_gear !== undefined) {
      resolved.manual_gear = msg.manual_gear;
    }
    else {
      resolved.manual_gear = 0
    }

    if (msg.gear_immediate !== undefined) {
      resolved.gear_immediate = msg.gear_immediate;
    }
    else {
      resolved.gear_immediate = false
    }

    return resolved;
    }
};

module.exports = CarCmd;
