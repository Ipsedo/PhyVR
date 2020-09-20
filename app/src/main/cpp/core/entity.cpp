//
// Created by samuel on 20/09/20.
//

#include "entity.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

btRigidBody::btRigidBodyConstructionInfo
phyvr_core::create_rigidbody_infos(btCollisionShape *shape, const btTransform &start_tr,
                                   btScalar mass) {

    btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 loacl_intertia(0, 0, 0);
    if (isDynamic)
        shape->calculateLocalInertia(mass, loacl_intertia);

    auto myMotionState = new btDefaultMotionState(start_tr);

    return {mass, myMotionState, shape, loacl_intertia};
}

btTransform phyvr_core::get_start_transform(glm::vec3 pos, glm::mat4 rot_mat) {
    btTransform start_tr;
    start_tr.setIdentity();
    start_tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
    glm::quat tmp = glm::quat_cast(rot_mat);
    start_tr.setRotation(btQuaternion(tmp.x, tmp.y, tmp.z, tmp.w));

    return start_tr;
}

/*
 * Entity
 */

phyvr_core::Entity::Entity(
        const std::function<btCollisionShape *()> &make_shape_fun,
        glm::vec3 pos, glm::mat4 rot_mat, glm::vec3 scale, float mass) :
        btRigidBody(phyvr_core::create_rigidbody_infos(
                make_shape_fun(),
                phyvr_core::get_start_transform(pos, rot_mat),
                mass)),
        scale(scale) {

}

glm::mat4 phyvr_core::Entity::get_model_matrix() {
    btScalar tmp[16];
    btTransform tr;
    getMotionState()->getWorldTransform(tr);
    tr.getOpenGLMatrix(tmp);
    glm::mat4 modelMatrix = glm::make_mat4(tmp) * glm::scale(glm::mat4(1.f), scale);

    return modelMatrix;
}


bool phyvr_core::Entity::is_dead() {
    return false;
}

bool phyvr_core::Entity::explosion() {
    return false;
}

phyvr_core::Entity::~Entity() {
    btRigidBody::~btRigidBody();
}
