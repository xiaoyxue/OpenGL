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
uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D backgroundTexture;

#define OCTAVES  8.0

#define LIVE_SMOKE 1

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float rand2(vec2 co) {
    return fract(cos(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

// Rough Value noise implementation
float valueNoiseSimple(vec2 vl) {
    float minStep = 1.0;

    vec2 grid = floor(vl);
    vec2 gridPnt1 = grid;
    vec2 gridPnt2 = vec2(grid.x, grid.y + minStep);
    vec2 gridPnt3 = vec2(grid.x + minStep, grid.y);
    vec2 gridPnt4 = vec2(gridPnt3.x, gridPnt2.y);

    float s = rand2(grid);
    float t = rand2(gridPnt3);
    float u = rand2(gridPnt2);
    float v = rand2(gridPnt4);

    float x1 = smoothstep(0., 1., fract(vl.x));
    float interpX1 = mix(s, t, x1);
    float interpX2 = mix(u, v, x1);

    float y = smoothstep(0., 1., fract(vl.y));
    float interpY = mix(interpX1, interpX2, y);

    return interpY;
}

float getLowFreqs()
{
    const int NUM_FREQS = 32;
    /* Close to the spectrum of the voice frequencies for this song. */
    const float lowStart = 0.65;
    const float lowEnd = 0.75;
    float result = 0.0;

    /*
    for (int i = 0; i < NUM_FREQS; i++)
    {
        result += texture(iChannel0,
                            vec2(lowStart + (lowEnd - lowStart)*float(i)/float(NUM_FREQS - 1),
                                 0.25)).x;
    }*/

    return smoothstep(0.0, 1.0, (result / float(NUM_FREQS)) * 2.);
}

float fractalNoise(vec2 vl) {
    float persistance = 2.0;
    float amplitude = 0.5;
    float rez = 0.0;
    vec2 p = vl;

    for (float i = 0.0; i < OCTAVES; i++) {
        rez += amplitude * valueNoiseSimple(p);
        amplitude /= persistance;
        p *= persistance;
    }
    return rez;
}

float complexFBM(vec2 p) {
    float sound = getLowFreqs();
    float slow = iTime / 2.5;
    float fast = iTime / .5;
    vec2 offset1 = vec2(slow, 0.); // Main front
    vec2 offset2 = vec2(sin(fast) * 0.1, 0.); // sub fronts

    return
#if LIVE_SMOKE
    (1. + sound) *
#endif
        fractalNoise(p + offset1 + fractalNoise(
            p + fractalNoise(
                p + 2. * fractalNoise(p - offset2)
            )
        )
        );
}


void main()
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;

    vec3 blueColor = vec3(0.529411765, 0.807843137, 0.980392157);
    vec3 orangeColor2 = vec3(0.909803922, 0.909803922, 0.909803922);

    vec3 bcgColor = texture(backgroundTexture, uv).rgb;

    vec3 rez = mix(orangeColor2, bcgColor, complexFBM(uv));

    outColor = vec4(rez, 1.0);
}