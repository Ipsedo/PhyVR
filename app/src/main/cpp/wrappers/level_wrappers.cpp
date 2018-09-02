//
// Created by samuel on 19/08/18.
//

#include <jni.h>
#include <android/asset_manager_jni.h>
#include "../core/engine.h"
#include "../levels/level0/level0.h"
#include "../levels/level.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_samuelberrien_phyvr_wrappers_MainWrappers_makeLevel(JNIEnv *env, jobject instance) {
	nbNew = 0;
	nbDel = 0;
	return (long) new Level0();//LevelDemo();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_samuelberrien_phyvr_wrappers_MainWrappers_initLevel(JNIEnv *env, jobject instance, jobject manager,
															 jboolean isVR, jlong levelPtr, jlong enginePtr) {

	AAssetManager *cppMgr = AAssetManager_fromJava(env, manager);
	Engine *engine = (Engine *) enginePtr;
	Level *level = (Level *) levelPtr;
	level->init(isVR, cppMgr, engine->world);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_samuelberrien_phyvr_wrappers_MainWrappers_hasWon(JNIEnv *env, jobject instance, jlong levelPtr) {
	return (jboolean) ((Level *) levelPtr)->won();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_samuelberrien_phyvr_wrappers_MainWrappers_hasLose(JNIEnv *env, jobject instance, jlong levelPtr) {
	return (jboolean) ((Level *) levelPtr)->lose();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_samuelberrien_phyvr_wrappers_MainWrappers_freeLevel(JNIEnv *env, jobject instance, jlong levelPtr) {
	delete (Level *)levelPtr;
}