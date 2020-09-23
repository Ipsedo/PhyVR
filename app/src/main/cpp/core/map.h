//
// Created by samuel on 20/09/20.
//

#ifndef PHYVR_NDK_MAP_H
#define PHYVR_NDK_MAP_H

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include "entity.h"

namespace phyvr_core {

    btHeightfieldTerrainShape *make_shape(
            unsigned char *normalized_height_values, int width, int height, glm::vec3 scale
    );

    class Map : public Entity {
    public:
        Map(btHeightfieldTerrainShape *shape, glm::vec3 pos, glm::vec3 scale);

        void update() override;

        void on_contact_finish(Entity *other) override;

        void damage(int to_sub) override;
    };
}

#endif //PHYVR_NDK_MAP_H
