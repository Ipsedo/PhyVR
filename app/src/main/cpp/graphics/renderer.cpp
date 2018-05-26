//
// Created by samuel on 25/05/18.
//

#include "renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLES2/gl2.h>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(vector<Box*>* boxes) {
    this->boxes = boxes;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);

    glDisable(GL_BLEND);

    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
}

void Renderer::update(glm::mat4 mHeadView) {

}

void Renderer::draw(glm::mat4 mEyeProjectionMatrix,
                 glm::mat4 mEyeViewMatrix,
                 glm::vec4 myLighPosInEyeSpace,
                 glm::vec3 mCameraPos) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);

    glDisable(GL_BLEND);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mCamera = glm::lookAt(glm::vec3(0.f),
                          glm::vec3(0.f, 0.f, 1.f),
                          glm::vec3(0.f, 1.f, 0.f));

    glm::mat4 mViewMatrix = mEyeViewMatrix * mCamera;



    glm::vec4 lighPos = this->updateLight(mViewMatrix, glm::vec3(0.f));

    for (Box* b : *boxes)
        b->draw(mEyeProjectionMatrix, mViewMatrix, glm::vec3(lighPos.x, lighPos.y, lighPos.z));
}

glm::vec4 Renderer::updateLight(glm::mat4 viewMatrix, glm::vec3 xyz) {
    glm::mat4 lightModel(1.f);
    lightModel = glm::translate(lightModel, xyz);
    glm::vec4 lightPosInModelSpace(0.f, 0.f, 0.f, 1.f);
    return viewMatrix * lightModel * lightPosInModelSpace;

}
