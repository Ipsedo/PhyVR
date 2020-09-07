//
// Created by samuel on 06/09/20.
//

#include "phyvr.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <GLES3/gl3.h>

ndk_phyvr::HelloCardboardApp::HelloCardboardApp(JavaVM *vm, jobject obj, jobject asset_mgr_obj)

        : head_tracker_(nullptr),
          lens_distortion_(nullptr),
          distortion_renderer_(nullptr),
          screen_params_changed_(false),
          device_params_changed_(false),
          screen_width_(0),
          screen_height_(0){

    JNIEnv* env;
    vm->GetEnv((void**)&env, JNI_VERSION_1_6);
    java_asset_mgr_ = env->NewGlobalRef(asset_mgr_obj);
    asset_mgr_ = AAssetManager_fromJava(env, asset_mgr_obj);

    Cardboard_initializeAndroid(vm, obj);
    head_tracker_ = CardboardHeadTracker_create();
}

ndk_phyvr::HelloCardboardApp::~HelloCardboardApp() {
    CardboardHeadTracker_destroy(head_tracker_);
    CardboardLensDistortion_destroy(lens_distortion_);
    CardboardDistortionRenderer_destroy(distortion_renderer_);
}

void ndk_phyvr::HelloCardboardApp::on_surface_created(JNIEnv *env) {
    //TODO Load gl programs
}

void ndk_phyvr::HelloCardboardApp::set_screen_parameters(int width, int height) {
    screen_width_ = width;
    screen_height_ = height;
    screen_params_changed_ = true;
}

void ndk_phyvr::HelloCardboardApp::on_draw_frame() {
    if (!update_device_params()) {
        return;
    }

    // Update Head Pose.
    head_view_ = get_pose();

    // Incorporate the floor height into the head_view
    //head_view_ = head_view_ * GetTranslationMatrix({0.0f, kDefaultFloorHeight, 0.0f});

    // Bind buffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw eyes views
    for (int eye = 0; eye < 2; ++eye) {
        glViewport(eye == kLeft ? 0 : screen_width_ / 2, 0, screen_width_ / 2,
                   screen_height_);

        /*Matrix4x4 eye_matrix = GetMatrixFromGlArray(eye_matrices_[eye]);
        Matrix4x4 eye_view = eye_matrix * head_view_;

        Matrix4x4 projection_matrix =
                GetMatrixFromGlArray(projection_matrices_[eye]);
        Matrix4x4 modelview_target = eye_view * model_target_;
        modelview_projection_target_ = projection_matrix * modelview_target;
        modelview_projection_room_ = projection_matrix * eye_view;*/

        // Draw room and target
        //DrawWorld();
    }

    // Render
    CardboardDistortionRenderer_renderEyeToDisplay(
            distortion_renderer_, /* target_display = */ 0, /* x = */ 0, /* y = */ 0,
            screen_width_, screen_height_, &left_eye_texture_description_,
            &right_eye_texture_description_);

    //CHECKGLERROR("onDrawFrame");
}

void ndk_phyvr::HelloCardboardApp::on_trigger_event() {
    //TODO get event
}

void ndk_phyvr::HelloCardboardApp::on_pause() {
    CardboardHeadTracker_pause(head_tracker_);
}

void ndk_phyvr::HelloCardboardApp::on_resume() {
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

void ndk_phyvr::HelloCardboardApp::switch_viwer() {
    CardboardQrCode_scanQrCodeAndSaveDeviceParams();
}

bool ndk_phyvr::HelloCardboardApp::update_device_params() {
    //TODO Do gl_setup (for texture -> when params change)
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

    gl_steup();

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

    //CHECKGLERROR("UpdateDeviceParams");

    return true;
}

void ndk_phyvr::HelloCardboardApp::gl_steup() {
    //TODO init buffer ect.
}

void ndk_phyvr::HelloCardboardApp::gl_teardown() {
    //TODO delete buffer etc
}

glm::mat4 ndk_phyvr::HelloCardboardApp::get_pose() {
    return glm::mat4();
}

void ndk_phyvr::HelloCardboardApp::draw() {

}
