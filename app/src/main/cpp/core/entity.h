//
// Created by samuel on 08/09/20.
//

#ifndef PHYVR_NDK_ENTITY_H
#define PHYVR_NDK_ENTITY_H

#include <btBulletDynamicsCommon.h>
#include <functional>
#include "../view/graphics.h"

namespace phyvr_core {

    btRigidBody::btRigidBodyConstructionInfo
    create_rigidbody_infos(btCollisionShape *shape, const btTransform &start_tr, btScalar mass);

    btTransform get_start_transform(glm::vec3 pos, glm::mat4 rot_mat);

    btConvexHullShape *parse_obj(const std::string &obj_file_content);

    class Entity : public btRigidBody {
    public:

        Entity(const std::function<btCollisionShape *()> &make_shape_fun, glm::vec3 pos,
               glm::mat4 rot_mat, glm::vec3 scale, float mass);

        virtual glm::mat4 get_model_matrix() final;

        virtual void update() = 0;

        virtual bool is_dead();

        virtual bool explosion();

        virtual void on_contact_finish(Entity *other) = 0;

        virtual void damage(int to_sub) = 0;

        virtual ~Entity();

    protected:
        glm::vec3 scale;
    };
}

#endif //PHYVR_NDK_ENTITY_H
