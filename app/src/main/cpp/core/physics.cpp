//
// Created by samuel on 20/09/20.
//

#include "physics.h"

phyvr_core::engine::engine() {
    collision_configuration__ = new btDefaultCollisionConfiguration();
    dispatcher__ = new btCollisionDispatcher(collision_configuration__);
    broad_phase__ = new btDbvtBroadphase();
    constraint_solver__ = new btSequentialImpulseConstraintSolver();

    world = new btDiscreteDynamicsWorld(
            dispatcher__,
            broad_phase__,
            constraint_solver__,
            collision_configuration__);
    world->setGravity(btVector3(0, -9.8f, 0));

    entities__ = std::vector<std::shared_ptr<phyvr_core::Entity>>();
}

void phyvr_core::engine::add_entity(
        const std::shared_ptr<phyvr_core::Entity> &entity) {

    world->addRigidBody(entity.get());
    entities__.push_back(entity);
}

void phyvr_core::engine::__del_entity(
        const std::shared_ptr<phyvr_core::Entity> &entity) {

    if (entity && entity->getMotionState()) {
        while (entity->getNumConstraintRefs()) {
            btTypedConstraint *constraint = entity->getConstraintRef(0);
            world->removeConstraint(constraint);
            delete constraint;
        }
        delete entity->getMotionState();
        delete entity->getCollisionShape();


        world->removeRigidBody(entity.get());
    }
}

void phyvr_core::engine::update() {
    float time_delta = 1.f / 60.f;

    world->stepSimulation(time_delta);

    std::for_each(entities__.begin(), entities__.end(),
                  [](const std::shared_ptr<phyvr_core::Entity> &e) { e->update(); });

    auto erased = std::remove_if(entities__.begin(), entities__.end(),
                                 [](const std::shared_ptr<phyvr_core::Entity> &e) {
                return e->is_dead();
    });

   /*for (auto &e : entities__) {
        if (e->is_dead()) {
            if (e->explosion()) {
                // TODO explosion
            }
            __del_entity(e);
        }
    }*/

    //entities__.erase(erased);
}

void phyvr_core::engine::clear_entities() {
    for (auto &e : entities__) __del_entity(e);

    entities__.clear();
}

phyvr_core::engine::~engine() {

    for (auto &e : entities__) __del_entity(e);

    entities__.clear();
}
