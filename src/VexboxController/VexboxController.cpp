#ifndef VEXBOXCONTROLLER_HPP
#define VEXBOXCONTROLLER_HPP

#include "VexboxController/VexboxController.hpp"
#include <algorithm>
#include <iostream>
#include <string>

VexboxController::VexboxController(pros::controller_id_e_t id)
    : pros::Controller(id),
      controllerTask([this]() { this->update_controller(); },
                     "VexboxController") {}

void VexboxController::update_controller() {
  while (true) {
    std::string input;
    std::getline(std::cin, input); // Use getline to read the entire line

    // if input contains XBOXRESTART at all clear cin
    if (input.find("XBOXRESTART") != std::string::npos) {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      continue;
    }
    if (input.rfind("xbox", 0) == 0) {
      std::string type = input.substr(4, input.find("_") - 4);
      std::string id = input.substr(input.find("_") + 1,
                                    input.find("$$") - input.find("_") - 1);
      std::string value = input.substr(input.find("$$") + 2);
      if (type == "axis") {
        double axis_value = std::stod(value); // -1 to 1
        int clamped_value =
            std::clamp(static_cast<int>(axis_value * 127.0), -127, 127);
        if (id == "leftX") {
          leftHorizontal = clamped_value;
        } else if (id == "leftY") {
          leftVertical = clamped_value;
        } else if (id == "rightX") {
          rightHorizontal = clamped_value;
        } else if (id == "rightY") {
          rightVertical = clamped_value;
        } else if (id == "leftTrigger") {
          leftTrigger = axis_value > 0.0 ? 1 : 0;
        } else if (id == "rightTrigger") {
          rightTrigger = axis_value > 0.0 ? 1 : 0;
        }
      } else if (type == "button") {
        int pressed = std::stoi(value);
        if (id == "A") {
          A = pressed;
          if (A)
            A_new = 1;
        } else if (id == "B") {
          B = pressed;
          if (B)
            B_new = 1;
        } else if (id == "X") {
          X = pressed;
          if (X)
            X_new = 1;
        } else if (id == "Y") {
          Y = pressed;
          if (Y)
            Y_new = 1;
        } else if (id == "LB") {
          LB = pressed;
          if (LB)
            LB_new = 1;
        } else if (id == "RB") {
          RB = pressed;
          if (RB)
            RB_new = 1;
        } else if (id == "VIEW") {
          VIEW = pressed;
          if (VIEW)
            VIEW_new = 1;
        } else if (id == "MENU") {
          MENU = pressed;
          if (MENU)
            MENU_new = 1;
        } else if (id == "LSB") {
          LSB = pressed;
          if (LSB)
            LSB_new = 1;
        } else if (id == "RSB") {
          RSB = pressed;
          if (RSB)
            RSB_new = 1;
        }
      } else if (type == "dpad") {
        if (id == "AXIS-HORIZONTAL") {
          if (value == "1") {
            DPL = 0;
            DPL_new = 0;
            DPR = 1;
            DPR_new = 1;
          } else if (value == "-1") {
            DPL = -1;
            DPL_new = 1;
            DPR = 0;
            DPR_new = 0;
          } else {
            DPL = 0;
            DPL_new = 0;
            DPR = 0;
            DPR_new = 0;
          }
        } else if (id == "AXIS-VERTICAL") {
          if (value == "1") {
            DPU = 1;
            DPU_new = 1;
            DPD = 0;
            DPD_new = 0;
          } else if (value == "-1") {
            DPU = 0;
            DPU_new = 0;
            DPD = 1;
            DPD_new = 1;
          } else {
            DPU = 0;
            DPU_new = 0;
            DPD = 0;
            DPD_new = 0;
          }
        }
      }
    }
  }
}

std::int32_t VexboxController::is_connected(void) { return true; }

std::int32_t
VexboxController::get_digital(pros::controller_digital_e_t button) {
  switch (button) {
  case DIGITAL_A:
    return A;
  case DIGITAL_B:
    return B;
  case DIGITAL_X:
    return X;
  case DIGITAL_Y:
    return Y;
  case DIGITAL_L1:
    return LB;
  case DIGITAL_R1:
    return RB;
  case DIGITAL_L2:
    return leftTrigger;
  case DIGITAL_R2:
    return rightTrigger;
  case DIGITAL_LEFT:
    return DPL;
  case DIGITAL_RIGHT:
    return DPR;
  case DIGITAL_DOWN:
    return DPD;
  case DIGITAL_UP:
    return DPU;
  default:
    return 0;
  }
}

std::int32_t VexboxController::get_analog(pros::controller_analog_e_t channel) {
  switch (channel) {
  case ANALOG_LEFT_X:
    return leftHorizontal;
  case ANALOG_LEFT_Y:
    return leftVertical;
  case ANALOG_RIGHT_X:
    return rightHorizontal;
  case ANALOG_RIGHT_Y:
    return rightVertical;
  default:
    return 0;
  }
}

std::int32_t
VexboxController::get_digital_new_press(pros::controller_digital_e_t button) {
  int temp = 0;
  switch (button) {
  case DIGITAL_A:
    temp = A_new;
    A_new = 0;
    break;
  case DIGITAL_B:
    temp = B_new;
    B_new = 0;
    break;
  case DIGITAL_X:
    temp = X_new;
    X_new = 0;
    break;
  case DIGITAL_Y:
    temp = Y_new;
    Y_new = 0;
    break;
  case DIGITAL_L1:
    temp = LB_new;
    LB_new = 0;
    break;
  case DIGITAL_R1:
    temp = RB_new;
    RB_new = 0;
    break;
  case DIGITAL_LEFT:
    temp = DPL_new;
    DPL_new = 0;
    break;
  case DIGITAL_RIGHT:
    temp = DPR_new;
    DPR_new = 0;
    break;
  case DIGITAL_DOWN:
    temp = DPD_new;
    DPD_new = 0;
    break;
  case DIGITAL_UP:
    temp = DPU_new;
    DPU_new = 0;
    break;
  default:
    break;
  }
  return temp;
}

std::int32_t VexboxController::get_battery_capacity(void) { return 0; }
std::int32_t VexboxController::get_battery_level(void) { return 0; }

template <typename... Parameters>
std::int32_t VexboxController::print(int line, int col, const char *fmt,
                                     Parameters... parameters) {
  return 1;
}

std::int32_t VexboxController::set_text(int line, int col, const char *str) {
  return 1;
}
std::int32_t VexboxController::set_text(int line, int col,
                                        const std::string &str) {
  return 1;
}
std::int32_t VexboxController::clear_line(std::uint8_t line) { return 1; }
std::int32_t VexboxController::clear(void) { return 1; }
std::int32_t VexboxController::rumble(const char *rumble_pattern) { return 1; }

#endif