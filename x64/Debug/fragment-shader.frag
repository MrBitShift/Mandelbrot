#version 460

uniform int maxRepititions;
uniform dvec2 center;
uniform double scale;
in vec2 Coord;

dvec2 dCoord;

out vec4 outColor;

vec3 LazyHSV(float huePrime)
{
	float x = 1 - abs(mod(huePrime, 2) - 1);
	if (huePrime <= 1)
	{
		return vec3(1, x, 0);
	}
	if (huePrime <= 2)
	{
		return vec3(x, 1, 0);
	}
	if (huePrime <= 3)
	{
		return vec3(0, 1, x);
	}
	if (huePrime <= 4)
	{
		return vec3(0, x, 1);
	}
	if (huePrime <= 5)
	{
		return vec3(x, 0, 1);
	}
	return vec3(1, 0, x);
}

float MandelbrotTest()
{
	double x = 0;
	double yTmp = 0;
	double y = 0;
	int i;
	for (i = 0; i < maxRepititions; i++)
	{
		yTmp = 2*x*y + dCoord.y;
		x = x*x - y*y + dCoord.x;
		y = yTmp;
		if (x*x + y*y >= 4)
			break;
	}
	return i;
}

void main()
{
	dCoord = dvec2(dvec2(center) + dvec2(Coord) * double(scale));
	float repititions = MandelbrotTest();
	float grad = 6 * (maxRepititions - repititions) / maxRepititions;
	outColor = vec4(grad == 0.0 ? grad.xxx : LazyHSV(grad), 1.0);
}