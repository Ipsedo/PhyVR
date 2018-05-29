//
// Created by samuel on 26/05/18.
//

#include "level.h"

Level::Level(vector<Base*>* bases) {

    this->bases = bases;

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    broadPhase = new btDbvtBroadphase();
    constraintSolver = new btSequentialImpulseConstraintSolver();

    world = new btDiscreteDynamicsWorld(dispatcher,
                                        broadPhase,
                                        constraintSolver,
                                        collisionConfiguration);
    world->setGravity(btVector3(0,-10,0));

    for (Base* b : *this->bases)
        for (btRigidBody* bd : b->rigidBody)
            world->addRigidBody(bd);
}

void Level::update(float delta) {
    world->stepSimulation(delta);
}

void Level::addNewBox(Base *b) {
    for (btRigidBody* bd : b->rigidBody)
        world->addRigidBody(bd);
}

Level::~Level() {
    // From car bullet example
    for (int i = world->getNumCollisionObjects() - 1; i >= 0 ; i--) {
        btCollisionObject* obj = world->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()) {

            while (body->getNumConstraintRefs()) {
                btTypedConstraint* constraint = body->getConstraintRef(0);
                world->removeConstraint(constraint);
                delete constraint;
            }
            //delete body->getMotionState(); base delete motionstate
            world->removeRigidBody(body);
        } else {
            world->removeCollisionObject( obj );
        }
        // delete obj; base delete body object (but btCollisionObject or btRigidBody ?)
    }

    delete world;
}
