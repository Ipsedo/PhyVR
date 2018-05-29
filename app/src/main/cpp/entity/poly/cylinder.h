//
// Created by samuel on 29/05/18.
//

#ifndef PHYVR_CYLINDER_H
#define PHYVR_CYLINDER_H


#include "../base.h"
#include <glm/glm.hpp>
#include <android/asset_manager.h>

class Cylinder : public Base {
public:
    Cylinder(AAssetManager* mgr, glm::vec3 pos, glm::vec3 scale, glm::mat4 rotationMatrix, float mass);
    void draw(glm::mat4 pMatrix, glm::mat4 vMatrix, glm::vec3 lighPos) override;
    ~Cylinder();
private:
    ModelVBO* modelVBO;
};


#endif //PHYVR_CYLINDER_H