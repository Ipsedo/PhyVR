//
// Created by samuel on 30/05/18.
//

#ifndef PHYVR_TANK_H
#define PHYVR_TANK_H

#include <glm/glm.hpp>
#include <android/asset_manager.h>
#include "../../graphics/camera.h"
#include "../base.h"
#include "../../controls/controls.h"
#include "../shooter.h"

class Tank : public Base, public Camera, public Controls, public Shooter {
public:
	Tank(glm::vec3 pos, btDynamicsWorld *world, AAssetManager *mgr, vector<Base*>* bases);

	void onInput(input in) override;

	void draw(glm::mat4 pMatrix, glm::mat4 vMatrix, glm::vec3 lighPos) override;

	glm::vec3 camPos() override;
	glm::vec3 camLookAtVec() override;
	glm::vec3 camUpVec() override;

	void fire(vector<Base*>* bases) override;

	~Tank();

private:
	btDynamicsWorld* world;

	ModelVBO* missile;

	vector<Base*>* bases;

	glm::vec3 chassisScale;
	glm::vec3 wheelScale;
	glm::vec3 turretScale;
	glm::vec3 canonScale;

	float direction;
	float speed;
	bool hasClickedShoot;

	glm::vec3 turretPos;
	glm::vec3 canonPos;
	float turretDir;
	float turretUp;

	int nbWheel;
	btVector3* wheelPos;

	glm::vec3 spawnPos;

	void init(btDynamicsWorld *world, AAssetManager *mgr);

	btHingeConstraint* turretHinge;
	btHingeConstraint* canonHinge;
	vector<btHinge2Constraint *> wheelHinge2;
	vector<ModelVBO *> modelVBOs;

	void makeChassis(AAssetManager* mgr);
	void makeWheels(AAssetManager* mgr, btDynamicsWorld* world);
	void makeTurret(AAssetManager* mgr, btDynamicsWorld* world);
	void makeCanon(AAssetManager* mgr, btDynamicsWorld* world);

	void respawn();
};


#endif //PHYVR_TANK_H
