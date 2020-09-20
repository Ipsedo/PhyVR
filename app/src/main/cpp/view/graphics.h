//
// Created by samuel on 08/09/20.
//

#ifndef PHYVR_NDK_GRAPHICS_H
#define PHYVR_NDK_GRAPHICS_H

#include <GLES2/gl2.h>
#include <string>
#include <android/asset_manager.h>
#include <glm/glm.hpp>

namespace phyvr_view {

    struct gl_infos {
        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;
        glm::mat4 model_matrix;
        glm::vec3 light_pos;
    };

    /*
     * OpenGL stuff
     */

    GLuint load_shader(GLenum type, const char *shaderSource);

    void check_gl_error(const char *tag);

}

#endif //PHYVR_NDK_GRAPHICS_H
