#include "skybox.h"

float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

SkyBox skybox_init(unsigned int shader, unsigned int cubemap) {
    SkyBox skybox;
    skybox.shader = shader;
    skybox.cubemap = cubemap;

    glGenVertexArrays(1, &skybox.vao);
    glGenBuffers(1, &skybox.vbo);
    glBindVertexArray(skybox.vao);
    glBindBuffer(GL_ARRAY_BUFFER, skybox.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    skybox.location.skybox = glGetUniformLocation(shader, "skybox");
    skybox.location.view = glGetUniformLocation(shader, "view");
    skybox.location.projection = glGetUniformLocation(shader, "projection");

    return skybox;
}

void skybox_draw(SkyBox skybox, mat4 view, mat4 projection) {
    // draw skybox as last
    GLint OldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    
    mat3 _view3x3;
    mat4 _view;
    glm_mat4_zero(_view);
    glm_mat4_pick3(view, _view3x3);
    glm_mat4_ins3(_view3x3, _view);
    
    glUseProgram(skybox.shader);
    glUniform1i(skybox.location.skybox, 0);
    glUniformMatrix4fv(skybox.location.view, 1, GL_FALSE, (const GLfloat *)_view);
    glUniformMatrix4fv(skybox.location.projection, 1, GL_FALSE,
                     (const GLfloat *)projection);
    // skybox cube
    glBindVertexArray(skybox.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemap);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(OldDepthFuncMode); // set depth function back to default
}