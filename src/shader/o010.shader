#SHADER: VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    //gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
};

#SHADER: FRAGMENT
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
uniform vec4 ObjectColor;
uniform vec4 LightColor;

void main()
{   
   FragColor = vec4(LightColor * ObjectColor);
};