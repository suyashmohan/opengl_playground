#include "engine.h"

#include <stdbool.h>
#include <stdlib.h>

void processInput(GLFWwindow* window, Camera* c, float *m);
void draw(GLuint shaderProgram, GLuint texture1, GLuint texture2, Mesh mesh, Camera c, float *m);

#include "constants.h"

int
main(void) {
    GLFWwindow* window = app_init(SCR_WIDTH, SCR_HEIGHT);
    if (window == NULL) {
        return EXIT_FAILURE;
    }

    Material mat= gfx_material_create(
            "assets/shader.vs", "assets/shader.fs",
            2, "assets/container.png", "assets/container_specular.png");

    Mesh mesh = gfx_mesh_load(36, vertices, normals, textcoords);
    Camera c = {
        {0.0f, 0.0f, 5.0f},
        {0, 0, 0},
        {0.0f, 1.0f, 0.0},
        45.0f,
        0.1,
        100.0f,
        SCR_WIDTH,
        SCR_HEIGHT,
    };

    // Prepare Shader
    glUseProgram(mat.shader);

    float shininess = 32.0f;
    glUniform1i(glGetUniformLocation(mat.shader, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(mat.shader, "material.specular"), 1);
    glUniform1f(glGetUniformLocation(mat.shader, "material.shininess"), shininess);

    float la[VEC3_SIZE] = {0.2f, 0.2f, 0.2f};
    float ld[VEC3_SIZE] = {0.5f, 0.5f, 0.5f};
    float ls[VEC3_SIZE] = {1.0f, 1.0f, 1.0f};
    float ldir[VEC3_SIZE] = {-0.2f, -1.0f, -0.3f};
    glUniform3fv(glGetUniformLocation(mat.shader, "light.ambient"), 1, (const GLfloat*)&la);
    glUniform3fv(glGetUniformLocation(mat.shader, "light.diffuse"), 1, (const GLfloat*)&ld);
    glUniform3fv(glGetUniformLocation(mat.shader, "light.specular"), 1, (const GLfloat*)&ls);
    glUniform3fv(glGetUniformLocation(mat.shader, "light.direction"), 1, (const GLfloat*)&ldir);

    float m[MAT4_SIZE];
    float m1[MAT4_SIZE];
    float m2[MAT4_SIZE];
    float m3[MAT4_SIZE];

    mat4_identity(m);
    mat4_identity(m1);
    mat4_identity(m2);
    mat4_identity(m3);

    float v1[VEC3_SIZE] = {2.0f, 0.0f, -1.0f};
    mat4_translate(m1, m1, v1);

    float v2[VEC3_SIZE] = {1.0f, -2.0f, 1.0f};
    mat4_translate(m2, m2, v2);

    float v3[VEC3_SIZE] = {-2.0f, 1.0f, -2.0f};
    mat4_translate(m3, m3, v3);

    float r[MAT4_SIZE];
    mat4_identity(r);

    while (app_running(window)) {
        processInput(window, &c, r);
        mat4_multiply(m, m, r);

        // Clear the Screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render Begin
        draw(mat.shader, mat.textures[0], mat.textures[1], mesh,  c, m);
        draw(mat.shader, mat.textures[0], mat.textures[1], mesh,  c, m1);
        draw(mat.shader, mat.textures[0], mat.textures[1], mesh,  c, m2);
        draw(mat.shader, mat.textures[0], mat.textures[1], mesh,  c, m3);
        // Render End

        app_swap_and_poll(window);
    }

    gfx_material_destroy(mat);
    gfx_mesh_free(mesh);
    app_quit(window);

    return EXIT_SUCCESS;
}

void
draw(GLuint shaderProgram, GLuint texture1, GLuint texture2, Mesh mesh,  Camera c, float *m) {
    GLint v_location = glGetUniformLocation(shaderProgram, "view");
    GLint p_location = glGetUniformLocation(shaderProgram, "projection");
    GLint m_location = glGetUniformLocation(shaderProgram, "model");
    GLint viewPos_location = glGetUniformLocation(shaderProgram, "viewPos");

    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBindVertexArray(mesh.vao);

    float v[MAT4_SIZE];
    float p[MAT4_SIZE];
    gfx_camera_vp(v, p, c);
    glUniformMatrix4fv(v_location, 1, GL_FALSE, (const GLfloat*)&v);
    glUniformMatrix4fv(p_location, 1, GL_FALSE, (const GLfloat*)&p);
    glUniformMatrix4fv(m_location, 1, GL_FALSE, (const GLfloat*)m);
    glUniform3fv(viewPos_location, 1, (const GLfloat*)&c.position);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void
processInput(GLFWwindow* window, Camera* c, float *r) {
    float angle = 1.0f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        if (c->position[2] > 0)
        { c->position[2] -= 0.05; }
    } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        c->position[2] += 0.05;
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        mat4_rotation_x(r, to_radians(-angle));
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        mat4_rotation_x(r, to_radians(angle));
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        mat4_rotation_y(r, to_radians(-angle));
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        mat4_rotation_y(r, to_radians(angle));
    }
}
