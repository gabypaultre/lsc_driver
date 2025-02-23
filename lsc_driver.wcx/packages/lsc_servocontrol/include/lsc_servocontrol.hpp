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

#include "hid_hidraw.hpp"

namespace lsc_servocontrol {
    class LSC_SERVOCONTROL_API lsc_servocontrol {
        public:
            explicit lsc_servocontrol(int count = 0);
            virtual ~lsc_servocontrol();
            void hello();

        private:
			hid_hidraw::hid_hidraw _hid_hidraw_instance;
            int _count;
    };
} // namespace lsc_servocontrol
#endif // __LSC_SERVOCONTROL_HPP__
