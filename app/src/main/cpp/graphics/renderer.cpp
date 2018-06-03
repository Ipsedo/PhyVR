//
// Created by samuel on 25/05/18.
//

#include "renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLES2/gl2.h>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(vector<Base *> *bases) {
	this->bases = bases;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	glDisable(GL_BLEND);

	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	camPos = glm::vec3(0.f, 5.f, -5.f);
	camLookAtVec = glm::vec3(0.f, 0.f, 1.f);
}

void Renderer::update(glm::mat4 mHeadView, bool VR) {
	camPos = camera->camPos(VR);
	camLookAtVec = camera->camLookAtVec(VR);
	camUpVec = camera->camUpVec(VR);
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

	mCamera = glm::lookAt(camPos,
						  camPos + camLookAtVec,
						  camUpVec);

	glm::mat4 mViewMatrix = mEyeViewMatrix * mCamera;


	glm::vec4 lighPos = this->updateLight(mViewMatrix, glm::vec3(0.f, 500.f, 0.f));

	for (Base *b : *bases)
		b->draw(mEyeProjectionMatrix, mViewMatrix, glm::vec3(lighPos.x, lighPos.y, lighPos.z));
}

glm::vec4 Renderer::updateLight(glm::mat4 viewMatrix, glm::vec3 xyz) {
	glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), xyz);
	return viewMatrix * lightModel * glm::vec4(0.f, 0.f, 0.f, 1.0f);

}

void Renderer::setCamera(Camera *c) {
	this->camera = c;
}
