#include "engine.h"
#include <linmath.h>

#include <stdbool.h>
#include <stdlib.h>

void processInput(GLFWwindow* window);
float degreesToRadians(float angleDegrees);

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

    Mesh mesh = gfx_mesh_load(8, 12, vertices, indices, colors, textcoords);

    // load texture
    unsigned int texture1 = gfx_texture_load("wall.jpg");
    if (texture1 == 0) {
        return EXIT_FAILURE;
    }

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    //float ratio = SCR_WIDTH / SCR_HEIGHT;
    vec3 eye = {0.0f, 3.0f, 5.0f};
    vec3 origin = {0, 0, 0};
    vec3 up = {0.0f, 1.0f, 0.0};
    mat4x4 p, v, m;
    mat4x4_perspective(p, (float)degreesToRadians(45.0f), 1.0f, 0.1f, 100.0f);
    mat4x4_look_at(v, eye, origin, up);
    mat4x4_identity(m);

    GLint mvp_location = glGetUniformLocation(shaderProgram, "MVP");

    while (app_running(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(mesh.vao);

        mat4x4 vp, n, mvp;
        mat4x4_dup(n, m);
        mat4x4_rotate_Y(n, n, (float)glfwGetTime());
        mat4x4_rotate_X(n, n, (float)glfwGetTime());
        mat4x4_mul(vp, p, v);
        mat4x4_mul(mvp, vp, n);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        app_swap_and_poll(window);
    }

    //gfx_mesh_free(mesh);
    gfx_texture_destroy(texture1);
    gfx_shader_destroy(shaderProgram);
    app_quit(window);

    return EXIT_SUCCESS;
}

float
degreesToRadians(float angleDegrees) {
    return angleDegrees * PI / 180.0;
}

void
processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    { glfwSetWindowShouldClose(window, true); }
}
