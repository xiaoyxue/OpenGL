#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = texCoord;
	gl_Position = position;
}


#shader fragment
#version 330 core

in vec2 v_TexCoord;
layout(location = 0) out vec4 color;

#define PI 			3.14159265359

#define FOV 		60.0
#define EPSILON		0.001
#define MAX_STEPS	1000
#define MAX_DIST	1000.0

#define PLANE		vec4 (0.0, 1.0, 0.0, 10.0)
//#define PLANE		vec4 (0.0, 1.0, 0.0, 15.0)
#define BOTTOM		vec4 (0.0, 1.0, 0.0, 4.0)


uniform sampler2D backgroundTexture;
uniform sampler2D noiseTexture;
uniform vec2 iResolution;
uniform float iTime;

vec4 noise3v(vec2 p) {
	return texture (noiseTexture, p);
}

vec4 fbm3v(vec2 p) {
	vec4 f = vec4(0.0);
	f += (vec4(0.5000) * noise3v(p)); p *= vec2(2.01);
	f += (vec4(0.2500) * noise3v(p)); p *= vec2(2.02);
	f += (vec4(0.1250) * noise3v(p)); p *= vec2(2.03);
	f += (vec4(0.0625) * noise3v(p)); p *= vec2(2.04);
	f /= vec4(0.9375);
	return f;
}

float dplane(vec3 pt, vec4 pl) {
	return dot(pl.xyz, pt) + pl.w;
}

float map(vec3 pt) {
	return dplane(pt, PLANE);
}

float map2(vec3 pt) {
	return dplane(pt, BOTTOM);
}

float march(vec3 ro, vec3 rd) {
	float t = 0.0;
	float d = 0.0;
	vec3 pt = vec3(0.0);
	for (int i = 0; i < MAX_STEPS; ++i) {
		pt = ro + rd * t;
		d = map(pt);
		if (d < EPSILON || t + d >= MAX_DIST) {
			break;
		}
		t += d;
	}

	return d <= EPSILON ? t : MAX_DIST;
}

float march2(vec3 ro, vec3 rd) {
	float t = 0.0;
	float d = 0.0;
	vec3 pt = vec3(0.0);
	for (int i = 0; i < MAX_STEPS; ++i) {
		pt = ro + rd * t;
		d = map2(pt);
		if (d < EPSILON || t + d >= MAX_DIST) {
			break;
		}
		t += d;
	}

	return d <= EPSILON ? t : MAX_DIST;
}

float fresnel_step(vec3 I, vec3 N, vec3 f) {
	return clamp(f.x + f.y * pow(1.0 + dot(I, N), f.z), 0.0, 1.0);
}


void main() {

	vec2 uv = (2.0 * gl_FragCoord.xy - iResolution.xy) / min(iResolution.x, iResolution.y) * tan(radians(FOV) / 2.0);

	vec2 st = gl_FragCoord.xy / iResolution.xy;

	vec3 up = vec3(0.0, 1.0, 0.0);			// up 
	vec3 fw = vec3(0.0, 0.0, 1.0);			// forward
	vec3 lf = cross(up, fw); 					// left

	vec3 ro = -fw * 5.0 + vec3(0.0, 5.0, 0.0); // ray origin
	vec3 rd = normalize(uv.x * lf + uv.y * up + fw); 		// ray direction

	float t = march(ro, rd);
	vec4 cm = texture(backgroundTexture, st);
	vec3 pt = rd * t + ro;
	vec3 pn = PLANE.xyz;
	vec3 dv = fbm3v(pt.xz / 512.0 + iTime / 256.0).xyz - 0.5;
	pn = normalize(pn + dv * 0.2);
	vec3 rfl = reflect(rd, pn);
	float fs = fresnel_step(rd, pn, vec3(0.0, 3.0, 6.0));
	vec4 c0 = vec4(0, 0, 0, 1);
	vec4 c1 = texture(backgroundTexture, st + 0.05 * dv.xy);

	if (t < 1000.0) {
		color = mix(c1, cm, smoothstep(80.0, 160.0, t));

	}
	else {
		color = texture(backgroundTexture, st);
	}


}