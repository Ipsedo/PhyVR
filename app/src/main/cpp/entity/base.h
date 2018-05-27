//
// Created by samuel on 27/05/18.
//

#ifndef PHYVR_BASE_H
#define PHYVR_BASE_H

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include "../graphics/drawable/modelvbo.h"

class Base {
public:
    btRigidBody* rigidBody;
    std::tuple<glm::mat4, glm::mat4> getMatrixes(glm::mat4 pMatrix, glm::mat4 vMatrix);
    virtual void draw(glm::mat4 pMatrix, glm::mat4 vMatrix, glm::vec3 lighPos) = 0;
    virtual ~Base();
protected:
    btCollisionShape* collisionShape;
    btDefaultMotionState* defaultMotionState;
    btTransform myTransform;

    glm::vec3 scale;
    glm::mat4 modelMatrix;
};


#endif //PHYVR_BASE_H