//
// Created by samuel on 30/05/18.
//


#include <jni.h>
#include <android/asset_manager_jni.h>
#include "../level/level.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_samuelberrien_phyvr_MyGvrView_initCar(JNIEnv *env, jobject instance, jobject assetManager,
                                               jlong levelPtr, jlong rendererPtr, jlong entityPtr) {

    Level* level = (Level*) levelPtr;
    Renderer* renderer = (Renderer*) rendererPtr;
    vector<Base*>* entity = (vector<Base*>*) entityPtr;
    AAssetManager* cppMgr = AAssetManager_fromJava(env, assetManager);

    Car* c = new Car(level->world, cppMgr);
    entity->push_back(c);
    level->addNewBox(c);
    renderer->setCamera(c);

    return (long) c;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_samuelberrien_phyvr_controls_Controls_control(JNIEnv *env, jobject instance,
                                                       jlong controlPtr, jfloat direction,
                                                       jfloat speed) {

    Car* c = (Car*) controlPtr;
    c->control(direction, speed);

}
