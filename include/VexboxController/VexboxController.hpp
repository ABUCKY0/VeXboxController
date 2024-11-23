#ifndef XBOX_CONTROLLER_HPP
#define XBOX_CONTROLLER_HPP

#include "main.h"
#include <atomic>
#include <cstdint>
#include <string>

class VexboxController : public pros::Controller {
public:
  explicit VexboxController(pros::controller_id_e_t id);

  std::int32_t is_connected(void);
  std::int32_t get_digital(pros::controller_digital_e_t button);
  std::int32_t get_analog(pros::controller_analog_e_t channel);
  std::int32_t get_digital_new_press(pros::controller_digital_e_t button);
  std::int32_t get_battery_capacity(void);
  std::int32_t get_battery_level(void);
  template <typename... Parameters>
  std::int32_t print(int line, int col, const char *fmt,
                     Parameters... parameters);
  std::int32_t set_text(int line, int col, const char *str);
  std::int32_t set_text(int line, int col, const std::string &str);
  std::int32_t clear_line(std::uint8_t line);
  std::int32_t clear(void);
  std::int32_t rumble(const char *rumble_pattern);

private:
  void update_controller();

  std::atomic<double> leftHorizontal = 0;
  std::atomic<double> leftVertical = 0;
  std::atomic<double> rightHorizontal = 0;
  std::atomic<double> rightVertical = 0;
  std::atomic<bool> leftTrigger = 0;
  std::atomic<bool> rightTrigger = 0;
  std::atomic<bool> A = 0;
  std::atomic<bool> B = 0;
  std::atomic<bool> X = 0;
  std::atomic<bool> Y = 0;
  std::atomic<bool> LB = 0;
  std::atomic<bool> RB = 0;
  std::atomic<bool> VIEW = 0;
  std::atomic<bool> MENU = 0;
  std::atomic<bool> LSB = 0;
  std::atomic<bool> RSB = 0;
  std::atomic<bool> DPU = 0;
  std::atomic<bool> DPD = 0;
  std::atomic<bool> DPL = 0;
  std::atomic<bool> DPR = 0;

  std::atomic<bool> A_new = 0;
  std::atomic<bool> B_new = 0;
  std::atomic<bool> X_new = 0;
  std::atomic<bool> Y_new = 0;
  std::atomic<bool> DPU_new = 0;
  std::atomic<bool> DPD_new = 0;
  std::atomic<bool> DPL_new = 0;
  std::atomic<bool> DPR_new = 0;
  std::atomic<bool> LB_new = 0;
  std::atomic<bool> RB_new = 0;
  std::atomic<bool> VIEW_new = 0;
  std::atomic<bool> MENU_new = 0;
  std::atomic<bool> LSB_new = 0;
  std::atomic<bool> RSB_new = 0;

  pros::Task controllerTask;
};

#endif // XBOX_CONTROLLER_HPP
