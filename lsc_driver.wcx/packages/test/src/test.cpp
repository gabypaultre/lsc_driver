#include <iostream>
#include <chrono>
#include <thread>

#include "hid_hidraw.hpp"
#include "lsc_servocontrol.hpp"

int main() {
    int count = 0;
	hid_hidraw::hid_hidraw hid_hidraw_instance(count);
	lsc_servocontrol::lsc_servocontrol lsc_servocontrol_instance(count);
    while(true) {
        std::cout << std::endl << count << ". Hello from test!" << std::endl;
		hid_hidraw_instance.hello();
		lsc_servocontrol_instance.hello();
        count ++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
