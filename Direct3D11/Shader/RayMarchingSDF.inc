#include "BufferStructure.inc"
#include "Noise.inc"

float SDF_Sphere(float3 p)
{
	return length(p) - 1;
}



// Implicit Maze-Like Patterns - Fizzer
// http://amietia.com/slashmaze.html
float SDF_Maze(float3 p)
{
   float3 cp = frac(p) - .5;
   float3 acp = abs(cp);
   float3 ofs = step(acp.yzx, acp.xyz) * step(acp.zxy, acp.xyz) * sign(cp);
   float3 op = floor(p) + .5 + ofs * .5;
   float f = snoise(op);
   float2 cp2 = abs(f > 1. / 3. ? f > 2. / 3. ? cp.xz : cp.yz : cp.xy);
   return max(cp2.x, cp2.y) - .1;
}

//https://github.com/JPBotelho/Raymarched-Fractals/blob/master/Content/DistanceFunc.cginc
float SDF_Mandelbulb(float3 p)
{
	float power = Time/10 + 1;

	 float3 z = p;
    float r = 0;
    float dr = 1;
    for(int i = 0; i < 5; i++) 
    {
        r = length(z);
        if(r > 100) break;
        
        float theta = acos(z.z / r);
        float phi = atan2(z.y, z.x);
        
        dr = power * pow(r, power-1)*dr+1;
        
        r = pow(r, power);
        theta *= power;
        phi *= power;
        
        z = r * float3(sin(theta) * cos(phi), 
                sin(theta) * sin(phi), 
                cos(theta));

        z += p;
    }
    return 0.5 * log(r) * r / dr;
}

//https://github.com/JPBotelho/Raymarched-Fractals/blob/master/Content/DistanceFunc.cginc
float SDF_Sierpinski(float3 p)
{
	const float3 va = float3(0.0, 0.575735, 0.0);
	const float3 vb = float3(0.0, -1.0, 1.15470);
	const float3 vc = float3(1.0, -1.0, -0.57735);
	const float3 vd = float3(-1.0, -1.0, -0.57735);

	float a = 0;
	float s = 1;
	float r = 1;
	float dm;
	float3 v;
	int iter = 15;

	[loop]
	for (int i = 0; i < iter; i++)
	{
		float d, t;
		d = dot(p - va, p - va);

		v = va;
		dm = d;
		t = 0;

		d = dot(p - vb, p - vb);
		if (d < dm)
		{
			v = vb;
			dm = d;
			t = 1.0;
		}

		d = dot(p - vc, p - vc);

		if (d < dm) { v = vc; dm = d; t = 2.0; }
		d = dot(p - vd, p - vd);
		if (d < dm) { v = vd; dm = d; t = 3.0; }

		p = v + 2 * (p - v);
		r *= 2;
		a = t + 4 * a;
		s *= 4;
	}
	//Original v. (But it includes unnecessary calculations and truncation)
	//return float2((sqrt(dm)-1.0)/r, a/s);

	return (sqrt(dm) - 1.0) / r;
}

//https://www.shadertoy.com/view/XtcGWn
float SDF_Cube(float3 p)
{
	// If d.x < 0, then -1 < p.x < 1, and same logic applies to p.y, p.z
	// So if all components of d are negative, then p is inside the unit cube
	float3 d = abs(p) - float3(1.0, 1.0, 1.0);

	// Assuming p is inside the cube, how far is it from the surface?
	// Result will be negative or zero.
	float insideDistance = min(max(d.x, max(d.y, d.z)), 0.0);

	// Assuming p is outside the cube, how far is it from the surface?
	// Result will be positive or zero.
	float outsideDistance = length(max(d, 0.0));

	return insideDistance + outsideDistance;
}