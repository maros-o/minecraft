#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 texAtlasCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    float x = texAtlasCoords.z, y = texAtlasCoords.y;

    if (aTexCoord.x == 0.0f) {
        x = texAtlasCoords.x;
    }

    if (aTexCoord.y == 1.0f) {
        y = texAtlasCoords.w;
    }

    TexCoord = vec2(x, y);
} 