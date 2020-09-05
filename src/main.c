#include "engine.h"

#include <stdbool.h>
#include <stdlib.h>

void processInput(GLFWwindow* window, Camera *c);

#include "constants.h"

int
main(void) {
    GLFWwindow* window = app_init(SCR_WIDTH, SCR_HEIGHT);
    if (window == NULL) {
        return EXIT_FAILURE;
    }
    int shaderProgram = gfx_shader_program(vertexShaderSource, fragmentShaderSource);
    if (shaderProgram == 0) {
        return EXIT_FAILURE;
    }
    unsigned int texture1 = gfx_texture_load("wall.jpg");
    if (texture1 == 0) {
        return EXIT_FAILURE;
    }

    Mesh mesh = gfx_mesh_load(8, 12, vertices, indices, colors, textcoords);
    Camera c = {
        {0.0f, 0.0f, 5.0f},
        {0, 0, 0},
        {0.0f, 1.0f, 0.0},
        45.0f,
        0.1,
        100.0f,
        (float)(SCR_WIDTH / SCR_HEIGHT),
    };

    // Prepare Shader
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    GLint vp_location = glGetUniformLocation(shaderProgram, "VP");
    GLint m_location = glGetUniformLocation(shaderProgram, "M");

    while (app_running(window)) {
        processInput(window, &c);

        // Clear the Screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render Begin
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(mesh.vao);

        mat4x4 m, vp;
        mat4x4_identity(m);
        mat4x4_rotate_Y(m, m, (float)glfwGetTime());
        mat4x4_rotate_X(m, m, (float)glfwGetTime());
        mat4x4_rotate_Z(m, m, (float)glfwGetTime());
        gfx_camera_vp(vp, c);
        glUniformMatrix4fv(vp_location, 1, GL_FALSE, (const GLfloat*)vp);
        glUniformMatrix4fv(m_location, 1, GL_FALSE, (const GLfloat*)m);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        // Render End

        app_swap_and_poll(window);
    }

    gfx_mesh_free(mesh);
    gfx_texture_destroy(texture1);
    gfx_shader_destroy(shaderProgram);
    app_quit(window);

    return EXIT_SUCCESS;
}

void
processInput(GLFWwindow* window, Camera *c) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { 
        glfwSetWindowShouldClose(window, true); 
    }
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        if (c->eye[2] > 0)
            c->eye[2] -= 0.1;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        c->eye[2] += 0.1;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        c->lookat[1] -= 0.1;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        c->lookat[1] += 0.1;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        c->lookat[0] -= 0.1;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        c->lookat[0] += 0.1;
    }
}
