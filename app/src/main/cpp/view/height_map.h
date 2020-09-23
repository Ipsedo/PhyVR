//
// Created by samuel on 20/09/20.
//

#ifndef PHYVR_NDK_HEIGHT_MAP_H
#define PHYVR_NDK_HEIGHT_MAP_H

#include "drawable.h"
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

namespace phyvr_view {

    class TriangleCallback : public btTriangleCallback {
    public:
        TriangleCallback();

        int nb_vertex;
        std::vector<float> packed_data;

        void processTriangle(btVector3 *triangle, int partid, int triangleindex) override;
    };

    class MapDrawable : public Drawable {
    public:
        MapDrawable(btHeightfieldTerrainShape *terrain, float maxheight);

        ~MapDrawable();

        void draw(gl_infos infos) override;

    private:
        const char *fragment_shader_prgm =
                "#version 100\n"
                "\n"
                "precision mediump float;\n"
                "\n"
                "uniform vec4 u_color;\n"
                "\n"
                "uniform vec3 u_light_pos;\n"
                "\n"
                "varying vec3 v_position;\n"
                "varying vec3 v_normal;\n"
                "\n"
                "void main() {\n"
                "    vec3 lightVector = normalize(u_light_pos - v_position);\n"
                "    float diffuse = max(dot(v_normal, lightVector), 0.1);\n"
                "    gl_FragColor = u_color * diffuse;\n"
                "}\n";

        const char *vertex_shader_prgm =
                "#version 100\n"
                "\n"
                "precision mediump float;\n"
                "\n"
                "uniform mat4 u_mvp_matrix;\n"
                "uniform mat4 u_mv_matrix;\n"
                "\n"
                "attribute vec3 a_position;\n"
                "attribute vec3 a_normal;\n"
                "\n"
                "varying vec3 v_position;\n"
                "varying vec3 v_normal;\n"
                "\n"
                "void main() {\n"
                "  v_position = vec3(u_mv_matrix * vec4(a_position, 1.0));\n"
                "  v_normal = normalize(vec3(u_mv_matrix * vec4(a_normal, 0.0)));\n"
                "  gl_Position = u_mvp_matrix * vec4(a_position, 1.0);\n"
                "}\n";

        const int POSITION_SIZE = 3;
        const int NORMAL_SIZE = 3;
        const int BYTES_PER_FLOAT = 4;
        const int STRIDE = (POSITION_SIZE + NORMAL_SIZE) * BYTES_PER_FLOAT;

        int nb_vertex;

        GLuint m_program;
        GLuint vertex_shader;
        GLuint fragment_shader;

        GLuint m_position_handle;
        GLuint m_normal_handle;
        GLuint m_mvp_handle;
        GLuint m_light_pos_handle;
        GLuint m_mv_handle;
        GLuint m_color_handle;

        GLuint buffer;

        float color[4];
    };
}

#endif //PHYVR_NDK_HEIGHT_MAP_H
