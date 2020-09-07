#include "engine.h"

#include <stdbool.h>
#include <stdlib.h>

void processInput(GLFWwindow* window, Camera* c, float *m);

#include "constants.h"

int
main(void) {
    GLFWwindow* window = app_init(SCR_WIDTH, SCR_HEIGHT);
    if (window == NULL) {
        return EXIT_FAILURE;
    }

    char* vrtSrc = app_readfile("assets/shader.vs");
    char* fragSrc = app_readfile("assets/shader.fs");
    int shaderProgram = gfx_shader_program(vrtSrc, fragSrc);
    if (shaderProgram == 0) {
        return EXIT_FAILURE;
    }
    free(vrtSrc);
    free(fragSrc);

    unsigned int texture1 = gfx_texture_load("assets/container.png");
    unsigned int texture2 = gfx_texture_load("assets/container_specular.png");
    if (texture1 == 0) {
        return EXIT_FAILURE;
    }
    if (texture2 == 0) {
        return EXIT_FAILURE;
    }

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
    glUseProgram(shaderProgram);

    GLint vp_location = glGetUniformLocation(shaderProgram, "view_projection");
    GLint m_location = glGetUniformLocation(shaderProgram, "model");
    GLint viewPos_location = glGetUniformLocation(shaderProgram, "viewPos");

    struct vec3 objectColor =  {1.0f, 0.5f, 0.31f};
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, (const GLfloat*)&objectColor);

    float shininess = 32.0f;
    glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);
    glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), shininess);

    struct vec3 la =  {0.2f, 0.2f, 0.2f};
    struct vec3 ld =  {0.5f, 0.5f, 0.5f};
    struct vec3 ls =  {1.0f, 1.0f, 1.0f};
    struct vec3 lp = {1.5f, 1.5f, 0.5f};
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.ambient"), 1, (const GLfloat*)&la);
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.diffuse"), 1, (const GLfloat*)&ld);
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.specular"), 1, (const GLfloat*)&ls);
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.position"), 1, (const GLfloat*)&lp);

    float_t m[MAT4_SIZE];
    float_t vp[MAT4_SIZE];
    mat4_identity(m);

    while (app_running(window)) {
        processInput(window, &c, m);

        // Clear the Screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render Begin
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(mesh.vao);

        gfx_camera_vp(vp, c);
        glUniformMatrix4fv(vp_location, 1, GL_FALSE, (const GLfloat*)&vp);
        glUniformMatrix4fv(m_location, 1, GL_FALSE, (const GLfloat*)&m);
        glUniform3fv(viewPos_location, 1, (const GLfloat*)&c.eye);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Render End

        app_swap_and_poll(window);
    }

    gfx_mesh_free(mesh);
    gfx_texture_destroy(texture1);
    gfx_texture_destroy(texture2);
    gfx_shader_destroy(shaderProgram);
    app_quit(window);

    return EXIT_SUCCESS;
}

void
processInput(GLFWwindow* window, Camera* c, float *m) {
    float angle = 1.0f;
    float_t r[MAT4_SIZE];
    mat4_identity(r);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        if (c->eye[2] > 0)
        { c->eye[2] -= 0.05; }
    } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        c->eye[2] += 0.05;
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        mat4_rotation_x(r, to_radians(-angle));
        mat4_multiply(m, m, r);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        mat4_rotation_x(r, to_radians(angle));
        mat4_multiply(m, m, r);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        mat4_rotation_y(r, to_radians(-angle));
        mat4_multiply(m, m, r);
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        mat4_rotation_y(r, to_radians(angle));
        mat4_multiply(m, m, r);
    }
}
