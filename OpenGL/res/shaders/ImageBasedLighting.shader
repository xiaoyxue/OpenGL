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
uniform sampler2D faceTextureSampler;
uniform sampler2D environmentTextureSampler;

#define PI 3.14159265358979323846

//
// The code below is a full implementation of a Disney BDRF that students may enjoy playing around with
// 
// For more detail, see:
//      https://disney-animation.s3.amazonaws.com/library/s2012_pbs_disney_brdf_notes_v2.pdf
//      https://www.disneyanimation.com/technology/brdf.html
//

// float sqr(float x) { return x*x; }

// float SchlickFresnel(float u)
// {
//     float m = clamp(1.-u, 0., 1.);
//     float m2 = m*m;
//     return m2 * m2 * m; // pow(m,5)
// }

// float GTR1(float NdotH, float a)
// {
//     if (a >= 1.) return 1./PI;
//     float a2 = a*a;
//     float t = 1. + (a2 - 1.)*NdotH*NdotH;
//     return (a2 - 1.) / (PI * log(a2) * t);
// }

// float GTR2(float NdotH, float a)
// {
//     float a2 = a*a;
//     float t = 1. + (a2-1.)*NdotH*NdotH;
//     return a2 / (PI * t*t);
// }

// float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
// {
//     return 1. / (PI * ax*ay * sqr( sqr(HdotX/ax) + sqr(HdotY/ay) + NdotH*NdotH ));
// }

// float smithG_GGX(float NdotV, float alphaG)
// {
//     float a = alphaG*alphaG;
//     float b = NdotV*NdotV;
//     return 1. / (NdotV + sqrt(a + b - a*b));
// }

// float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay)
// {
//     return 1. / (NdotV + sqrt( sqr(VdotX*ax) + sqr(VdotY*ay) + sqr(NdotV) ));
// }

// vec3 mon2lin(vec3 x)
// {
//     return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
// }


// // Disney_BRDF --
// //
// // Evaluate Disney BRDF
// //
// // L -- direction to light
// // V -- direction to camera (view direction)
// // N -- surface normal at point being shaded
// // X and Y are tangent/binormal
// //
// vec3 Disney_BRDF( vec3 L, vec3 V, vec3 N, vec3 X, vec3 Y, vec3 baseColor)
// {
//     float NdotL = clamp(dot(N,L), .0001, .9999);
//     float NdotV = clamp(dot(N,V), .0001, .9999);

//     vec3 H = normalize(L+V);
//     float NdotH = clamp(dot(N,H), .0001, .9999);
//     float LdotH = clamp(dot(L,H), .0001, .9999);

//     vec3 Cdlin = mon2lin(baseColor);
//     float Cdlum = .3*Cdlin[0] + .6*Cdlin[1]  + .1*Cdlin[2]; // luminance approx.

//     vec3 Ctint = Cdlum > 0. ? Cdlin/Cdlum : vec3(1); // normalize lum. to isolate hue+sat
//     vec3 Cspec0 = mix(specular*.08*mix(vec3(1), Ctint, specularTint), Cdlin, metallic);
//     vec3 Csheen = mix(vec3(1), Ctint, sheenTint);

//     // Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
//     // and mix in diffuse retro-reflection based on roughness
//     float FL = SchlickFresnel(NdotL), FV = SchlickFresnel(NdotV);
//     float Fd90 = 0.5 + 2. * LdotH*LdotH * roughness;
//     float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);

//     // Based on Hanrahan-Krueger brdf approximation of isotropic bssrdf
//     // 1.25 scale is used to (roughly) preserve albedo
//     // Fss90 used to "flatten" retroreflection based on roughness
//     float Fss90 = LdotH*LdotH*roughness;
//     float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV);
//     float ss = 1.25 * (Fss * (1. / (NdotL + NdotV) - .5) + .5);

//     // specular
//     float aspect = sqrt(1.-anisotropic*.9);
//     float ax = max(.001, sqr(roughness)/aspect);
//     float ay = max(.001, sqr(roughness)*aspect);
//     float Ds = GTR2_aniso(NdotH, dot(H, X), dot(H, Y), ax, ay);
//     float FH = SchlickFresnel(LdotH);
//     vec3 Fs = mix(Cspec0, vec3(1), FH);
//     float Gs;
//     Gs  = smithG_GGX_aniso(NdotL, dot(L, X), dot(L, Y), ax, ay);
//     Gs *= smithG_GGX_aniso(NdotV, dot(V, X), dot(V, Y), ax, ay);

//     // sheen
//     vec3 Fsheen = FH * sheen * Csheen;

//     // clearcoat (ior = 1.5 -> F0 = 0.04)
//     float Dr = GTR1(NdotH, mix(.1,.001,clearcoatGloss));
//     float Fr = mix(.04, 1.0, FH);
//     float Gr = smithG_GGX(NdotL, .25) * smithG_GGX(NdotV, .25);

//     return ((1./PI) * mix(Fd, ss, subsurface)*Cdlin + Fsheen)
//         * (1.-metallic)
//         + Gs*Fs*Ds + .25*clearcoat*Gr*Fr*Dr;
// }

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


void main()
{
	vec3 V = normalize(dir2camera);
	vec3 R = 2.0 * (dot(V, N)) * N - V;
    color = vec4(SampleEnvironmentMap_Mirror(R), 1);
}