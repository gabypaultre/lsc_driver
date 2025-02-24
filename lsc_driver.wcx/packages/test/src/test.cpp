#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include "lsc_servocontrol.hpp"

void wait(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void readAndPrintServoPositions(lsc_servocontrol::lsc_servocontrol& controller, 
                                const std::vector<uint8_t>& servos) {
    std::map<uint8_t, uint16_t> positions = controller.readServoPositions(servos);
    if (!positions.empty()) {
        std::cout << "✅ SUCCESS: Read servo positions:\n";
        for (const auto& [id, pos] : positions) {
            std::cout << "Servo " << (int)id << " Position: " << pos << std::endl;
        }
    } else {
        std::cerr << "❌ FAILED: Could not read servo positions.\n";
    }
    std::cout << "-----------------------------" << std::endl;
}

void testMoveServoConfigurations(lsc_servocontrol::lsc_servocontrol& controller) {
    std::cout << "\n⚙️  Testing moveServo configurations...\n";

    uint8_t servo1 = 1, servo2 = 2, servo3 = 3;
    std::vector<uint8_t> servos = {servo1, servo2, servo3};

    // 🔹 1️⃣ Déplacer un seul servo à une position précise
    std::cout << "\n🔹 Moving Servo 1 to 250" << std::endl;
    if (controller.moveServo({{servo1, 250}}, 1000)) {
        std::cout << "✅ SUCCESS: Servo 1 moved to 250.\n";
    } else {
        std::cerr << "❌ FAILED: moveServo for Servo 1.\n";
    }
    wait(2000);
    readAndPrintServoPositions(controller, {servo1});

    // 🔹 2️⃣ Déplacer deux servos ensemble
    std::cout << "\n🔹 Moving Servo 1 to 500 and Servo 2 to 750" << std::endl;
    if (controller.moveServo({{servo1, 500}, {servo2, 750}}, 1000)) {
        std::cout << "✅ SUCCESS: Servo 1 -> 500, Servo 2 -> 750.\n";
    } else {
        std::cerr << "❌ FAILED: moveServo for Servo 1 and 2.\n";
    }
    wait(2000);
    readAndPrintServoPositions(controller, {servo1, servo2});

    // 🔹 3️⃣ Déplacer trois servos ensemble
    std::cout << "\n🔹 Moving all servos to [Servo1=1000, Servo2=500, Servo3=0]" << std::endl;
    if (controller.moveServo({{servo1, 1000}, {servo2, 500}, {servo3, 0}}, 1000)) {
        std::cout << "✅ SUCCESS: Servo 1 -> 1000, Servo 2 -> 500, Servo 3 -> 0.\n";
    } else {
        std::cerr << "❌ FAILED: moveServo for all servos.\n";
    }
    wait(2000);
    readAndPrintServoPositions(controller, servos);

    // 🔹 4️⃣ Déplacer les servos avec des valeurs limites
    std::cout << "\n🔹 Moving Servo 1 to min (0) and Servo 3 to max (1000)" << std::endl;
    if (controller.moveServo({{servo1, 0}, {servo3, 1000}}, 1000)) {
        std::cout << "✅ SUCCESS: Servo 1 -> 0, Servo 3 -> 1000.\n";
    } else {
        std::cerr << "❌ FAILED: moveServo for Servo 1 and 3.\n";
    }
    wait(2000);
    readAndPrintServoPositions(controller, {servo1, servo3});

    // 🔹 5️⃣ Test de retour au centre
    std::cout << "\n🔹 Moving all servos to center (500)" << std::endl;
    if (controller.moveServo({{servo1, 500}, {servo2, 500}, {servo3, 500}}, 1000)) {
        std::cout << "✅ SUCCESS: All servos at center (500).\n";
    } else {
        std::cerr << "❌ FAILED: moveServo for center position.\n";
    }
    wait(2000);
    readAndPrintServoPositions(controller, servos);
}

int main() {
    lsc_servocontrol::lsc_servocontrol controller;

    // 🔹 1️⃣ Connexion au contrôleur
    if (!controller.connect()) {
        std::cerr << "❌ FAILED: Could not connect to LSC controller!\n";
        return 1;
    }

    // 🔹 2️⃣ Lire la tension de la batterie avant les tests
    uint16_t batteryVoltage = 0;
    if (controller.getBatteryVoltage(batteryVoltage)) {
        std::cout << "🔋 Battery Voltage: " << batteryVoltage << " mV\n";
    } else {
        std::cerr << "❌ FAILED: Could not read battery voltage!\n";
    }

    // 🔹 3️⃣ Test des différentes configurations de `moveServo`
    testMoveServoConfigurations(controller);

    // 🔹 4️⃣ Décharger les servos après les tests
    std::cout << "\n🔻 Powering off servos\n";
    controller.powerOffServos({1, 2, 3});

    // 🔹 5️⃣ Déconnexion
    controller.disconnect();
    std::cout << "\n🔌 Disconnected from controller.\n";

    return 0;
}
