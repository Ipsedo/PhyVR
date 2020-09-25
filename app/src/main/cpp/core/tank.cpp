//
// Created by samuel on 25/09/20.
//

#include "tank.h"

#include "../utils.h"

/*
 * Wheel
 */

phyvr_core::Wheel::Wheel(AAssetManager *mgr, btDynamicsWorld *world,
                         std::shared_ptr<phyvr_core::Chassis> *chassis, glm::vec3 chassis_pos,
                         glm::vec3 pos) :
        Entity([mgr]() {
                   glm::vec3 scale(wheel_width, wheel_radius, wheel_radius);
                   std::string objTxt = phyvr_utils::get_file_content(mgr, "obj/wheel.obj");
                   btCollisionShape *shape = phyvr_core::parse_obj(objTxt);
                   shape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
                   return shape;
               }, pos + chassis_pos, glm::mat4(1.f),
               glm::vec3(wheel_width, wheel_radius, wheel_radius),
               wheel_mass) {

}

void phyvr_core::Wheel::update() {

}

void phyvr_core::Wheel::on_contact_finish(phyvr_core::Entity *other) {

}

void phyvr_core::Wheel::damage(int to_sub) {

}

/*
 *
 */

std::shared_ptr<phyvr_view::Camera> phyvr_core::Chassis::get_cam() {
    return std::shared_ptr<phyvr_view::Camera>();
}

/*
 *
 */

std::shared_ptr<phyvr_view::Camera> phyvr_core::Canon::get_cam() {
    return std::shared_ptr<phyvr_view::Camera>();
}
