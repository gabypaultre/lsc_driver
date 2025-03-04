#ifndef __LSC_SERVOCONTROL_HPP__
#define __LSC_SERVOCONTROL_HPP__

#if defined(_WIN32) || defined(_WIN64)
    #ifdef __LSC_SERVOCONTROL_EXPORTS__
        #define LSC_SERVOCONTROL_API __declspec(dllexport)
    #else
        #define LSC_SERVOCONTROL_API __declspec(dllimport)
    #endif
#else
    #define LSC_SERVOCONTROL_API __attribute__((visibility("default")))
#endif

#include <vector>
#include <array>
#include <cstdint>
#include <tuple>
#include <map>
#include "hid_hidraw.hpp"

namespace lsc_servocontrol {

class LSC_SERVOCONTROL_API lsc_servocontrol {
public:
    explicit lsc_servocontrol();
    virtual ~lsc_servocontrol();

    bool connect();
    void disconnect();
    bool isConnected() const;

    bool sendCommand(uint8_t cmd, const std::vector<uint8_t>& params);
    bool receiveResponse(std::vector<uint8_t>& response, int timeout = 500);

    // bool moveServo(const std::vector<std::tuple<uint8_t, uint16_t>>& servos, uint16_t time);
    bool moveServo(const std::vector<std::tuple<uint8_t, double>>& servos, uint16_t time);

    bool getBatteryVoltage(uint16_t& voltage);
    bool powerOffServos(const std::vector<uint8_t>& servo_ids);
    
    //std::map<uint8_t, uint16_t> readServoPositions(const std::vector<uint8_t>& servo_ids);
    std::map<uint8_t, double> readServoPositions(const std::vector<uint8_t>& servo_ids);

    bool runActionGroup(uint8_t group_id, uint16_t repetitions);
    bool stopActionGroup();
    bool setActionGroupSpeed(uint8_t group_id, uint16_t speed);

    bool isActionGroupRunning(uint8_t& group_id, uint16_t& repetitions);
    bool isActionGroupStopped();
    bool isActionGroupComplete(uint8_t& group_id, uint16_t& repetitions);


private:
    hid_hidraw::hid_hidraw _hid_hidraw_instance;

    static constexpr uint16_t VENDOR_ID = 0x0483;
    static constexpr uint16_t PRODUCT_ID = 0x5750;
    static constexpr std::array<uint8_t, 2> HEADER = {0x55, 0x55};

    static constexpr uint8_t CMD_SERVO_MOVE = 0x03;
    static constexpr uint8_t CMD_GET_BATTERY_VOLTAGE = 0x0F;
    static constexpr uint8_t CMD_MULT_SERVO_UNLOAD = 0x14;
    static constexpr uint8_t CMD_MULT_SERVO_POS_READ = 0x15;

    static constexpr uint8_t CMD_ACTION_GROUP_RUN = 0x06;
    static constexpr uint8_t CMD_ACTION_STOP = 0x07;
    static constexpr uint8_t CMD_ACTION_SPEED = 0x0B;

    static constexpr uint8_t CMD_ACTION_GROUP_STOP = 0x07; //Same as CMD_ACTION_STOP but not same function
    static constexpr uint8_t CMD_ACTION_GROUP_COMPLETE = 0x08;

    static uint16_t radiansToPosition(double radians);
    static double positionToRadians(uint16_t position);

    std::vector<uint8_t> buildCommandPacket(uint8_t cmd, const std::vector<uint8_t>& params);
};

} // namespace lsc_servocontrol

#endif // __LSC_SERVOCONTROL_HPP__
