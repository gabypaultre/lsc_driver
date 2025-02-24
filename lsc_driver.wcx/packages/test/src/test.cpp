#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include "lsc_servocontrol.hpp"

void wait(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void readAndPrintServoPositions(lsc_servocontrol::lsc_servocontrol& controller, const std::vector<uint8_t>& servos) {
    std::map<uint8_t, uint16_t> positions = controller.readServoPositions(servos);
    if (!positions.empty()) {
        std::cout << "✅ SUCCESS: Read servo positions:\n";
        for (const auto& [id, pos] : positions) {
            std::cout << "Servo " << static_cast<int>(id) << " Position: " << pos << std::endl;
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

    // 🔹 1️⃣ Move a single servo to a specific position
    std::cout << "\n🔹 Moving Servo 1 to 250" << std::endl;
    if (controller.moveServo({{servo1, 250}}, 1000)) {
        std::cout << "✅ SUCCESS: Servo 1 moved to 250.\n";
    } else {
        std::cerr << "❌ FAILED: moveServo for Servo 1.\n";
    }
    wait(2500);
    readAndPrintServoPositions(controller, {servo1});

    // 🔹 2️⃣ Move servos to center
    std::cout << "\n🔹 Moving all servos to center (500)" << std::endl;
    if (controller.moveServo({{servo1, 500}, {servo2, 500}, {servo3, 500}}, 2000)) {
        std::cout << "✅ SUCCESS: All servos at center (500).\n";
    } else {
        std::cerr << "❌ FAILED: moveServo for center position.\n";
    }
    wait(2500);
    readAndPrintServoPositions(controller, servos);
}

void testActionGroupInfinite(lsc_servocontrol::lsc_servocontrol& controller) {
    uint8_t actionGroup = 0;
    uint16_t repetitions = 0; // 0 = Infinite loop

    std::cout << "\n▶️ Running Action Group " << static_cast<int>(actionGroup) << " in infinite mode...\n";
    if (controller.runActionGroup(actionGroup, repetitions)) {
        std::cout << "✅ SUCCESS: Action Group " << static_cast<int>(actionGroup) << " started with infinite repetitions.\n";
    } else {
        std::cerr << "❌ FAILED: Could not start Action Group " << static_cast<int>(actionGroup) << ".\n";
        return;
    }

    // 🕒 Wait for 10 seconds before stopping
    for (int i = 0; i < 9; ++i) {
        std::cout << "⏳ Running... (" << i + 1 << "/10 sec)\r" << std::flush;
        wait(1000);
    }
    std::cout << std::endl;

    // ⏹️ Stopping the action group
    std::cout << "\n⏹️ Stopping Action Group " << static_cast<int>(actionGroup) << "...\n";
    if (controller.stopActionGroup()) {
        std::cout << "✅ SUCCESS: Action Group stopped.\n";
    } else {
        std::cerr << "❌ FAILED: Could not stop Action Group.\n";
    }

    // ✅ Verifying if it is really stopped
    wait(2000);
    if (controller.isActionGroupStopped()) {
        std::cout << "✅ SUCCESS: Verified that Action Group " << static_cast<int>(actionGroup) << " is stopped.\n";
    } else {
        std::cerr << "❌ FAILED: Action Group " << static_cast<int>(actionGroup) << " is still running!\n";
    }
}

int main() {
    lsc_servocontrol::lsc_servocontrol controller;

    // 🔹 1️⃣ Connect to the controller
    if (!controller.connect()) {
        std::cerr << "❌ FAILED: Could not connect to LSC controller!\n";
        return 1;
    }

    // 🔹 2️⃣ Read battery voltage before tests
    uint16_t batteryVoltage = 0;
    if (controller.getBatteryVoltage(batteryVoltage)) {
        std::cout << "🔋 Battery Voltage: " << batteryVoltage << " mV\n";
    } else {
        std::cerr << "❌ FAILED: Could not read battery voltage!\n";
    }

    // 🔹 3️⃣ Test moveServo configurations
    testMoveServoConfigurations(controller);

    // 🔹 4️⃣ Test Action Group Execution in Infinite Mode
    testActionGroupInfinite(controller);

    // 🔹 5️⃣ Power off servos
    std::cout << "\n🔻 Powering off servos\n";
    controller.powerOffServos({1, 2, 3});

    // 🔹 6️⃣ Disconnect
    controller.disconnect();
    std::cout << "\n🔌 Disconnected from controller.\n";

    return 0;
}
