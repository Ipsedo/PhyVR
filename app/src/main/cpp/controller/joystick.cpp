//
// Created by samuel on 20/03/2023.
//

#include "./joystick.h"

#include <android/input.h>

#include "../utils/logging.h"
#include "../utils/units.h"

HUDJoyStick::HUDJoyStick(AConfiguration *config, int width_px, int height_px,
                         int margin_dp, bool left, int size_dp,
                         int stick_size_dp)
    : pointer_id(-1), touched(false), size(dp_to_px(config, size_dp)),
      stick_size(dp_to_px(config, stick_size_dp)),
      center_pos_x(dp_to_px(config, margin_dp) + size / 2.f),
      center_pos_y(center_pos_x), stick_x(center_pos_x), stick_y(stick_x),
      pointer_rel_x(stick_x), pointer_rel_y(stick_y), x_value(0.f),
      y_value(0.f), width(width_px), height(height_px) {
  if (!left) {
    center_pos_x = float(width_px) - center_pos_x;
    stick_x = float(width_px) - stick_x;
  }
}

bool HUDJoyStick::on_event(AInputEvent *event) {
  int action = AMotionEvent_getAction(event);
  int pointer_index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
                      AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
  int curr_pointer_id = AMotionEvent_getPointerId(event, pointer_index);

  float pt_x = AMotionEvent_getX(event, pointer_index);
  float pt_y = float(height) - AMotionEvent_getY(event, pointer_index);

  switch (action & AMOTION_EVENT_ACTION_MASK) {
  case AMOTION_EVENT_ACTION_POINTER_DOWN:
  case AMOTION_EVENT_ACTION_DOWN:
    if (!touched && is_inside_(pt_x, pt_y)) {
      pointer_id = curr_pointer_id;
      touched = true;

      pointer_rel_x = pt_x - stick_x;
      pointer_rel_y = pt_y - stick_y;

      stick_x = pt_x - pointer_rel_x;
      stick_y = pt_y - pointer_rel_y;

      return true;
    }
    break;
  case AMOTION_EVENT_ACTION_POINTER_UP:
  case AMOTION_EVENT_ACTION_UP:
    if (touched && pointer_id == curr_pointer_id) {
      touched = false;
      pointer_id = -1;

      stick_x = center_pos_x;
      stick_y = center_pos_y;

      return true;
    }
    break;
  case AMOTION_EVENT_ACTION_MOVE:
    for (int i = 0; i < AMotionEvent_getPointerCount(event); i++) {
      if (touched && pointer_id == AMotionEvent_getPointerId(event, i)) {
        pt_x = AMotionEvent_getX(event, i);
        pt_y = float(height) - AMotionEvent_getY(event, i);
        float rel_x = pt_x - pointer_rel_x - center_pos_x;
        float rel_y = pt_y - pointer_rel_y - center_pos_y;

        float max_size = (size - stick_size) / 2.f;

        rel_x = rel_x > max_size ? max_size : rel_x;
        rel_y = rel_y > max_size ? max_size : rel_y;

        rel_x = rel_x < -max_size ? -max_size : rel_x;
        rel_y = rel_y < -max_size ? -max_size : rel_y;

        stick_x = rel_x + center_pos_x;
        stick_y = rel_y + center_pos_y;

        x_value = rel_x / max_size;
        y_value = rel_y / max_size;

        return true;
      }
    }

  default:
    break;
  }
  return false;
}

bool HUDJoyStick::is_inside_(float x, float y) const {
  return (stick_x - stick_size / 2.f < x) && (x < stick_x + stick_size / 2.f) &&
         (stick_y - stick_size / 2.f < y) && (y < stick_y + stick_size / 2.f);
}

joystick HUDJoyStick::get_input() { return {x_value, y_value}; }

joystick HUDJoyStick::get_input_px() { return {stick_x, stick_y}; }

std::unique_ptr<HUDDrawable> HUDJoyStick::get_hud_drawable(AAssetManager *mgr) {
  return std::move(std::unique_ptr<HUDDrawable>(new JoyStickDrawable(
      mgr, [this] { return get_input_px(); }, {center_pos_x, center_pos_y},
      size, stick_size)));
}
