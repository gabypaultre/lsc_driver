#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include "lsc_servocontrol.hpp"

#define M_PI 3.14159265358979323846

void wait(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int main() {
    lsc_servocontrol::lsc_servocontrol controller;

    if (!controller.connect()) {
        std::cerr << "❌ Échec de connexion au contrôleur LSC !\n";
        return 1;
    }

    uint8_t servo_id = 1; 
    double angle_1 = -(M_PI / 2);
    double angle_2 = -(M_PI / 2) + (M_PI / 4);
    double angle_3 = -(M_PI / 2) + (M_PI / 2);
    double angle_4 = -(M_PI / 2) + (3 * M_PI / 4);
    double angle_5 = -(M_PI / 2) + M_PI;

    while(1) {
        controller.moveServo({{servo_id, angle_1}}, 10);
        wait(500);
        controller.moveServo({{servo_id, angle_2}}, 10);
        wait(500);
        controller.moveServo({{servo_id, angle_3}}, 10);
        wait(500);
        controller.moveServo({{servo_id, angle_4}}, 10);
        wait(500);
        controller.moveServo({{servo_id, angle_5}}, 10);
        wait(500);
    }
   
    controller.disconnect();

    return 0;
}
