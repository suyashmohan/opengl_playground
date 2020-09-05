#ifndef CONSTANTS_H
#define CONSTANTS_H

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourNormal;\n"
    "out vec2 TexCoord;\n"
    "out vec3 FragPos;\n"
    "uniform mat4 VP;\n"
    "uniform mat4 M;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = VP * M * vec4(aPos, 1.0);\n"
//    "   ourNormal = aNormal;\n"
    "   ourNormal = mat3(transpose(inverse(M))) * aNormal;\n"
    "   TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "   FragPos = vec3(M * vec4(aPos, 1.0));"
    "}\n\0";

const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourNormal;\n"
    "in vec2 TexCoord;\n"
    "in vec3 FragPos;\n"
    "uniform sampler2D texture1;\n"
    "uniform vec3 objectColor;\n"
    "uniform vec3 lightColor;\n"
    "uniform vec3 lightPos;\n"
    "uniform vec3 viewPos;\n"
    "void main()\n"
    "{\n"
    "   float ambientStrength  = 1.0;\n"
    "   vec3 ambient = ambientStrength * lightColor;\n"
    ""
    "   vec3 norm = normalize(ourNormal);\n"
    "   vec3 lightDir = normalize(lightPos - FragPos); \n"
    "   float diff = max(dot(norm, lightDir), 0.0);\n"
    "   vec3 diffuse = diff * lightColor;\n"
    ""
    "   float specularStrength = 0.5;\n"
    "   vec3 viewDir = normalize(viewPos - FragPos);\n"
    "   vec3 reflectDir = reflect(-lightDir, norm);\n"
    "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
    "   vec3 specular = specularStrength * spec * lightColor;\n"
    ""
    "   vec3 result = (ambient + diffuse + specular) * objectColor;\n"
    "   FragColor = vec4(result, 1.0);\n"
//    "   FragColor = texture(texture1, TexCoord);\n"
    "}\n\0";

float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
};

const float normals[] =  {
    0.0f,  0.0f, -1.0f,
  0.0f,  0.0f, -1.0f, 
  0.0f,  0.0f, -1.0f, 
  0.0f,  0.0f, -1.0f, 
  0.0f,  0.0f, -1.0f, 
  0.0f,  0.0f, -1.0f, 
  0.0f,  0.0f, 1.0f,
  0.0f,  0.0f, 1.0f,
  0.0f,  0.0f, 1.0f,
  0.0f,  0.0f, 1.0f,
  0.0f,  0.0f, 1.0f,
  0.0f,  0.0f, 1.0f,
   -1.0f,  0.0f,  0.0f,
 -1.0f,  0.0f,  0.0f,
 -1.0f,  0.0f,  0.0f,
 -1.0f,  0.0f,  0.0f,
 -1.0f,  0.0f,  0.0f,
 -1.0f,  0.0f,  0.0f,
   1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
  0.0f, -1.0f,  0.0f,
  0.0f, -1.0f,  0.0f,
  0.0f, -1.0f,  0.0f,
  0.0f, -1.0f,  0.0f,
  0.0f, -1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f
};

// textures
const float textcoords[] = {
0.0f, 0.0f,
1.0f, 0.0f,
1.0f, 1.0f,
1.0f, 1.0f,
0.0f, 1.0f,
0.0f, 0.0f,

0.0f, 0.0f,
1.0f, 0.0f,
1.0f, 1.0f,
1.0f, 1.0f,
0.0f, 1.0f,
0.0f, 0.0f,

1.0f, 0.0f,
1.0f, 1.0f,
0.0f, 1.0f,
0.0f, 1.0f,
0.0f, 0.0f,
1.0f, 0.0f,

1.0f, 0.0f,
1.0f, 1.0f,
0.0f, 1.0f,
0.0f, 1.0f,
0.0f, 0.0f,
1.0f, 0.0f,

0.0f, 1.0f,
1.0f, 1.0f,
1.0f, 0.0f,
1.0f, 0.0f,
0.0f, 0.0f,
0.0f, 1.0f,

0.0f, 1.0f,
1.0f, 1.0f,
1.0f, 0.0f,
1.0f, 0.0f,
0.0f, 0.0f,
0.0f, 1.0f
};

#endif
