#version 460

uniform float time;

in vec2 coord;

out vec2 Coord;

void main()
{
	Coord = vec2(coord.x - 0.5, coord.y) * 1.5;
	gl_Position = vec4(coord, 0.0, 1.0);
}