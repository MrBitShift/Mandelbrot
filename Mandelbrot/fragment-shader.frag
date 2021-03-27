#version 460

uniform int maxRepititions;
in vec2 Coord;

out vec4 outColor;

float isMandelbrot()
{
	float x = 0;
	float yTmp = 0;
	float y = 0;
	int i;
	for (i = 0; i < maxRepititions; i++)
	{
		yTmp = 2*x*y + Coord.y;
		x = x*x - y*y + Coord.x;
		y = yTmp;
		if (x*x + y*y >= 40)
			break;
	}
	return i;
}

void main()
{
	float repititions = isMandelbrot();
	float clamped = clamp((40 - repititions) / 40, 0.0, 1.0);
	outColor = vec4(clamped.xxx, 1.0);
}