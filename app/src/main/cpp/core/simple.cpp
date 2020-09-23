//
// Created by samuel on 20/09/20.
//

#include "simple.h"


/*
 * Cube
 */

phyvr_core::Cube::Cube(glm::vec3 pos, glm::vec3 scale,
                       glm::mat4 rot_mat, float mass, int life) :
        Entity([scale]() {
            return new btBoxShape(btVector3(scale.x, scale.y, scale.z));
        }, pos, rot_mat, scale, mass),
        life(life) {

}

bool phyvr_core::Cube::is_dead() {
    return life <= 0;
}


bool phyvr_core::Cube::explosion() {
    return life <= 0;
}

void phyvr_core::Cube::damage(int to_sub) {
    life -= to_sub;
}

void phyvr_core::Cube::on_contact_finish(Entity *other) {

}

void phyvr_core::Cube::update() {

}


/*
 * Cone
 */

phyvr_core::Cone::Cone(
        glm::vec3 pos, glm::vec3 scale, glm::mat4 rot_mat, float mass, int life) :
        Entity([scale]() {
            btCollisionShape *shape = new btConeShape(1.f, 2.f);
            shape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
            return shape;
        }, pos, rot_mat, scale, mass),
        life(life) {

}

bool phyvr_core::Cone::is_dead() {
    return life <= 0;
}

bool phyvr_core::Cone::explosion() {
    return life <= 0;
}

void phyvr_core::Cone::on_contact_finish(Entity *other) {

}

void phyvr_core::Cone::damage(int to_sub) {
    life -= to_sub;
}

void phyvr_core::Cone::update() {

}
