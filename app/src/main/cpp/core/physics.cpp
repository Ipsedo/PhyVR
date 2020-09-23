//
// Created by samuel on 20/09/20.
//

#include <android/log.h>
#include "physics.h"


bool phyvr_core::callback_finish(void *user_persistent_data) {
    auto *t = (std::tuple<phyvr_core::Entity *, phyvr_core::Entity *> *) user_persistent_data;

    phyvr_core::Entity *b0 = std::get<0>(*t);
    phyvr_core::Entity *b1 = std::get<1>(*t);
    b0->on_contact_finish(b1);
    b1->on_contact_finish(b0);

    b0->damage(1);
    b1->damage(1);

    delete t;

    return true;
}

bool phyvr_core::callback_processed(btManifoldPoint &cp, void *body0, void *body1) {
    auto b0 = (phyvr_core::Entity *) body0;
    auto b1 = (phyvr_core::Entity *) body1;

    cp.m_userPersistentData = new std::tuple<phyvr_core::Entity *, phyvr_core::Entity *>(b0, b1);

    return true;
}


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

    gContactDestroyedCallback = callback_finish;
    gContactProcessedCallback = callback_processed;

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

    auto for_each_fun = [this](const std::shared_ptr<phyvr_core::Entity> &e) {
        e->update();
        if (e->is_dead()) {
            __del_entity(e);
            if (e->explosion()) {
                // TODO explosion
            }
        }
    };

    // Update, delete and explosion
    std::for_each(entities__.begin(), entities__.end(), for_each_fun);

    auto rm_if = std::remove_if(
            entities__.begin(), entities__.end(),
            [](const std::shared_ptr<phyvr_core::Entity> &e) { return e->is_dead(); });

    // Remove engine entities
    entities__.erase(rm_if, entities__.end());
}

void phyvr_core::engine::clear_entities() {
    std::for_each(entities__.begin(), entities__.end(),
                  [this](const std::shared_ptr<phyvr_core::Entity> &e) { __del_entity(e); });

    entities__.clear();
}

phyvr_core::engine::~engine() {
    clear_entities();

    delete world;
    delete broad_phase__;
    delete dispatcher__;
    delete collision_configuration__;
    delete constraint_solver__;
}
