#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 v_color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform vec3 cameraPosition;

varying vec3 dir2camera;
varying vec3 N;

out vec2 v_TexCoord;
out vec4 v_Color;
void main()
{
    gl_Position = u_Proj * u_View * u_Model * position;
    v_TexCoord = texCoord;
    v_Color = v_color;
    dir2camera = (vec4(cameraPosition, 0) - u_Model * position).xyz;
    N = vec3(normal);
}





#shader fragment
#version 330 core

in vec2 v_TexCoord;
in vec4 v_Color;

layout(location = 0) out vec4 color;

varying vec3 dir2camera;
varying vec3 N;

uniform sampler2D environmentTextureSampler;
uniform sampler2D brdfLUTSampler;
uniform float roughness;
uniform vec3 F0;

#define PI 3.14159265358979323846

vec3 SampleEnvironmentMap_Mirror(vec3 D)
{

    //
    // TODO CS248 PART 2: sample environment map in direction D.  This requires
    // converting D into spherical coordinates where Y is the polar direction
    // (warning: in our scene, theta is angle with Y axis, which differs from
    // typical convention in physics)
    //

    // Tips:
    //
    // (1) See GLSL documentation of acos(x) and atan(x, y)
    //
    // (2) atan() returns an angle in the range -PI to PI, so you'll have to
    //     convert negative values to the range 0 - 2PI
    //
    // (3) How do you convert theta and phi to normalized texture
    //     coordinates in the domain [0,1]^2?

    D = normalize(D);
    float theta = acos(D.y);
    float phi = atan(D.x, D.z) + PI;

    float rotate = PI;
    phi = (phi + rotate) - floor((phi + rotate) / 2.0 / PI) * 2.0 * PI;
    float u = clamp(phi / 2.0 / PI, 0.0, 1.0);
    float v = clamp(theta / PI, 0.0, 1.0);
    vec2 uv = vec2(u, v);
    vec3 refectColor = texture2D(environmentTextureSampler, uv).rgb;
    return refectColor;
}

vec2 IntegralBRDF(float NdotV, float Roughness)
{
    vec2 uv = vec2(NdotV, Roughness);
    vec2 BRDF = texture2D(brdfLUTSampler, uv).rg;
    return BRDF;
}

void main()
{
    vec3 V = normalize(dir2camera);
    vec3 R = 2.0 * (dot(V, N)) * N - V;
    vec3 Li = SampleEnvironmentMap_Mirror(R);
    float NdotV = clamp((1.0 + dot(V, N)) / 2.0, 0.0, 1.0);
    vec2 BRDF = IntegralBRDF(NdotV, roughness);
    //color = vec4(Li, 1);
    color = vec4(Li * (F0 * BRDF.x + BRDF.y), 1);
}