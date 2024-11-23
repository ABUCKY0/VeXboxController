#include "main.h"
#include "VexboxController/VexboxController.hpp"
#include "pros/apix.h"
#include "pros/misc.h"
#include "pros/screen.h"
#include <cstdio>
#include <fstream>
#include <string>

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

  // disable COBS
  pros::c::serctl(SERCTL_DISABLE_COBS, NULL);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is boolended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol() {
  VexboxController controller(pros::E_CONTROLLER_MASTER);
  while (true) {
    // std::cout << controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) <<
    // std::endl;
    // left_mtr.move(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));

    pros::screen::print(pros::E_TEXT_SMALL, 0, "LB: %d   RB: %d                            ",
                        controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2),
                        controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2));
    pros::screen::print(pros::E_TEXT_SMALL, 1, "LB: %d   RB: %d                              ",
                        controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1),
                        controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1));
    pros::screen::print(
        pros::E_TEXT_SMALL, 3, "LX: %D   LY: %D                                  ",
        controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X),
        controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
    pros::screen::print(
        pros::E_TEXT_SMALL, 4, "RX: %d   RY: %d                               ",
        controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X),
        controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));
    pros::screen::print(pros::E_TEXT_SMALL, 6, "A: %d   B: %d                                       ",
                        controller.get_digital(pros::E_CONTROLLER_DIGITAL_A),
                        controller.get_digital(pros::E_CONTROLLER_DIGITAL_B));
    pros::screen::print(pros::E_TEXT_SMALL, 7, "X: %d   Y: %d                                 ",
                        controller.get_digital(pros::E_CONTROLLER_DIGITAL_X),
                        controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y));

    pros::screen::print(
        pros::E_TEXT_SMALL, 9, "DPU: %d   DPD: %d                                 ",
        controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP),
        controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN));
    pros::screen::print(
        pros::E_TEXT_SMALL, 10, "DPL: %d   DPR: %d                                          ",
        controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT),
        controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT));

    std::cout << "LX: " << controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X)
							<< " LY: " << controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)
							<< " RX: " << controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)
							<< " RY: " << controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)
							<< std::endl;

    pros::delay(20); 
  }
}