#version 410 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

// uniform vec3 triangleColor;

void main()
{
    FragColor = texture(texture1, TexCoord);
}