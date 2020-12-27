//
// Created by samuel on 20/09/20.
//

#ifndef PHYVR_NDK_HEIGHT_MAP_H
#define PHYVR_NDK_HEIGHT_MAP_H

#include "drawable.h"
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <random>

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
        MapDrawable(btHeightfieldTerrainShape *terrain, int width, int height);

        ~MapDrawable();

        void draw(gl_infos infos) override;

    private:
        const char *fragment_shader_prgm =
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
                "    vec3 light_vector = normalize(u_light_pos - v_position);\n"
                "    float diffuse = max(dot(v_normal, light_vector), 0.1);\n"
                "    gl_FragColor = u_color * diffuse;\n"
                "}\n";

        const char *vertex_shader_prgm =
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

        std::mt19937 rand_gen;
        std::uniform_real_distribution<float> uniform_dist;

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

        std::vector<float> smooth_normal(
                const std::vector<float> &packed_data, int width, int height);
    };
}

#endif //PHYVR_NDK_HEIGHT_MAP_H
