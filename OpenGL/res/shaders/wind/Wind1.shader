#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec2 v_TexCoord;

uniform float iTime;

//float randomNum(in float2 uv)
//{
//    vec2 noise = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
//    return abs(noise.x + noise.y) * 0.5;
//}

void main()
{

    v_TexCoord = texCoord;
    gl_Position = u_Proj * u_View *  u_Model *  position;

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



void main()
{

    outColor = vec4(0, 1, 0, 1);
}