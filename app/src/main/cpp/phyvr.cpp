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
#include <glm/gtc/type_ptr.hpp>
#include <SOIL2.h>
#include <android/log.h>

static constexpr uint64_t k_nanos_in_seconds = 1000000000;
static constexpr uint64_t k_prediction_time_without_vsync_nanos = 50000000;

/*
 * App
 */

phyvr_app::App::App(JavaVM *vm, jobject obj, jobject asset_mgr_obj) :
        game_objects_(), game_engine_() {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    java_asset_mgr_ = env->NewGlobalRef(asset_mgr_obj);
    asset_mgr_ = AAssetManager_fromJava(env, asset_mgr_obj);
}

phyvr_app::App::~App() = default;

void phyvr_app::App::on_surface_created(JNIEnv *env) {

}

void phyvr_app::App::set_screen_parameters(int width, int height) {

}

void phyvr_app::App::on_draw_frame() {

}

void phyvr_app::App::on_trigger_event() {

}

void phyvr_app::App::on_pause() {

}

void phyvr_app::App::on_resume() {

}

void phyvr_app::App::switch_viwer() {

}

/*
 * NormalApp
 */


void phyvr_app::NormalApp::on_surface_created(JNIEnv *env) {

}

void phyvr_app::NormalApp::set_screen_parameters(int width, int height) {

}

void phyvr_app::NormalApp::on_draw_frame() {

}

void phyvr_app::NormalApp::on_trigger_event() {

}

void phyvr_app::NormalApp::on_pause() {

}

void phyvr_app::NormalApp::on_resume() {

}

void phyvr_app::NormalApp::switch_viwer() {

}

/*
 * CardboardApp
 */

phyvr_app::CardboardApp::CardboardApp(JavaVM *vm, jobject obj, jobject asset_mgr_obj)
        : App(vm, obj,
              asset_mgr_obj),
          head_tracker_(nullptr),
          lens_distortion_(nullptr),
          distortion_renderer_(nullptr),
          depthRenderBuffer_(0),
          framebuffer_(0),
          texture_(0),
          screen_width_(0),
          screen_height_(0),
          screen_params_changed_(false),
          device_params_changed_(false),
          projection_matrices_(),
          eye_matrices_(),
          left_eye_texture_description_(),
          right_eye_texture_description_(),
          rand_gen(std::chrono::system_clock::now().time_since_epoch().count()),
          real_dist(0.f, 1.f) {
    Cardboard_initializeAndroid(vm, obj);
    head_tracker_ = CardboardHeadTracker_create();
}

phyvr_app::CardboardApp::~CardboardApp() {
    CardboardHeadTracker_destroy(head_tracker_);
    CardboardLensDistortion_destroy(lens_distortion_);
    CardboardDistortionRenderer_destroy(distortion_renderer_);
}

void phyvr_app::CardboardApp::on_surface_created(JNIEnv *env) {
    //TODO init gl stuff
    gl_setup();
}

void phyvr_app::CardboardApp::set_screen_parameters(int width, int height) {
    screen_width_ = width;
    screen_height_ = height;
    screen_params_changed_ = true;
}

void phyvr_app::CardboardApp::on_draw_frame() {
    if (!update_device_params()) {
        return;
    }

    glm::mat4 head_view = get_cam_pos();

    // Update model
    game_engine_.update();
    game_objects_.erase(std::remove_if(game_objects_.begin(), game_objects_.end(),
                                       [](const game_object &go) { return go.entity->is_dead(); }),
                        game_objects_.end());

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

        glm::mat4 eye_matrix = glm::make_mat4(eye_matrices_[eye]);
        glm::mat4 eye_view = eye_matrix * head_view;

        glm::mat4 projection_matrix =
                glm::make_mat4(projection_matrices_[eye]);

        for (const auto &go: game_objects_) {
            glm::mat4 model_matrix = go.entity->get_model_matrix();

            phyvr_view::gl_infos infos{
                    projection_matrix,
                    eye_view,
                    model_matrix,
                    glm::vec3(500, 400, 200)
            };

            go.drawable->draw(infos);
        }
    }

    //TODO graphic motor
    CardboardDistortionRenderer_renderEyeToDisplay(
            distortion_renderer_, 0, 0, 0,
            screen_width_, screen_height_, &left_eye_texture_description_,
            &right_eye_texture_description_);
}

void phyvr_app::CardboardApp::on_trigger_event() {

}

void phyvr_app::CardboardApp::on_pause() {
    CardboardHeadTracker_pause(head_tracker_);
}

void phyvr_app::CardboardApp::on_resume() {
    CardboardHeadTracker_resume(head_tracker_);

    // Parameters may have changed.
    device_params_changed_ = true;

    // Check for device parameters existence in external storage. If they're
    // missing, we must scan a Cardboard QR code and save the obtained parameters.
    uint8_t *buffer;
    int size;
    CardboardQrCode_getSavedDeviceParams(&buffer, &size);
    if (size == 0) {
        switch_viwer();
    }
    CardboardQrCode_destroy(buffer);
}

void phyvr_app::CardboardApp::switch_viwer() {
    CardboardQrCode_scanQrCodeAndSaveDeviceParams();
}

void phyvr_app::CardboardApp::gl_teardown() {
    if (framebuffer_ == 0) {
        return;
    }
    glDeleteRenderbuffers(1, &depthRenderBuffer_);
    depthRenderBuffer_ = 0;
    glDeleteFramebuffers(1, &framebuffer_);
    framebuffer_ = 0;
    glDeleteTextures(1, &texture_);
    texture_ = 0;
}

void phyvr_app::CardboardApp::gl_setup() {
    if (framebuffer_ != 0) {
        gl_teardown();
    }

    // Create render texture.
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width_, screen_height_, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    left_eye_texture_description_.texture = texture_;
    left_eye_texture_description_.left_u = 0;
    left_eye_texture_description_.right_u = 0.5;
    left_eye_texture_description_.top_v = 1;
    left_eye_texture_description_.bottom_v = 0;

    right_eye_texture_description_.texture = texture_;
    right_eye_texture_description_.left_u = 0.5;
    right_eye_texture_description_.right_u = 1;
    right_eye_texture_description_.top_v = 1;
    right_eye_texture_description_.bottom_v = 0;

    // Generate depth buffer to perform depth test.
    glGenRenderbuffers(1, &depthRenderBuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screen_width_,
                          screen_height_);

    // Create render target.
    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture_, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthRenderBuffer_);

    game_objects_.clear();
    game_engine_.clear_entities();

    // Cube
    auto cube_drawable = std::make_shared<phyvr_view::ObjDrawable>(
            asset_mgr_, "obj/cube.obj", "158.png"
    );
    auto cube_entity = std::make_shared<phyvr_core::Cube>(
            glm::vec3(0.f, 0.f, -10.f),
            glm::vec3(1.f),
            glm::rotate(glm::mat4(1.f), real_dist(rand_gen) * 360.f,
                        glm::vec3(sqrt(2.), sqrt(2), 0.f)),
            10.f, 1);

    phyvr_app::game_object cube_object{
            cube_drawable, cube_entity
    };

    game_objects_.push_back(cube_object);
    game_engine_.add_entity(cube_entity);

    // Floor

    AAsset *file = AAssetManager_open(asset_mgr_, "heightmap.png", AASSET_MODE_BUFFER);

    auto length = AAsset_getLength(file);
    auto img_buffer = AAsset_getBuffer(file);

    int width = 0, height = 0, channels = 0;
    auto texture_pixels = SOIL_load_image_from_memory(
            (const unsigned char *) img_buffer, length,
            &width, &height, &channels,
            SOIL_LOAD_L
    );

    auto array_px = new float[width * height];
    for (size_t i = 0; i < width * height; i++)
        array_px[i] = float(texture_pixels[i]) / 255.f;

    glm::vec3 terrain_scale(10.f, 200.f, 10.f);
    auto terrain_field = phyvr_core::make_shape(array_px, width, height, terrain_scale);

    auto map_drawable = std::make_shared<phyvr_view::MapDrawable>(
            terrain_field
    );

    auto map_entity = std::make_shared<phyvr_core::Map>(
            terrain_field, glm::vec3(0.f, 40.f, 0.f)
    );

    game_object map_object{
            map_drawable, map_entity
    };

    SOIL_free_image_data(texture_pixels);
    delete[] array_px;

    game_objects_.push_back(map_object);
    game_engine_.add_entity(map_entity);
}

bool phyvr_app::CardboardApp::update_device_params() {
    if (!screen_params_changed_ && !device_params_changed_) {
        return true;
    }

    // Get saved device parameters
    uint8_t *buffer;
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

    gl_setup();

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

glm::mat4 phyvr_app::CardboardApp::get_cam_pos() {
    std::array<float, 4> out_orientation{};
    std::array<float, 3> out_position{};

    timespec res{};
    clock_gettime(CLOCK_MONOTONIC, &res);

    unsigned long monotonic_time_nano = (res.tv_sec * k_nanos_in_seconds) + res.tv_nsec;
    monotonic_time_nano += k_prediction_time_without_vsync_nanos;
    CardboardHeadTracker_getPose(head_tracker_, monotonic_time_nano,
                                 &out_position[0], &out_orientation[0]);

    glm::vec3 pos(out_position[0], out_position[1], out_position[2]);
    glm::quat rot_quat(out_orientation[3], out_orientation[0], out_orientation[1],
                       out_orientation[2]);

    return glm::translate(glm::mat4(1.f), pos) * glm::mat4_cast(rot_quat);
}
