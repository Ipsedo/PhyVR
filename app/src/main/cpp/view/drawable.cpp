//
// Created by samuel on 08/09/20.
//

#include "drawable.h"

#include "../utils.h"

#include <SOIL2.h>
#include <glm/gtc/type_ptr.hpp>

phyvr_view::ObjDrawable::ObjDrawable(
        AAssetManager *a_asset_manager,
        const std::string &obj_file_name,
        const std::string &texture_file) :
        texture_tex(new GLuint{0}) {

    /*
     * Init OpenGL program
     */

    m_program = glCreateProgram();

    vertex_shader = load_shader(
            GL_VERTEX_SHADER,
            vertex_shader_prgm
    );

    fragment_shader = load_shader(
            GL_FRAGMENT_SHADER,
            fragment_shader_prgm
    );

    glAttachShader(m_program, vertex_shader);
    glAttachShader(m_program, fragment_shader);
    glLinkProgram(m_program);

    /*
     * Bind shader var
     */

    m_mvp_handle = (GLuint) glGetUniformLocation(m_program, "u_mvp_matrix");
    m_mv_handle = (GLuint) glGetUniformLocation(m_program, "u_mv_matrix");

    m_position_handle = (GLuint) glGetAttribLocation(m_program, "a_position");
    m_tex_coord_handle = (GLuint) glGetAttribLocation(m_program, "a_tex_coord");
    m_normal_handle = (GLuint) glGetAttribLocation(m_program, "a_normal");

    m_light_pos_handle = (GLuint) glGetUniformLocation(m_program, "u_light_pos");

    m_tex_handle = (GLuint) glGetUniformLocation(m_program, "u_tex");

    /*
     * Load textures
     */

    int width, height, channels;

    AAsset *file = AAssetManager_open(a_asset_manager, texture_file.c_str(), AASSET_MODE_BUFFER);

    auto length = AAsset_getLength(file);
    auto img_buffer = AAsset_getBuffer(file);

    texture_pixels = SOIL_load_image_from_memory(
            (const unsigned char *) img_buffer, length,
            &width, &height, &channels,
            SOIL_LOAD_RGB
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_tex[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 texture_pixels);

    /*
     * Load OBJ model
     */

    nb_vertex = 0;

    std::string text = phyvr_utils::get_file_content(a_asset_manager, obj_file_name);
    std::vector<std::string> lines = phyvr_utils::split(text, '\n');

    std::vector<float> vertex_list;
    std::vector<float> normal_list;
    std::vector<float> uv_list;
    std::vector<int> vertex_draw_order;
    std::vector<int> normal_draw_order;
    std::vector<int> uv_draw_order;

    for (const auto &str : lines) {
        //cout << str << endl;
        std::vector<std::string> splitted_line = phyvr_utils::split(str, ' ');
        if (!splitted_line.empty()) {
            if (splitted_line[0] == "vn") {
                normal_list.push_back(stof(splitted_line[1]));
                normal_list.push_back(stof(splitted_line[2]));
                normal_list.push_back(stof(splitted_line[3]));
            } else if (splitted_line[0] == "vt") {
                uv_list.push_back(stof(splitted_line[1]));
                uv_list.push_back(stof(splitted_line[2]));
            } else if (splitted_line[0] == "v") {
                vertex_list.push_back(stof(splitted_line[1]));
                vertex_list.push_back(stof(splitted_line[2]));
                vertex_list.push_back(stof(splitted_line[3]));
            } else if (splitted_line[0] == "f") {
                std::vector<std::string> v1 = phyvr_utils::split(splitted_line[1], '/');
                std::vector<std::string> v2 = phyvr_utils::split(splitted_line[2], '/');
                std::vector<std::string> v3 = phyvr_utils::split(splitted_line[3], '/');

                vertex_draw_order.push_back(stoi(v1[0]));
                vertex_draw_order.push_back(stoi(v2[0]));
                vertex_draw_order.push_back(stoi(v3[0]));

                uv_draw_order.push_back(stoi(v1[1]));
                uv_draw_order.push_back(stoi(v2[1]));
                uv_draw_order.push_back(stoi(v3[1]));

                normal_draw_order.push_back(stoi(v1[2]));
                normal_draw_order.push_back(stoi(v2[2]));
                normal_draw_order.push_back(stoi(v3[2]));

                v1.clear();
                v2.clear();
                v3.clear();
            }
        }
        splitted_line.clear();
    }
    lines.clear();

    std::vector<float> packed_data;
    for (int i = 0; i < vertex_draw_order.size(); i++) {
        packed_data.push_back(vertex_list[(vertex_draw_order[i] - 1) * 3]);
        packed_data.push_back(vertex_list[(vertex_draw_order[i] - 1) * 3 + 1]);
        packed_data.push_back(vertex_list[(vertex_draw_order[i] - 1) * 3 + 2]);

        packed_data.push_back(normal_list[(normal_draw_order[i] - 1) * 3]);
        packed_data.push_back(normal_list[(normal_draw_order[i] - 1) * 3 + 1]);
        packed_data.push_back(normal_list[(normal_draw_order[i] - 1) * 3 + 2]);

        packed_data.push_back(uv_list[(uv_draw_order[i] - 1) * 2]);
        packed_data.push_back(uv_list[(uv_draw_order[i] - 1) * 2 + 1]);

        nb_vertex++;
    }

    vertex_list.clear();
    vertex_draw_order.clear();
    normal_list.clear();
    normal_draw_order.clear();
    uv_list.clear();
    uv_draw_order.clear();

    /*
     * Gen buffer
     */

    buffer = 0;
    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(
            GL_ARRAY_BUFFER,
            packed_data.size() * BYTES_PER_FLOAT,
            packed_data.data(),
            GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

phyvr_view::ObjDrawable::~ObjDrawable() {
    glDeleteTextures(1, texture_tex);

    glDeleteBuffers(1, &buffer);

    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteProgram(m_program);

    SOIL_free_image_data(texture_pixels);
}

void phyvr_view::ObjDrawable::draw(phyvr_view::gl_infos infos) {
    glm::mat4 mv_matrix = infos.view_matrix * infos.model_matrix;
    glm::mat4 mvp_matrix = infos.projection_matrix * mv_matrix;

    glUseProgram(m_program);
    check_gl_error("create_prgm");

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(m_position_handle);
    glVertexAttribPointer(
            m_position_handle, POSITION_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, (char *) nullptr);

    check_gl_error("pos_buffer");

    glEnableVertexAttribArray(m_normal_handle);
    glVertexAttribPointer(
            m_normal_handle, NORMAL_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, (char *) nullptr + POSITION_SIZE * BYTES_PER_FLOAT
    );

    check_gl_error("normal_buffer");

    glEnableVertexAttribArray(m_tex_coord_handle);
    glVertexAttribPointer(
            m_tex_coord_handle, TEX_COORD_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, (char *) nullptr + (POSITION_SIZE + NORMAL_SIZE) * BYTES_PER_FLOAT
    );

    check_gl_error("tex_buffer");

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(m_mv_handle, 1, GL_FALSE, glm::value_ptr(mv_matrix));

    check_gl_error("mv_handle");

    glUniformMatrix4fv(m_mvp_handle, 1, GL_FALSE, glm::value_ptr(mvp_matrix));

    check_gl_error("mvp_handle");

    glUniform3fv(m_light_pos_handle, 1, glm::value_ptr(infos.light_pos));

    check_gl_error("light_handle");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_tex[0]);
    glUniform1i(m_tex_handle, 0);

    check_gl_error("texture");

    glDrawArrays(GL_TRIANGLES, 0, nb_vertex);

    check_gl_error("draw");

    glDisableVertexAttribArray(m_position_handle);
    glDisableVertexAttribArray(m_tex_coord_handle);
    glDisableVertexAttribArray(m_normal_handle);

    check_gl_error("end_draw");
}
