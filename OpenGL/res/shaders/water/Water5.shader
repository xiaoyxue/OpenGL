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
layout(location = 0) out vec4 outColor;

uniform sampler2D backgroundTexture;
uniform sampler2D noiseTexture;
uniform vec2 iResolution;
uniform float iTime;

const float PI = 3.1415926535897932;

// play with these parameters to custimize the effect
// ==================================================

//speed
const float speed = 0.2;
const float speed_x = 0.3;
const float speed_y = 0.3;

// refraction
const float emboss = 0.50;
const float intensity = 2.4;
const int steps = 8;
const float frequency = 6.0;
const int angle = 7; // better when a prime

// reflection
const float delta = 60.;
const float gain = 700.;
const float reflectionCutOff = 0.012;
const float reflectionIntensity = 200000.;

// ===================================================


float col(vec2 coord, float time)
{
    float delta_theta = 2.0 * PI / float(angle);
    float col = 0.0;
    float theta = 0.0;
    for (int i = 0; i < steps; i++)
    {
        vec2 adjc = coord;
        theta = delta_theta * float(i);
        adjc.x += cos(theta) * time * speed + time * speed_x;
        adjc.y -= sin(theta) * time * speed - time * speed_y;
        col = col + cos((adjc.x * cos(theta) - adjc.y * sin(theta)) * frequency) * intensity;
    }

    return cos(col);
}

//---------- main

void main()
{
    float time = iTime * 0.6;

    vec2 p = (gl_FragCoord.xy) / iResolution.xy, c1 = p, c2 = p;
    float cc1 = col(c1, time);

    c2.x += iResolution.x / delta;
    float dx = emboss * (cc1 - col(c2, time)) / delta;

    c2.x = p.x;
    c2.y += iResolution.y / delta;
    float dy = emboss * (cc1 - col(c2, time)) / delta;

    c1.x += dx * 0.5;
    //c1.y = -(c1.y + dy * 0.5);

    float alpha = 1. + dot(dx, dy) * gain;

    vec4 col = texture(backgroundTexture, c1) * (alpha);
    outColor = col;
}