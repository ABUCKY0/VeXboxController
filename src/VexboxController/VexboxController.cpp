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
      std::string id = input.substr(input.find("_") + 1, input.find("$$") - input.find("_") - 1);
      std::string value = input.substr(input.find("$$") + 2);
      if (type == "axis") {
        if (id == "0") {
          double lefthraw = std::stod(value); // -1 to 1
          
          // Convert to -127 to 127
          leftHorizontal = std::clamp((lefthraw * 127.0), -127.0, 127.0);
        }
        else if (id == "1") {
          double leftvraw = std::stod(value); // -1 to 1
          // Convert to -127 to 127
          leftVertical = std::clamp((leftvraw * 127.0), -127.0, 127.0);
        }
        else if (id == "2") {
          double righraw = std::stod(value); // -1 to 1
          // Convert to -127 to 127
          rightHorizontal = std::clamp((righraw * 127.0), -127.0, 127.0);
        }
        else if (id == "3") {
          double rightvraw = std::stod(value); // -1 to 1
          // Convert to -127 to 127
          rightVertical = std::clamp((rightvraw * 127.0), -127.0, 127.0);
          
        }
        else if (id == "4") {
          leftTrigger = std::stod(value) > 0.0 ? 1 : 0;

        }
        else if (id == "5")  {
          rightTrigger = std::stod(value) > 0.0 ? 1 : 0;
        }
      } else if (type == "button") {
        int pressed = std::stoi(value);
        if (id == "0") {
          A = pressed;
          if (A)
            A_new = 1;
        } else if (id == "1") {
          B = pressed;
          if (B)
            B_new = 1;
        } else if (id == "2") {
          X = pressed;
          if (X)
            X_new = 1;
        } else if (id == "3") {
          Y = pressed;
          if (Y)
            Y_new = 1;
        } else if (id == "4") {
          LB = pressed;
          if (LB)
            LB_new = 1;
        } else if (id == "5") {
          RB = pressed;
          if (RB)
            RB_new = 1;
        } else if (id == "6") {
          VIEW = pressed;
          if (VIEW)
            VIEW_new = 1;
        } else if (id == "7") {
          MENU = pressed;
          if (MENU)
            MENU_new = 1;
        } else if (id == "8") {
          LSB = pressed;
          if (LSB)
            LSB_new = 1;
        } else if (id == "9") {
          RSB = pressed;
          if (RSB)
            RSB_new = 1;
        }
      } else if (type == "dpad") {
        std::string hdapd = value.substr(1, value.find(",") - 1);
        std::string vdpad = value.substr(value.find(",") + 2, value.size() - 5);
        if (hdapd == "1") {
          DPL = 1;
          if (DPL)
            DPL_new = 1;
        } 
        if (hdapd == "-1") {
          DPR = 1;
          if (DPR)
            DPR_new = 1;
        }
         if (hdapd == "0") {
          DPL = 0;
          DPR = 0;
          DPL_new = 0;
          DPR_new = 0;
        }

        if (vdpad == "1") {
          DPU = 1;
          if (DPU)
            DPU_new = 1;
        }
        if (vdpad == "-1") {
          DPD = 1;
          if (DPD)
            DPD_new = 1;
        }
        if (vdpad == "0") {
          DPU = 0;
          DPD = 0;
          DPU_new = 0;
          DPD_new = 0;
        }
      
      }
    }

  }
}

std::int32_t VexboxController::is_connected(void) { return true; }

std::int32_t VexboxController::get_digital(pros::controller_digital_e_t button) {
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
    temp = DPD_new;
    DPD_new = 0;
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