//
// Created by samuel on 26/06/18.
//

#include "wheel.h"
#include "../../utils/assets.h"
#include "../../utils/rigidbody.h"
#include "../../utils/vec.h"
#include <glm/glm.hpp>

Wheel::Wheel(AAssetManager *mgr, btDynamicsWorld *world, Base *chassis, btVector3 chassisPos,
			 btVector3 pos)
		: Poly([mgr](glm::vec3 scale) {
				   string objTxt = getFileText(mgr, "obj/wheel.obj");
				   btCollisionShape *shape = parseObj(objTxt);
				   shape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
				   return shape;
			   }, makeWheelMesh(mgr), btVector3ToVec3(pos + chassisPos),
			   glm::vec3(wheelWidth, wheelRadius, wheelRadius),
			   glm::mat4(1.0f), wheelMass, true),
		  pos(pos), chassisPos(chassisPos), isMotorEnabled(true),
		  isBraking(true), freeWheel(false), targetSpeed(0.f), hasReAccelerate(false),
		  respawn(false) {
	setFriction(500.f);

	btTransform trA, trB;
	trA.setIdentity();
	trA.setOrigin(pos);
	trB.setIdentity();
	trB.setOrigin(btVector3(0, 0, 0));
	hinge = new btGeneric6DofSpring2Constraint(*chassis, *this, trA, trB, RO_XYZ);

	// Angular limits
	hinge->setAngularLowerLimit(btVector3(1, 0, 0));
	hinge->setAngularUpperLimit(btVector3(-1, 0, 0));

	// Linear limits
	hinge->setLinearLowerLimit(btVector3(0, -0.4f, 0));
	hinge->setLinearUpperLimit(btVector3(0, 0, 0));

	{
		int motorAxis = 3;
		hinge->enableMotor(motorAxis, isMotorEnabled);
		hinge->setMaxMotorForce(motorAxis, 1e4f);
		hinge->setTargetVelocity(motorAxis, targetSpeed);
	}

	{
		int index = 1;
		hinge->enableSpring(index, true);
		hinge->setDamping(index, 30.f, true);
		hinge->setStiffness(index, 100.f, true);
		hinge->setBounce(index, 1e-2f);
		hinge->setEquilibriumPoint(index, -0.2f);
	}

	world->addConstraint(hinge, true);

	/*int axis[4] {0,2,4,5};
	for (int i : axis) {
		hinge->setLimit(i, 1, 0);
		hinge->enableSpring(i, false);
	}*/

	//hinge->setDbgDrawSize(btScalar(5.f));
}


void Wheel::onInput(input in) {
	isMotorEnabled = abs(in.speed) > 1e-2f;
	targetSpeed = in.speed;
	respawn = in.respawn;
	if (isMotorEnabled) hasReAccelerate = true;
	if (hasReAccelerate) isBraking = in.brake;
	if (isBraking) hasReAccelerate = false;
}

void Wheel::update() {
	Base::update();

	if (isBraking) {
		isMotorEnabled = true;
		targetSpeed = 0.f;
	}

	int motorAxis = 3;
	hinge->enableMotor(motorAxis, isMotorEnabled);
	hinge->setTargetVelocity(motorAxis, -targetSpeed * 24.f);

	if (respawn) {
		btTransform tr;
		tr.setIdentity();
		tr.setOrigin(pos + chassisPos);

		clearForces();
		setLinearVelocity(btVector3(0, 0, 0));
		setAngularVelocity(btVector3(0, 0, 0));

		getMotionState()->setWorldTransform(tr);
		setWorldTransform(tr);

		respawn = false;
	}
}


/**
 * Front wheels
 * Direction en plus pour Controls
 *
 *
 */

FrontWheel::FrontWheel(AAssetManager *mgr, btDynamicsWorld *world, Base *chassis,
					   btVector3 chassisPos, btVector3 pos)
		: Wheel(mgr, world, chassis, chassisPos, pos), direction(0.f) {}

void FrontWheel::onInput(input in) {
	Wheel::onInput(in);
	direction = in.xAxis;
}

void FrontWheel::update() {
	Wheel::update();

	int motorAxis = 4;
	float angle = float(M_PI) * direction / 10.f;
	hinge->setLimit(motorAxis, angle, angle);
}

ModelVBO *Wheel::makeWheelMesh(AAssetManager *mgr) {
	std::string cylObjText = getFileText(mgr, "obj/wheel.obj");
	return new ModelVBO(cylObjText, wheelColor[0], wheelColor[1], wheelColor[2], wheelColor[3]);
}

output Wheel::getOutput() {
	return {false};
}
