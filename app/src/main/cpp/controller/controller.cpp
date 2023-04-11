//
// Created by samuel on 19/03/2023.
//

#include "./controller.h"
#include "../utils/logging.h"

#include <android/input.h>
#include <android_native_app_glue.h>

ControllerEngine::ControllerEngine(AConfiguration *config, int width,
                                   int height)
    : controllers() {
  left_joystick =
      std::make_shared<HUDJoyStick>(config, width, height, 40, true, 150, 40);
  right_joystick =
      std::make_shared<HUDJoyStick>(config, width, height, 40, false, 150, 40);
}

void ControllerEngine::add_controller(
    const std::shared_ptr<Controller> &controller) {
  controllers.push_back(controller);
}

int32_t ControllerEngine::on_event(AInputEvent *event) {
  left_joystick->on_event(event);
  right_joystick->on_event(event);

  user_input input{left_joystick->get_input(),
                   right_joystick->get_input(),
                   {false},
                   {false},
                   {false}};

  for (auto &ctrl : controllers)
    ctrl->on_input(input);

  return 1;
}

std::vector<std::unique_ptr<HUDDrawable>>
ControllerEngine::get_hud_drawables(AAssetManager *mgr) {
  std::vector<std::unique_ptr<HUDDrawable>> result{};

  auto left_joystick_drawable = left_joystick->get_hud_drawable(mgr);
  result.push_back(std::move(left_joystick_drawable));

  auto right_joystick_drawable = right_joystick->get_hud_drawable(mgr);
  result.push_back(std::move(right_joystick_drawable));

  return result;
}
