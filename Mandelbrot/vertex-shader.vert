#version 460

in vec2 coord;
uniform vec2 center;
uniform float scale;

out vec2 Coord;

void main()
{
	Coord = center + coord * scale;
	gl_Position = vec4(coord, 0.0, 1.0);
}