#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec2 v_TexCoord;
out float zValue;
uniform float iTime;

const mat2 m = mat2(1.6, 1.2, -1.2, 1.6);

vec2 hash(vec2 p) {
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

float noise(in vec2 p) {
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;
    vec2 i = floor(p + (p.x + p.y) * K1);
    vec2 a = p - i + (i.x + i.y) * K2;
    vec2 o = (a.x > a.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0); //vec2 of = 0.5 + 0.5*vec2(sign(a.x-a.y), sign(a.y-a.x));
    vec2 b = a - o + K2;
    vec2 c = a - 1.0 + 2.0 * K2;
    vec3 h = max(0.5 - vec3(dot(a, a), dot(b, b), dot(c, c)), 0.0);
    vec3 n = h * h * h * h * vec3(dot(a, hash(i + 0.0)), dot(b, hash(i + o)), dot(c, hash(i + 1.0)));
    return dot(n, vec3(70.0));
}

float fbm(vec2 n) {
    float total = 0.0, amplitude = 0.1;
    for (int i = 0; i < 7; i++) {
        total += noise(n) * amplitude;
        n = m * n;
        amplitude *= 0.4;
    }
    return total;
}

void main()
{

    v_TexCoord = texCoord;
    gl_Position = u_Proj * u_View *  u_Model *  position;
    gl_Position.x += fbm(vec2(iTime, iTime));
    gl_Position.y += fbm(vec2(iTime, iTime));
    gl_Position.z = 0.5;
    zValue = gl_Position.z;

}


#shader fragment
#version 330 core

in vec2 v_TexCoord;
in float zValue;
layout(location = 0) out vec4 outColor;

uniform sampler2D backgroundTexture;
uniform sampler2D noiseTexture;
uniform sampler2D u_Texture0;
uniform vec2 iResolution;
uniform float iTime;


const float PI = 3.1415926535897932;


void main()
{
    vec4 texColor = texture(u_Texture0, v_TexCoord);
    //outColor = vec4(zValue, zValue, zValue, 1);
    outColor = texColor;
}