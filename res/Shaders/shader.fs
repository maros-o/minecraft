#version 460 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D textureAtlas;
uniform float brightness;

void main()
{
    FragColor = texture(textureAtlas, TexCoord) * vec4(brightness, brightness, brightness, 1.0f);
}