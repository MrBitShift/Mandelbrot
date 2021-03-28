#version 460

in vec2 coord;

out vec2 Coord;

void main()
{
	Coord = coord;
	gl_Position = vec4(coord, 0.0, 1.0);
}