//
// Created by samuel on 29/08/18.
//

#ifndef PHYVR_LEVEL0_H
#define PHYVR_LEVEL0_H

#include "../level.h"

/**
 * Niveau 0 (training)
 * idée niveau :
 * 1) se rendre à un point précis
 * 2) tirer sur des cibles
 * 	  les cibles sont montées sur charnière et basculent
 */
class Level0 : public Level {

public:
	void init(bool isVR, AAssetManager *mgr, btDynamicsWorld *world) override;

	vector<Controls *> getControls() override;

	Camera *getCamera() override;

	vector<Shooter *> getShooters() override;

	vector<Base *> getEntities() override;

	vector<Drawable *> getDrawables() override;

	Limits *getLimits() override;

	bool won() override;

	bool lose() override;

	~Level0() override;

};


#endif //PHYVR_LEVEL0_H
