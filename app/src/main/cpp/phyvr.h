//
// Created by samuel on 06/09/20.
//

#ifndef PHYVR_PHYVR_H
#define PHYVR_PHYVR_H

#include <android/asset_manager.h>
#include <jni.h>

#include <cardboard.h>

#include <glm/glm.hpp>
#include <GLES3/gl3.h>

namespace ndk_phyvr {

/**
 * This is a sample app for the Cardboard SDK. It loads a simple environment and
 * objects that you can click on.
 */
    class HelloCardboardApp {
    public:
        /**
         * Creates a HelloCardboardApp.
         *
         * @param vm JavaVM pointer.
         * @param obj Android activity object.
         * @param asset_mgr_obj The asset manager object.
         */
        HelloCardboardApp(JavaVM *vm, jobject obj, jobject asset_mgr_obj);

        ~HelloCardboardApp();

        /**
         * Initializes any GL-related objects. This should be called on the rendering
         * thread with a valid GL context.
         *
         * @param env The JNI environment.
         */
        void on_surface_created(JNIEnv *env);

        /**
         * Sets screen parameters.
         *
         * @param width Screen width
         * @param height Screen height
         */
        void set_screen_parameters(int width, int height);

        /**
         * Draws the scene. This should be called on the rendering thread.
         */
        void on_draw_frame();

        /**
         * Hides the target object if it's being targeted.
         */
        void on_trigger_event();

        /**
         * Pauses head tracking.
         */
        void on_pause();

        /**
         * Resumes head tracking.
         */
        void on_resume();

        /**
         * Allows user to switch viewer.
         */
        void switch_viwer();

    private:
        /**
         * Default near clip plane z-axis coordinate.
         */
        static constexpr float kZNear = 0.1f;

        /**
         * Default far clip plane z-axis coordinate.
         */
        static constexpr float kZFar = 100.f;

        /**
         * Updates device parameters, if necessary.
         *
         * @return true if device parameters were successfully updated.
         */
        bool update_device_params();

        /**
         * Initializes GL environment.
         */
        void gl_steup();

        /**
         * Deletes GL environment.
         */
        void gl_teardown();

        /**
         * Gets head's pose as a 4x4 matrix.
         *
         * @return matrix containing head's pose.
         */
        glm::mat4 get_pose();

        /**
         * Draw
         */
        void draw();


        jobject java_asset_mgr_;
        AAssetManager *asset_mgr_;

        CardboardHeadTracker *head_tracker_;
        CardboardLensDistortion *lens_distortion_;
        CardboardDistortionRenderer *distortion_renderer_;

        CardboardEyeTextureDescription left_eye_texture_description_;
        CardboardEyeTextureDescription right_eye_texture_description_;

        bool screen_params_changed_;
        bool device_params_changed_;
        int screen_width_;
        int screen_height_;

        GLuint framebuffer_;

        glm::mat4 head_view_;
        float projection_matrices_[2][16];
        float eye_matrices_[2][16];
    };
}

#endif //PHYVR_PHYVR_H
