//
// Created by samuel on 10/09/18.
//

#ifndef PHYVR_CANON_H
#define PHYVR_CANON_H

#include <glm/glm.hpp>
#include "turret.h"

const float canonMass = 10.f;
const float canonOffset = 0.1f;
const glm::vec3 canonScale(0.1f, 0.1f, 0.8f);
const btVector3 canonRelPos(0.f, 0.f, turretScale.z + canonScale.z - canonOffset);

class Canon : public Controls, public Camera, public Shooter, public Poly {
private:
	// Controls
	float angle;

	bool respawn;

	bool hasClickedShoot;

	float added;

	bool isHit;

	// Canon
	btVector3 pos;

	btHingeConstraint *hinge;

	Base *turret;

	// Shooter
	DiffuseModel *missile;

	const int maxFramesFire;

	int fireCounter;

	// Camera
	glm::mat4 getCamRot();

	Base *generateMissile();

public:
	Canon(AAssetManager *mgr, btDynamicsWorld *world, Base *turret, btVector3 turretPos);

	void onInput(input in) override;

	output getOutput() override;

	void update() override;

	void decreaseLife(int toSub) override;

	vector<Base *> fire() override;

	glm::vec3 camPos(bool VR) override;

	glm::vec3 camLookAtVec(bool VR) override;

	glm::vec3 camUpVec(bool VR) override;


	btRigidBody *getMissileCopy();

	virtual ~Canon();
};


#endif //PHYVR_CANON_H
