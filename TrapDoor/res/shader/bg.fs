#version 150 core

in vec3 Color;
in vec2 Texcoord;
out vec4 outColor;

uniform sampler2D texStone;
uniform sampler2D texMortyFront;

void main() {
	outColor = texture(texStone, Texcoord); 
}