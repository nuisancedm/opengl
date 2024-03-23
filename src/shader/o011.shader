#SHADER: VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; //@@ it is very expensive to do this in the shader.
};

#SHADER: FRAGMENT
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec4 objectColor; //rgba
uniform vec4 lightColor;  //rgba

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{  
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(lightColor.x, lightColor.y, lightColor.z);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(lightColor.x, lightColor.y, lightColor.z);

    float specularStrength = 0.7;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(lightColor.x, lightColor.y, lightColor.z);

    vec3 result = (ambient + diffuse + specular) * vec3(objectColor.x, objectColor.y, objectColor.z);

    FragColor = vec4(result, 1.0);
};