//
// Created by samuel on 08/09/20.
//

#ifndef PHYVR_NDK_DRAWABLE_H
#define PHYVR_NDK_DRAWABLE_H

#include <glm/glm.hpp>
#include <android/asset_manager.h>
#include <string>
#include <vector>
#include <tuple>
#include <GLES2/gl2.h>

#include "graphics.h"

namespace phyvr_view {

    class Drawable {
    public:
        virtual void draw(gl_infos) = 0;
    };

    class ObjDrawable : public Drawable {
    public:
        ObjDrawable(
                AAssetManager *a_asset_manager,
                const std::string &obj_file_name,
                const std::string &texture_file
        );

        ~ObjDrawable();

        void draw(gl_infos infos) override;

    private:
        const char *fragment_shader_prgm =
                "precision mediump float;\n"
                "\n"
                "uniform sampler2D u_tex;\n"
                "uniform vec3 u_light_pos;\n"
                "\n"
                "varying vec3 v_position;\n"
                "varying vec2 v_tex_coord;\n"
                "varying vec3 v_normal;\n"
                "\n"
                "void main() {\n"
                "    vec3 lightVector = normalize(u_light_pos - v_position);\n"
                "    float diffuse = max(dot(v_normal, lightVector), 0.1);\n"
                "    gl_FragColor = texture2D(u_tex, v_tex_coord) * diffuse;\n"
                "}\n";

        const char *vertex_shader_prgm =
                "uniform mat4 u_mvp_matrix;\n"
                "uniform mat4 u_mv_matrix;\n"
                "\n"
                "attribute vec3 a_position;\n"
                "attribute vec3 a_normal;\n"
                "attribute vec2 a_tex_coord;\n"
                "\n"
                "varying vec3 v_position;\n"
                "varying vec2 v_tex_coord;\n"
                "varying vec3 v_normal;\n"
                "\n"
                "void main() {\n"
                "  v_position = vec3(u_mv_matrix * vec4(a_position, 1.0));\n"
                "  v_tex_coord = a_tex_coord;\n"
                "  v_normal = normalize(vec3(u_mv_matrix * vec4(a_normal, 0.0)));\n"
                "  gl_Position = u_mvp_matrix * vec4(a_position, 1.0);\n"
                "}\n";


        const int POSITION_SIZE = 3;
        const int NORMAL_SIZE = 3;
        const int TEX_COORD_SIZE = 2;
        const int BYTES_PER_FLOAT = 4;
        const int STRIDE = (POSITION_SIZE + NORMAL_SIZE + TEX_COORD_SIZE) * BYTES_PER_FLOAT;

        int nb_vertex;

        GLuint m_program;
        GLuint vertex_shader;
        GLuint fragment_shader;

        GLuint m_position_handle;
        GLuint m_normal_handle;
        GLuint m_tex_coord_handle;
        GLuint m_mvp_handle;
        GLuint m_light_pos_handle;
        GLuint m_mv_handle;
        GLuint m_tex_handle;

        unsigned char *texture_pixels;
        GLuint *texture_tex;

        GLuint buffer;

    };


}

#endif //PHYVR_NDK_DRAWABLE_H
