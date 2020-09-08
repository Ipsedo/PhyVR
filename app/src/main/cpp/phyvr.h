//
// Created by samuel on 06/09/20.
//

#ifndef PHYVR_PHYVR_H
#define PHYVR_PHYVR_H

#include <android/asset_manager.h>
#include <jni.h>

#include <cardboard.h>

#include <glm/glm.hpp>
#include <GLES2/gl2.h>

#include "view/graphics.h"
#include "model/physics.h"
#include "controller/inputs.h"

namespace ndk_phyvr {

/**
 * This is a sample app for the Cardboard SDK. It loads a simple environment and
 * objects that you can click on.
 */

    class App {
    public:
        App(JavaVM *vm, jobject obj, jobject asset_mgr_obj);

        virtual ~App();

        virtual void on_surface_created(JNIEnv *env) = 0;

        virtual void set_screen_parameters(int width, int height) = 0;

        virtual void on_draw_frame() = 0;

        virtual void on_trigger_event() = 0;

        virtual void on_pause() = 0;

        virtual void on_resume() = 0;

        virtual void switch_viwer() = 0;

    protected:
        jobject java_asset_mgr_;
        AAssetManager *asset_mgr_;
    };

    class NormalApp : public App {
    public:
        void on_surface_created(JNIEnv *env) override;

        void set_screen_parameters(int width, int height) override;

        void on_draw_frame() override;

        void on_trigger_event() override;

        void on_pause() override;

        void on_resume() override;

        void switch_viwer() override;
    };

    class CardboardApp : public App {
    public:
        CardboardApp(JavaVM *vm, jobject obj, jobject asset_mgr_obj);

        ~CardboardApp() override;

        void on_surface_created(JNIEnv *env) override;

        void set_screen_parameters(int width, int height) override;

        void on_draw_frame() override;

        void on_trigger_event() override;

        void on_pause() override;

        void on_resume() override;

        void switch_viwer() override;

    private:
        static constexpr float kZNear = 0.1f;
        static constexpr float kZFar = 100.f;

        int screen_width_;
        int screen_height_;

        float projection_matrices_[2][16];
        float eye_matrices_[2][16];

        CardboardHeadTracker *head_tracker_;
        CardboardLensDistortion *lens_distortion_;
        CardboardDistortionRenderer *distortion_renderer_;

        CardboardEyeTextureDescription left_eye_texture_description_;
        CardboardEyeTextureDescription right_eye_texture_description_;

        bool device_params_changed_;
        bool screen_params_changed_;

        bool update_device_params();

        glm::mat4 get_cam_pos();
    };
}

#endif //PHYVR_PHYVR_H
