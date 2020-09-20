//
// Created by samuel on 20/09/20.
//

#ifndef PHYVR_NDK_PHYSICS_H
#define PHYVR_NDK_PHYSICS_H

#include <vector>

#include "entity.h"
#include "simple.h"

namespace phyvr_core {

    struct engine {
        btBroadphaseInterface *broad_phase__;
        btCollisionDispatcher *dispatcher__;
        btDefaultCollisionConfiguration *collision_configuration__;
        btSequentialImpulseConstraintSolver *constraint_solver__;

        btDiscreteDynamicsWorld *world;

        std::vector<std::shared_ptr<phyvr_core::Entity>> entities__;

        engine();
        ~engine();

        void add_entity(const std::shared_ptr<phyvr_core::Entity> &entity);
        void __del_entity(const std::shared_ptr<phyvr_core::Entity> &entity);
        void clear_entities();

        void update();
    };

}

#endif //PHYVR_NDK_PHYSICS_H
