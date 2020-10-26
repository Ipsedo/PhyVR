#include <jni.h>
#include <string>

#include "phyvr.h"

#define JNI_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_com_samuelberrien_phyvr_GameActivity_##method_name

namespace {

    inline jlong jptr(phyvr_app::CardboardApp* native_app) {
        return reinterpret_cast<intptr_t>(native_app);
    }

    inline phyvr_app::CardboardApp* native(jlong ptr) {
        return reinterpret_cast<phyvr_app::CardboardApp*>(ptr);
    }

    JavaVM* javaVm;

}  // anonymous namespace

extern "C" {

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    javaVm = vm;
    return JNI_VERSION_1_6;
}

JNI_METHOD(jlong, nativeOnCreate)
(JNIEnv* env, jobject obj, jobject asset_mgr) {
    return jptr(new phyvr_app::CardboardApp(javaVm, obj, asset_mgr));
}

JNI_METHOD(void, nativeOnDestroy)
(JNIEnv* env, jobject obj, jlong native_app) { delete native(native_app); }

JNI_METHOD(void, nativeOnSurfaceCreated)
(JNIEnv* env, jobject obj, jlong native_app) {
    native(native_app)->on_surface_created(env);
}

JNI_METHOD(void, nativeOnDrawFrame)
(JNIEnv* env, jobject obj, jlong native_app) {
    native(native_app)->on_draw_frame();
}

JNI_METHOD(void, nativeOnTriggerEvent)
(JNIEnv* env, jobject obj, jlong native_app) {
    native(native_app)->on_trigger_event();
}

JNI_METHOD(void, nativeOnPause)
(JNIEnv* env, jobject obj, jlong native_app) { native(native_app)->on_pause(); }

JNI_METHOD(void, nativeOnResume)
(JNIEnv* env, jobject obj, jlong native_app) { native(native_app)->on_resume(); }

JNI_METHOD(void, nativeSetScreenParams)
(JNIEnv* env, jobject obj, jlong native_app, jint width, jint height) {
    native(native_app)->set_screen_parameters(width, height);
}

JNI_METHOD(void, nativeSwitchViewer)
(JNIEnv* env, jobject obj, jlong native_app) {
    native(native_app)->switch_viwer();
}

}
