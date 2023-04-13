//
// Created by samuel on 19/03/2023.
//

#ifndef PHYVR_CONTROLLER_H
#define PHYVR_CONTROLLER_H

#include <android/input.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../view/drawable/hud.h"
#include "./inputs.h"
#include "./joystick.h"

class Controller {
public:
  virtual void on_input(const user_input &input) = 0;
};

class ControllerEngine {
public:
  ControllerEngine(AConfiguration *config, int width, int height);

  void add_controller(const std::shared_ptr<Controller> &controller);

  int32_t on_event(AInputEvent *event);

  std::vector<std::unique_ptr<HUDDrawable>>
  get_hud_drawables(AAssetManager *mgr);

private:
  std::vector<std::shared_ptr<Controller>> controllers;

  std::shared_ptr<HUDJoyStick> drive_joystick;
  std::shared_ptr<ScreenJoyStick> turret_joystick;
};

#endif // PHYVR_CONTROLLER_H
