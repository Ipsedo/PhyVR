//
// Created by samuel on 29/05/18.
//

#include "sphere.h"
#include "../../utils/assets.h"
#include "../../utils/rigidbody.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
/*
Sphere::Sphere(AAssetManager *mgr, glm::vec3 pos, glm::vec3 scale, glm::mat4 rotationMatrix,
			   float mass) {
	std::string objTxt = getFileText(mgr, "obj/sphere.obj");

	modelVBO = new ModelVBO(
			objTxt,
			new float[4]{(float) rand() / RAND_MAX,
						 (float) rand() / RAND_MAX,
						 (float) rand() / RAND_MAX,
						 1.f});

	this->scale.push_back(scale);

	collisionShape.push_back(new btSphereShape(1.f));
	collisionShape[0]->setLocalScaling(btVector3(scale.x, scale.y, scale.z));

	btTransform myTransform;
	myTransform.setIdentity();
	myTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	glm::quat tmp = glm::quat_cast(rotationMatrix);
	myTransform.setRotation(btQuaternion(tmp.x, tmp.y, tmp.z, tmp.w));

	std::tuple<btRigidBody *, btDefaultMotionState *> t = localCreateRigidBody(mass, myTransform, collisionShape[0],
																			   this);
	rigidBody.push_back(std::get<0>(t));
	defaultMotionState.push_back(std::get<1>(t));
}

void Sphere::draw(glm::mat4 pMatrix, glm::mat4 vMatrix, glm::vec3 lighPos) {
	std::tuple<glm::mat4, glm::mat4> matrixes = getMatrixes(pMatrix, vMatrix);
	modelVBO->draw(std::get<0>(matrixes), std::get<1>(matrixes), lighPos);
}

Sphere::~Sphere() {
	delete modelVBO;
}*/

Sphere::Sphere(const btRigidBody::btRigidBodyConstructionInfo &constructionInfo, btDefaultMotionState *motionState,
			   DiffuseModel *modelVBO, const glm::vec3 &scale) : Base(constructionInfo, motionState, modelVBO,
																		  scale) {

}

Sphere *Sphere::MakeSphere(AAssetManager *mgr, glm::vec3 pos, glm::vec3 scale, glm::mat4 rotMat, float mass) {
	std::string objTxt = getFileText(mgr, "obj/sphere.obj");

	ModelVBO *modelVBO = new ModelVBO(
			objTxt,
			new float[4]{(float) rand() / RAND_MAX,
						 (float) rand() / RAND_MAX,
						 (float) rand() / RAND_MAX,
						 1.f});

	btCollisionShape *shape = new btSphereShape(1.f);
	shape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));

	btTransform myTransform;
	myTransform.setIdentity();
	myTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	glm::quat tmp = glm::quat_cast(rotMat);
	myTransform.setRotation(btQuaternion(tmp.x, tmp.y, tmp.z, tmp.w));

	tuple<btRigidBody::btRigidBodyConstructionInfo, btDefaultMotionState *> cinfo
		= localCreateInfo(mass, myTransform, shape);
	return new Sphere(get<0>(cinfo), get<1>(cinfo), modelVBO, scale);
}
