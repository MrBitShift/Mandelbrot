#version 460

in vec2 Coord;

void main()
{
	gl_FragColor = vec4((sin(Coord) + 1)/2, (cos(Coord.y + Coord.x) + 1)/2, 1.0);
}