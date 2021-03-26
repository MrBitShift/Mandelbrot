#version 460

in vec2 Coord;

void main()
{
	gl_FragColor = vec4(sin(Coord), 1.0, 1.0);
}