//
// Created by samuel on 19/03/2023.
//

#include "controller.h"

void
ControllerEngine::add_controller(const std::string &name, const std::shared_ptr<Controller> &controller) {
    controllers.insert({name, controller});
}

void ControllerEngine::remove_controller(const std::string &name) {
    controllers.erase(name);
}

void ControllerEngine::on_event(AInputEvent *event) {

}