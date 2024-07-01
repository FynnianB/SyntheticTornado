#version 330
smooth in vec2 fragTextureCoord;

layout(location = 0) out vec4 fragColor;

uniform sampler2D colorMappingTexture;

void main()
{
    fragColor = texture(colorMappingTexture, fragTextureCoord); // Maybe color animation, change of scala
}
