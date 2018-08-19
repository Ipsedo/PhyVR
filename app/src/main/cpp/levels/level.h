//
// Created by samuel on 19/08/18.
//

#ifndef PHYVR_LEVEL_H
#define PHYVR_LEVEL_H

#include <android/asset_manager.h>
#include "../entity/base.h"
#include "../core/limits.h"
#include "../controls/controls.h"
#include "../graphics/camera.h"
#include "../entity/shooter.h"

class Level {
protected:
	vector<Base *> entities;

public:
	virtual void init(bool isVR, AAssetManager *mgr, btDynamicsWorld *world) = 0;

	virtual vector<Controls *> getControls() = 0;

	virtual Camera *getCamera() = 0;

	virtual vector<Shooter *> getShooters() = 0;

	virtual vector<Base *> getEntities() = 0;

	virtual vector<Drawable *> getDrawables() = 0;

	virtual Limits *getLimits() = 0;

	void addBases(vector<Base *> bs);

	virtual bool won() = 0;

	virtual bool lose() = 0;

	template<typename FunDeleteBase>
	void deleteBase(FunDeleteBase fn) {
		entities.erase(remove_if(entities.begin(), entities.end(), fn), entities.end());
	}

	virtual ~Level();
};

#endif //PHYVR_LEVEL_H