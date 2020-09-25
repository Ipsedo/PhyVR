//
// Created by samuel on 08/09/20.
//

#include <android/log.h>
#include "graphics.h"

GLuint phyvr_view::load_shader(GLenum type, const char *shaderSource) {
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);


    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        __android_log_print(ANDROID_LOG_ERROR, "PhyVR", "%s", "shader compilation problem");
        __android_log_print(ANDROID_LOG_DEBUG, "PhyVR", "%s", shaderSource);
        exit(EXIT_FAILURE);
    }
    return shader;
}

void phyvr_view::check_gl_error(const char *tag) {
    GLenum err;
    bool failed = false;
    while ((err = glGetError()) != GL_NO_ERROR) {
        __android_log_print(ANDROID_LOG_ERROR, "PhyVR", "[%s] OpenGL Error : %d", tag, err);
        failed = true;
    }
    if (failed)
        exit(EXIT_FAILURE);
}

glm::vec3 phyvr_view::Camera::cam_pos() {
    return glm::vec3(0.1);
}

glm::vec3 phyvr_view::Camera::cam_look_at() {
    return glm::vec3(0.f, 0.f, 1.f);
}

glm::vec3 phyvr_view::Camera::cam_up() {
    return glm::vec3(0.f, 1.f, 0.f);
}
