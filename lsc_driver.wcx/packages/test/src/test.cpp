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
    double angle = M_PI;

    std::cout << "🔄 Déplacement du servo ID " << (int)servo_id << " vers " << angle << " rad\n";
    controller.moveServo({{servo_id, angle}}, 1000);

    wait(1200);

    std::map<uint8_t, double> positions = controller.readServoPositions({servo_id});

    if (positions.find(servo_id) != positions.end()) {
        std::cout << "Position actuelle du servo " << (int)servo_id 
                  << " : " << positions[servo_id] << " rad\n";
    } else {
        std::cerr << "❌ Échec de lecture de la position du servo " << (int)servo_id << "\n";
    }

    controller.disconnect();
    return 0;
}
