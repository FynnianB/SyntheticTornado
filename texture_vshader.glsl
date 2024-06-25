#version 330
layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec2 textureCoord;

smooth out vec2 fragTextureCoord;

uniform mat4 mvpMatrix;

void main()
{
    gl_Position = mvpMatrix * vertexPosition;
    fragTextureCoord = textureCoord;
}
