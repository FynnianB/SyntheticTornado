#version 330
uniform mat4 mvpMatrix;
in vec4 vertexPosition;

out vec4 fColor;

void main()
{
    // Calculate vertex position in screen space.
    gl_Position = mvpMatrix * vertexPosition;

    fColor = vec4(vertexPosition.x,vertexPosition.y,vertexPosition.z,1);
}
