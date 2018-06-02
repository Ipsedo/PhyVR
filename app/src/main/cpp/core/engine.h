//
// Created by samuel on 26/05/18.
//

#ifndef PHYVR_LEVEL_H
#define PHYVR_LEVEL_H

#include "../graphics/renderer.h"
#include "../entity/base.h"
#include "../entity/shooter.h"

class Engine {
public:
	Engine(vector<Base *> *b);

	void update(float delta);

	void addShooter(Shooter* s);

	~Engine();

	btDiscreteDynamicsWorld *world;

private:
	vector<Shooter*> shooters;
	vector<Base *> *bases;
	btBroadphaseInterface *broadPhase;
	btCollisionDispatcher *dispatcher;
	btDefaultCollisionConfiguration *collisionConfiguration;
	btSequentialImpulseConstraintSolver *constraintSolver;
};


#endif //PHYVR_LEVEL_H