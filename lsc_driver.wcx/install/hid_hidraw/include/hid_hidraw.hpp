#ifndef __HID_HIDRAW_HPP__
#define __HID_HIDRAW_HPP__

#if defined(_WIN32) || defined(_WIN64)
    #ifdef __HID_HIDRAW_EXPORTS__
        #define HID_HIDRAW_API __declspec(dllexport)
    #else
        #define HID_HIDRAW_API __declspec(dllimport)
    #endif
#else
    #define HID_HIDRAW_API __attribute__((visibility("default")))
#endif

#include "hidapi/hidapi.h"
#include <vector>
#include <cstdint>

namespace hid_hidraw {

class HID_HIDRAW_API hid_hidraw {
public:
    explicit hid_hidraw();
    virtual ~hid_hidraw();
    
    bool openDevice(uint16_t vendor_id, uint16_t product_id);
    void closeDevice();
    bool isConnected() const;

    int sendData(const std::vector<uint8_t>& data);
    int receiveData(std::vector<uint8_t>& data, int timeout = 500);

private:
    hid_device* device = nullptr;
};

} // namespace hid_hidraw

#endif // __HID_HIDRAW_HPP__
