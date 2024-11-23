#ifndef XBOX_CONTROLLER_HPP
#define XBOX_CONTROLLER_HPP

#include "main.h"
#include <atomic>
#include <cstdint>
#include <string>

class XboxController : public pros::Controller {
public:
  explicit XboxController(pros::controller_id_e_t id);

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
  std::atomic<int> leftTrigger = 0;
  std::atomic<int> rightTrigger = 0;
  std::atomic<int> A = 0;
  std::atomic<int> B = 0;
  std::atomic<int> X = 0;
  std::atomic<int> Y = 0;
  std::atomic<int> LB = 0;
  std::atomic<int> RB = 0;
  std::atomic<int> VIEW = 0;
  std::atomic<int> MENU = 0;
  std::atomic<int> LSB = 0;
  std::atomic<int> RSB = 0;
  std::atomic<int> HDAPD = 0;
  std::atomic<int> VDPAD = 0;

  std::atomic<int> A_new = 0;
  std::atomic<int> B_new = 0;
  std::atomic<int> X_new = 0;
  std::atomic<int> Y_new = 0;
  std::atomic<int> DPU_new = 0;
  std::atomic<int> DPD_new = 0;
  std::atomic<int> DPL_new = 0;
  std::atomic<int> DPR_new = 0;
  std::atomic<int> LB_new = 0;
  std::atomic<int> RB_new = 0;
  std::atomic<int> VIEW_new = 0;
  std::atomic<int> MENU_new = 0;
  std::atomic<int> LSB_new = 0;
  std::atomic<int> RSB_new = 0;

  pros::Task controllerTask;
};

#endif // XBOX_CONTROLLER_HPP
