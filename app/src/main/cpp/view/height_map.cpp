//
// Created by samuel on 20/09/20.
//

#include "height_map.h"

#include <glm/gtc/type_ptr.hpp>
#include <android/log.h>

phyvr_view::TriangleCallback::TriangleCallback() : nb_vertex(0), packed_data() {

}

void
phyvr_view::TriangleCallback::processTriangle(btVector3 *triangle, int partid, int triangleindex) {
    glm::vec3 p1 = glm::vec3(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
    glm::vec3 p2 = glm::vec3(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
    glm::vec3 p3 = glm::vec3(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());

    glm::vec3 n1 = glm::cross(p1 - p2, p3 - p2);
    glm::vec3 n2 = glm::cross(p3 - p2, p1 - p2);

    // y : up axis
    glm::vec3 n = n1.y > 0 ? n1 : n2;

    packed_data.push_back(p1.x);
    packed_data.push_back(p1.y);
    packed_data.push_back(p1.z);
    packed_data.push_back(n.x);
    packed_data.push_back(n.y);
    packed_data.push_back(n.z);

    packed_data.push_back(p2.x);
    packed_data.push_back(p2.y);
    packed_data.push_back(p2.z);
    packed_data.push_back(n.x);
    packed_data.push_back(n.y);
    packed_data.push_back(n.z);

    packed_data.push_back(p3.x);
    packed_data.push_back(p3.y);
    packed_data.push_back(p3.z);
    packed_data.push_back(n.x);
    packed_data.push_back(n.y);
    packed_data.push_back(n.z);

    nb_vertex += 3;
}

phyvr_view::MapDrawable::MapDrawable(btHeightfieldTerrainShape *terrain) :
        rand_gen(std::chrono::system_clock::now().time_since_epoch().count()),
        uniform_dist(0.f, 1.f),
        nb_vertex(0), color{1.f, 0.f, 0.f, 1.f} {

    std::vector<float> res;
    TriangleCallback callback;
    // TODO limits aabb
    terrain->processAllTriangles(&callback, btVector3(-1000, -1000, -1000),
                                 btVector3(1000, 1000, 1000));
    nb_vertex = callback.nb_vertex;

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
    m_normal_handle = (GLuint) glGetAttribLocation(m_program, "a_normal");

    m_light_pos_handle = (GLuint) glGetUniformLocation(m_program, "u_light_pos");

    m_color_handle = (GLuint) glGetUniformLocation(m_program, "u_color");

    buffer = 0;
    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(
            GL_ARRAY_BUFFER,
            callback.packed_data.size() * BYTES_PER_FLOAT,
            callback.packed_data.data(),
            GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    for (int i = 0; i < 3; i++)
        color[i] = 0.3f + 0.7f * uniform_dist(rand_gen);
    color[3] = 1.f;

}

phyvr_view::MapDrawable::~MapDrawable() {
    glDeleteBuffers(1, &buffer);

    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteProgram(m_program);
}

void phyvr_view::MapDrawable::draw(phyvr_view::gl_infos infos) {
    glm::mat4 mv_matrix = infos.view_matrix * infos.model_matrix;
    glm::mat4 mvp_matrix = infos.projection_matrix * mv_matrix;

    glUseProgram(m_program);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(m_position_handle);
    glVertexAttribPointer(
            m_position_handle, POSITION_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, (char *) nullptr);


    glEnableVertexAttribArray(m_normal_handle);
    glVertexAttribPointer(
            m_normal_handle, NORMAL_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, (char *) nullptr + POSITION_SIZE * BYTES_PER_FLOAT
    );


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(m_mv_handle, 1, GL_FALSE, glm::value_ptr(mv_matrix));


    glUniformMatrix4fv(m_mvp_handle, 1, GL_FALSE, glm::value_ptr(mvp_matrix));


    glUniform3fv(m_light_pos_handle, 1, glm::value_ptr(infos.light_pos));

    glUniform4fv(m_color_handle, 1, color);

    glDrawArrays(GL_TRIANGLES, 0, nb_vertex);


    glDisableVertexAttribArray(m_position_handle);
    glDisableVertexAttribArray(m_normal_handle);
}
