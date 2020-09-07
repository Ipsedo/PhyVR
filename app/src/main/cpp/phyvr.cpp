//
// Created by samuel on 06/09/20.
//

#include "phyvr.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <GLES2/gl2.h>
#include <array>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

static constexpr uint64_t k_nanos_in_seconds = 1000000000;
static constexpr uint64_t k_prediction_time_without_vsync_nanos = 50000000;

/*
 * App
 */

ndk_phyvr::App::App(JavaVM *vm, jobject obj, jobject asset_mgr_obj) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    java_asset_mgr_ = env->NewGlobalRef(asset_mgr_obj);
    asset_mgr_ = AAssetManager_fromJava(env, asset_mgr_obj);
}

ndk_phyvr::App::~App() {

}

void ndk_phyvr::App::on_surface_created(JNIEnv *env) {

}

void ndk_phyvr::App::set_screen_parameters(int width, int height) {

}

void ndk_phyvr::App::on_draw_frame() {

}

void ndk_phyvr::App::on_trigger_event() {

}

void ndk_phyvr::App::on_pause() {

}

void ndk_phyvr::App::on_resume() {

}

void ndk_phyvr::App::switch_viwer() {

}

/*
 * NormalApp
 */


void ndk_phyvr::NormalApp::on_surface_created(JNIEnv *env) {

}

void ndk_phyvr::NormalApp::set_screen_parameters(int width, int height) {

}

void ndk_phyvr::NormalApp::on_draw_frame() {

}

void ndk_phyvr::NormalApp::on_trigger_event() {

}

void ndk_phyvr::NormalApp::on_pause() {

}

void ndk_phyvr::NormalApp::on_resume() {

}

void ndk_phyvr::NormalApp::switch_viwer() {

}

/*
 * CardboardApp
 */

ndk_phyvr::CardboardApp::CardboardApp(JavaVM *vm, jobject obj, jobject asset_mgr_obj)
        : App(vm, obj,
              asset_mgr_obj),
          head_tracker_(nullptr),
          lens_distortion_(nullptr),
          distortion_renderer_(nullptr),
          screen_width_(0),
          screen_height_(0),
          screen_params_changed_(false),
          device_params_changed_(false){
    Cardboard_initializeAndroid(vm, obj);
    head_tracker_ = CardboardHeadTracker_create();
}

ndk_phyvr::CardboardApp::~CardboardApp() {
    CardboardHeadTracker_destroy(head_tracker_);
    CardboardLensDistortion_destroy(lens_distortion_);
    CardboardDistortionRenderer_destroy(distortion_renderer_);
}

void ndk_phyvr::CardboardApp::on_surface_created(JNIEnv *env) {
    //TODO init gl stuff
}

void ndk_phyvr::CardboardApp::set_screen_parameters(int width, int height) {
    screen_width_ = width;
    screen_height_ = height;
    screen_params_changed_ = true;
}

void ndk_phyvr::CardboardApp::on_draw_frame() {
    if (!update_device_params()) {
        return;
    }

    //TODO graphic motor
    CardboardDistortionRenderer_renderEyeToDisplay(
            distortion_renderer_, 0, 0, 0,
            screen_width_, screen_height_, &left_eye_texture_description_,
            &right_eye_texture_description_);
}

void ndk_phyvr::CardboardApp::on_trigger_event() {

}

void ndk_phyvr::CardboardApp::on_pause() {
    CardboardHeadTracker_pause(head_tracker_);
}

void ndk_phyvr::CardboardApp::on_resume() {
    CardboardHeadTracker_resume(head_tracker_);

    // Parameters may have changed.
    device_params_changed_ = true;

    // Check for device parameters existence in external storage. If they're
    // missing, we must scan a Cardboard QR code and save the obtained parameters.
    uint8_t* buffer;
    int size;
    CardboardQrCode_getSavedDeviceParams(&buffer, &size);
    if (size == 0) {
        switch_viwer();
    }
    CardboardQrCode_destroy(buffer);
}

void ndk_phyvr::CardboardApp::switch_viwer() {
    CardboardQrCode_scanQrCodeAndSaveDeviceParams();
}

bool ndk_phyvr::CardboardApp::update_device_params() {
    if (!screen_params_changed_ && !device_params_changed_) {
        return true;
    }

    // Get saved device parameters
    uint8_t* buffer;
    int size;
    CardboardQrCode_getSavedDeviceParams(&buffer, &size);

    // If there are no parameters saved yet, returns false.
    if (size == 0) {
        return false;
    }

    CardboardLensDistortion_destroy(lens_distortion_);
    lens_distortion_ = CardboardLensDistortion_create(
            buffer, size, screen_width_, screen_height_);

    CardboardQrCode_destroy(buffer);

    //TODO GLSetup()

    CardboardDistortionRenderer_destroy(distortion_renderer_);
    distortion_renderer_ = CardboardDistortionRenderer_create();

    CardboardMesh left_mesh;
    CardboardMesh right_mesh;
    CardboardLensDistortion_getDistortionMesh(lens_distortion_, kLeft,
                                              &left_mesh);
    CardboardLensDistortion_getDistortionMesh(lens_distortion_, kRight,
                                              &right_mesh);

    CardboardDistortionRenderer_setMesh(distortion_renderer_, &left_mesh, kLeft);
    CardboardDistortionRenderer_setMesh(distortion_renderer_, &right_mesh,
                                        kRight);

    // Get eye matrices
    CardboardLensDistortion_getEyeFromHeadMatrix(
            lens_distortion_, kLeft, eye_matrices_[0]);
    CardboardLensDistortion_getEyeFromHeadMatrix(
            lens_distortion_, kRight, eye_matrices_[1]);
    CardboardLensDistortion_getProjectionMatrix(
            lens_distortion_, kLeft, kZNear, kZFar, projection_matrices_[0]);
    CardboardLensDistortion_getProjectionMatrix(
            lens_distortion_, kRight, kZNear, kZFar, projection_matrices_[1]);

    screen_params_changed_ = false;
    device_params_changed_ = false;

    return true;
}

glm::mat4 ndk_phyvr::CardboardApp::get_cam_pos() {
    std::array<float, 4> out_orientation{};
    std::array<float, 3> out_position{};

    timespec res{};
    clock_gettime(CLOCK_MONOTONIC, &res);

    unsigned long monotonic_time_nano = (res.tv_sec * k_nanos_in_seconds) + res.tv_nsec;
    monotonic_time_nano += k_prediction_time_without_vsync_nanos;
    CardboardHeadTracker_getPose(head_tracker_, monotonic_time_nano,
                                 &out_position[0], &out_orientation[0]);

    glm::vec3 pos(out_position[0], out_position[1], out_position[2]);
    glm::quat rot_quat(out_orientation[3], out_orientation[0], out_orientation[1], out_orientation[2]);

    return glm::translate(glm::mat4(1.f), pos) * glm::mat4_cast(rot_quat);


}
