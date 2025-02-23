#include <iostream>
#include "hid_hidraw.hpp"

namespace hid_hidraw {

    hid_hidraw::hid_hidraw() : device(nullptr) {
        // Constructor
    }

    hid_hidraw::~hid_hidraw() {
        // Destructor
        closeDevice();
    }

    bool hid_hidraw::openDevice(uint16_t vendor_id, uint16_t product_id) {
        if (hid_init() < 0) {
            std::cerr << "Failed to initialize HIDAPI" << std::endl;
            return false;
        }

        device = hid_open(vendor_id, product_id, nullptr);
        if (!device) {
            std::cerr << "Failed to open device" << std::endl;
            return false;
        }

        return true;
    }

    void hid_hidraw::closeDevice() {
        if (device) {
            hid_close(device);
            device = nullptr;
        }
    }

    bool hid_hidraw::isConnected() const {
        return device != nullptr;
    }

    int hid_hidraw::sendData(const std::vector<uint8_t>& data) {
        if (!device) {
            std::cerr << "Device is not connected" << std::endl;
            return -1;
        }

        int res = hid_write(device, data.data(), data.size());
        return res;
    }

    int hid_hidraw::receiveData(std::vector<uint8_t>& data, int timeout) {
        if (!device) {
            std::cerr << "Device is not connected" << std::endl;
            return -1;
        }

        data.resize(64);
        int res = hid_read_timeout(device, data.data(), data.size(), timeout);

        if (res < 0) {
            std::cerr << "Failed to read data" << std::endl;
            return -1;
        }

        data.resize(res);
        return res;
    }

} // namespace hid_hidraw
