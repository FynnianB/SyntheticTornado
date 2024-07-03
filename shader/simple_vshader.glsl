#version 330
layout(location = 0) in vec4 vertexPosition;
uniform mat4 mvpMatrix;

void main()
{
    gl_Position = mvpMatrix * vec4(1-vertexPosition.x, vertexPosition.y,1-vertexPosition.z,vertexPosition.w);
}
