//
// Created by samuel on 20/09/20.
//

#include "entity.h"

#include "../utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

btRigidBody::btRigidBodyConstructionInfo
phyvr_core::create_rigidbody_infos(btCollisionShape *shape, const btTransform &start_tr,
                                   btScalar mass) {
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

btConvexHullShape *phyvr_core::parse_obj(const std::string &obj_file_content) {
    std::vector<std::string> lines = phyvr_utils::split(obj_file_content, '\n');
    auto shape = new btConvexHullShape();
    std::vector<float> vertex_list;
    std::vector<int> vertex_draw_order;

    for (auto &str : lines) {
        std::vector<std::string> splitted_line = phyvr_utils::split(str, ' ');
        if (!splitted_line.empty()) {
            if (splitted_line[0] == "v") {
                vertex_list.push_back(std::stof(splitted_line[1]));
                vertex_list.push_back(std::stof(splitted_line[2]));
                vertex_list.push_back(std::stof(splitted_line[3]));
            } else if (splitted_line[0] == "f") {
                std::vector<std::string> v1 = phyvr_utils::split(splitted_line[1], '/');
                std::vector<std::string> v2 = phyvr_utils::split(splitted_line[2], '/');
                std::vector<std::string> v3 = phyvr_utils::split(splitted_line[3], '/');

                vertex_draw_order.push_back(std::stoi(v1[0]) - 1);
                vertex_draw_order.push_back(std::stoi(v2[0]) - 1);
                vertex_draw_order.push_back(std::stoi(v3[0]) - 1);

                v1.clear();
                v2.clear();
                v3.clear();
            }
        }
        splitted_line.clear();
    }

    unsigned long nbVertex = vertex_draw_order.size();
    for (int i = 0; i < nbVertex; i++) {
        btVector3 point(vertex_list[(vertex_draw_order[i] - 1) * 3],
                        vertex_list[(vertex_draw_order[i] - 1) * 3 + 1],
                        vertex_list[(vertex_draw_order[i] - 1) * 3 + 2]
        );
        shape->addPoint(point, true);
    }

    vertex_draw_order.clear();
    vertex_list.clear();
    lines.clear();

    return shape;
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
