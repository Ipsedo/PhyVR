//
// Created by samuel on 11/06/18.
//

#ifndef PHYVR_LEVEL0_H
#define PHYVR_LEVEL0_H

#include "../../graphics/drawable/cubemap.h"
#include "../level.h"
#include "../../entity/vehicles/tank/tank.h"

class LevelDemo : public Level {
public:
	LevelDemo();

	void init(bool isVR, AAssetManager *mgr, btDynamicsWorld *world) override;

	vector<Controls *> getControls() override;

	vector<Shooter *> getShooters() override;

	Camera *getCamera() override;

	vector<Base *> getEntities() override;

	vector<Drawable *> getDrawables() override;

	Limits getLimits() override;

	bool won() override;

	bool lose() override;

	void step() override ;

private:
	CubeMap *map;
	Tank *tank;
	bool isInit;
};


#endif //PHYVR_LEVEL0_H
