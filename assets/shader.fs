#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    vec3 diffuseColor;
    vec3 specularColor;
    int useDiffuseTexture;
    int useSpecularTexture;
};
struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    vec3 ambient, diffuse, specular;
    if (material.useDiffuseTexture > 0) {
        ambient =  light.ambient * vec3(texture(material.diffuse, TexCoord));
    } else {
        ambient =  light.ambient *material.diffuseColor;
    }
    

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction); 
    float diff = max(dot(norm, lightDir), 0.0);
    if (material.useDiffuseTexture > 0) {
        diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    } else {
        diffuse = light.diffuse * diff * material.diffuseColor;
    }

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    if (material.useSpecularTexture > 0) {
        specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    } else {
        specular = light.specular * spec * material.specularColor;
    }

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}