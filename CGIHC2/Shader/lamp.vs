#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
        FragPos = vec3(model * vec4(position, 1.0f));
    TexCoords = texCoords;
}