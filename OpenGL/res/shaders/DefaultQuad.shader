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
    //gl_Position = position;
    gl_Position = vec4(position.x, position.y, 0.999, 1);
}


#shader fragment
#version 330 core

in vec2 v_TexCoord;
layout(location = 0) out vec4 outColor;

uniform sampler2D backgroundTexture;
uniform sampler2D noiseTexture;
uniform vec2 iResolution;
uniform float iTime;


void main()
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec3 col = texture(backgroundTexture, uv).rgb;
    outColor = vec4(col, 1);
}