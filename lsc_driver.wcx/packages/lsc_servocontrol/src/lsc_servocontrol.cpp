#include <iostream>
#include "lsc_servocontrol.hpp"

namespace lsc_servocontrol {

    lsc_servocontrol::lsc_servocontrol() {
        // Constructor
    }

    lsc_servocontrol::~lsc_servocontrol() {
        // Destructor
        disconnect();
    }

    bool lsc_servocontrol::connect() {
        if (_hid_hidraw_instance.open(VENDOR_ID, PRODUCT_ID)) {
            std::cout << "HID Connection established" << std::endl;
            return true;
        } else {
            std::cout << "HID Connection failed" << std::endl;
            return false;
        }
    }

    void lsc_servocontrol::disconnect() {
        _hid_hidraw_instance.closeDevice();
        std::cout << "HID Connection closed" << std::endl;
    }

    bool lsc_servocontrol::isConnected() const {
        return _hid_hidraw_instance.isConnected();
    }

    bool lsc_servocontrol::sendCommand(uint8_t cmd, const std::vector<uint8_t>& params) {
        if (!isConnected()) {
            std::cout << "HID Connection not established" << std::endl;
            return false;
        }

        std::vector<uint8_t> packet = buildCommandPacket(cmd, params);

        if (_hid_hidraw_instance.sendData(packet) < 0) {
            std::cout << "Failed to send command" << std::endl;
            return false;
        }

        return true;
    }

    bool lsc_servocontrol::receiveResponse(std::vector<uint8_t>& response, int timeout) {
        if (!isConnected()) {
            std::cout << "HID Connection not established" << std::endl;
            return false;
        }

        response.clear();
        int bytesRead = _hid_hidraw_instance.receiveData(response, timeout);

        if (bytesRead <= 0) {
            std::cout << "Failed to receive response" << std::endl;
            return false;
        }

        if (bytesRead < 2) {
            std::cout << "Response too short" << std::endl;
            return false;
        }

        if (response[0] != HEADER[0] || response[1] != HEADER[1]) {
            std::cout << "Invalid response header" << std::endl;
            return false;
        }

        return true;
    }

    bool lsc_servocontrol::moveServo(const std::vector<std::tuple<uint8_t, uint16_t>>& servos, uint16_t time) {
        if (!isConnected()) {
            std::cout << "HID Connection not established" << std::endl;
            return false;
        }

        if (servos.empty()) {
            std::cout << "No servos specified" << std::endl;
            return false;
        }

        std::vector<uint8_t> params;
        params.push_back(static_cast<uint8_t>(servos.size()));

        params.push_back(static_cast<uint8_t>(time & 0xFF)); // Time LSB
        params.push_back(static_cast<uint8_t>((time >> 8) & 0xFF)); // Time MSB

        for (const auto& servo : servos) {
            uint8_t id = std::get<0>(servo);
            uint16_t position = std::get<1>(servo);

            params.push_back(id);
            params.push_back(static_cast<uint8_t>(position & 0xFF)); // Position LSB
            params.push_back(static_cast<uint8_t>((position >> 8) & 0xFF)); // Position MSB
        }

        return sendCommand(CMD_SERVO_MOVE, params);
    }

    bool lsc_servocontrol::getBatteryVoltage(uint16_t& voltage) {
        if (!isConnected()) {
            std::cout << "HID Connection not established" << std::endl;
            return false;
        }

        if (!sendCommand(CMD_GET_BATTERY_VOLTAGE, {})) {
            std::cout << "Failed to send command" << std::endl;
            return false;
        }

        std::vector<uint8_t> response;
        if (!receiveResponse(response)) {
            std::cout << "Failed to receive response" << std::endl;
            return false;
        }

        if (response.size() != 6 || response[2] != 0x04 || response[3] != CMD_GET_BATTERY_VOLTAGE) {
            std::cout << "Invalid response" << std::endl;
            return false;
        }

        voltage = static_cast<uint16_t>(response[4]) | (static_cast<uint16_t>(response[5]) << 8);
        std::cout << "Battery Voltage: " << voltage << " mV" << std::endl;

        return true;
    }

    bool lsc_servocontrol::powerOffServos(const std::vector<uint8_t>& servo_ids) {
        if (!isConnected()) {
            std::cout << "HID Connection not established" << std::endl;
            return false;
        }

        if (servo_ids.empty()) {
            std::cout << "No servos specified" << std::endl;
            return false;
        }

        std::vector<uint8_t> params;
        params.push_back(static_cast<uint8_t>(servo_ids.size()));

        params.insert(params.end(), servo_ids.begin(), servo_ids.end());

        return sendCommand(CMD_MULT_SERVO_UNLOAD, params);
    }

    std::map<uint8_t, uint16_t> lsc_servocontrol::readServoPositions(const std::vector<uint8_t>& servo_ids) {
        std::map<uint8_t, uint16_t> positions;

        if (!isConnected()) {
            std::cout << "HID Connection not established" << std::endl;
            return positions;
        }

        if (servo_ids.empty()) {
            std::cout << "No servos specified" << std::endl;
            return positions;
        }

        std::vector<uint8_t> params;
        params.push_back(static_cast<uint8_t>(servo_ids.size()));

        params.insert(params.end(), servo_ids.begin(), servo_ids.end());

        if (!sendCommand(CMD_MULT_SERVO_POS_READ, params)) {
            std::cout << "Failed to send command" << std::endl;
            return positions;
        }

        // Wait for response
        std::vector<uint8_t> response;
        if (!receiveResponse(response)) {
            std::cout << "Failed to receive response" << std::endl;
            return positions;
        }

        if (response.size() < 5 || response[3] != CMD_MULT_SERVO_POS_READ) {
            std::cout << "Invalid response" << std::endl;
            return positions;
        }

        // Number of servos in the response
        uint8_t numServos = response[4];
        if (numServos != servo_ids.size()) {
            std::cout << "Warning: Number of servos in response does not match request" << std::endl;
        }

        // Extract positions
        size_t index = 5; // Position after `Header`, `Length`, `Command`, `Nb Servos`
        for (uint8_t i = 0; i < numServos; ++i) {
            if (index + 2 >= response.size()) {
                std::cout << "Warning: Incomplete position data for servo " << (int)response[index] << std::endl;
                break;
            }

            uint8_t servo_id = response[index];
            uint16_t position = static_cast<uint16_t>(response[index + 1]) | (static_cast<uint16_t>(response[index + 2]) << 8);
            positions[servo_id] = position;

            index += 3; // Move to the next block of 3 bytes (ID, Pos LSB, Pos MSB)
        }

        return positions;
    }

    std::vector<uint8_t> lsc_servocontrol::buildCommandPacket(uint8_t cmd, const std::vector<uint8_t>& params) {
        std::vector<uint8_t> packet;

        // Add fixed header [0x55, 0x55]
        packet.push_back(HEADER[0]);
        packet.push_back(HEADER[1]);

        // Calculate total message length (params.size() + Command + Length itself)
        uint8_t length = static_cast<uint8_t>(params.size() + 2);
        packet.push_back(length);

        // Add command
        packet.push_back(cmd);

        // Add parameters if present
        packet.insert(packet.end(), params.begin(), params.end());

        return packet;
    }

} // namespace lsc_servocontrol
