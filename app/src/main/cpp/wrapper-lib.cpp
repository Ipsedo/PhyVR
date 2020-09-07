#include <jni.h>
#include <string>

#include <btBulletDynamicsCommon.h>
#include <GLES3/gl3.h>
#include <SOIL2/SOIL2.h>

#include "phyvr.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_samuelberrien_phyvr_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
