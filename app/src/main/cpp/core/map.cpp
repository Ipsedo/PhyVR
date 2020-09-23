//
// Created by samuel on 20/09/20.
//

#include "map.h"
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

btHeightfieldTerrainShape *
phyvr_core::make_shape(unsigned char *normalized_height_values, int width, int length, glm::vec3 scale) {
    auto map = new btHeightfieldTerrainShape(
            width, length, normalized_height_values, 255, 0.f, 1.f,
            1, PHY_UCHAR, false
    );
    map->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
    return map;
}

phyvr_core::Map::Map(btHeightfieldTerrainShape *shape, glm::vec3 pos,
                     glm::vec3 scale)
        : Entity(
        [shape]() {
            return shape;
        },
        pos, glm::mat4(1.f), scale, 0.f) {

}

void phyvr_core::Map::update() {

}

void phyvr_core::Map::on_contact_finish(phyvr_core::Entity *other) {

}

void phyvr_core::Map::damage(int to_sub) {

}
