#include <iostream>
#include "lsc_servocontrol.hpp"

namespace lsc_servocontrol {
    lsc_servocontrol::lsc_servocontrol(int count) : _count(count)
						,_hid_hidraw_instance(count)
    {
        // Constructor
    }

    lsc_servocontrol::~lsc_servocontrol() 
    {
        // Destructor
    }

    void lsc_servocontrol::hello() {
        std::cout << std::endl << _count << ". Hello from lsc_servocontrol!" << std::endl;
		_hid_hidraw_instance.hello();
        _count++;
    }
} // namespace lsc_servocontrol
