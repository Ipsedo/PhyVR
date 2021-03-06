//
// Created by samuel on 02/06/18.
//

#ifndef PHYVR_PLAYER_H
#define PHYVR_PLAYER_H

#include "base.h"
#include "../controls.h"
#include "ammu/shooter.h"
#include "../graphics/camera.h"

class Player {
public:
	virtual vector<Base *> getBases() = 0;

	virtual vector<Controls *> getControls() = 0;

	virtual Camera *getCamera() = 0;

	virtual vector<Shooter *> getShooters() = 0;

	// Only drawable items, not Base
	virtual vector<Drawable *> getHUDDrawables() = 0;
};

#endif //PHYVR_PLAYER_H
