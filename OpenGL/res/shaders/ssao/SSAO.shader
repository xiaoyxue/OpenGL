#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoord;

out vec2 TexCoords;

void main()
{
    TexCoords = texCoord;
    gl_Position = position;
}



#shader fragment
#version 330 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

uniform vec3 samples[64];

uniform sampler2D gPosition;
uniform sampler2D gNormal;



const float PI = 3.14159265359;
const int SAMPLE_SIZE = 64;
float bias = 0.0025;

const float radius = 0.5;

in vec2 TexCoords;

layout(location = 0) out vec4 FragColor;

void CoordinateSystem(const vec3 v1, out vec3 v2, out vec3 v3) {
    if (abs(v1.x) > abs(v1.y)) {
        v2 = vec3(-v1.z, 0, v1.x) / sqrt(v1.x * v1.x + v1.z * v1.z);
    }
    else {
        v2 = vec3(0, v1.z, -v1.y) / sqrt(v1.y * v1.y + v1.z * v1.z);
    }
    v3 = cross(v1, v2);
}


float LinearizeDepth(float depth)
{
    const float near = 0.1f;
    const float far = 1000.0f;
    float z = depth * 2.0 - 1.0; // 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

float LengthSqr(vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

void main()
{
    vec3 position = texture(gPosition, TexCoords).xyz;
    vec3 normal = texture(gNormal, TexCoords).xyz;
    vec3 v1 = normal;
    vec3 v2, v3;
    vec3 tangent, binormal;
    float occlusion = 0.0;
    float visibility = 1.0;
    float delta = 0.0;
    float absDelta = 0.0;
    //CoordinateSystem(v1, v2, v3);
    // for (int i = 0; i < SAMPLE_SIZE; ++i) {
    //    vec3 randomSample = samples[i];
    //    float zeta1 = randomSample.x, zeta2 = randomSample.y, zeta3 = randomSample.z;
    //    float r = radius * zeta3;
    //    float x = r * cos(2 * PI * zeta2) * sqrt(1 - zeta1 * zeta1);
    //    float y = r * sin(2 * PI * zeta2) * sqrt(1 - zeta1 * zeta1);
    //    float z = r * zeta1;
    //    vec4 sampleDir = vec4(x, y, z, 0);
    //    vec4 cameraSpaceSamplePoint = vec4(position, 1) + sampleDir;
    //    vec4 ndcSpacePoint = Proj * cameraSpaceSamplePoint;
    //    ndcSpacePoint.xyz /= ndcSpacePoint.w;
    //    ndcSpacePoint.xyz = ndcSpacePoint.xyz * 0.5 + 0.5;
    //    vec4 projCoord = Proj * vec4(texture(gPosition, ndcSpacePoint.xy).xyz, 1);
    //    projCoord.xyz /= projCoord.w;
    //    float sampleDepth = texture(gPosition, ndcSpacePoint.xy).z;
    //    float rangeCheck = smoothstep(0.0, 1.0, radius / abs(position.z - sampleDepth));
    //    /*occlusion += (sampleDepth <= cameraSpaceSamplePoint.z + bias ? 1.0 : 0.0) * rangeCheck;*/
    //    occlusion += sampleDepth <= cameraSpaceSamplePoint.z + bias ? 1.0 : 0.0;
    // }
    //float visibility = 1.0 - (occlusion / SAMPLE_SIZE);
    //FragColor = vec4(occlusion, occlusion, occlusion, 1);
    vec4 debugVal = vec4(0, 0, 0, 1);
    if (LengthSqr(normal) > 0.1) {
        CoordinateSystem(v1, v2, v3);
        vec3 randomVec = normalize(vec3(v1.x + 0.3, v1.y, v1.z));
        tangent = normalize(randomVec - dot(normal, randomVec) * normal);
        binormal = cross(v1, v2);
        for (int i = 0; i < SAMPLE_SIZE; ++i) {
            vec3 randomSample = samples[i];
            float zeta1 = randomSample.x, zeta2 = randomSample.y, zeta3 = randomSample.z;
            float r = radius * zeta3;
            float x = r * cos(2 * PI * zeta2) * sqrt(1 - zeta1 * zeta1);
            float y = r * sin(2 * PI * zeta2) * sqrt(1 - zeta1 * zeta1);
            float z = r * zeta1;
            //vec4 sampleDir = vec4(x * tangent + y * binormal + z * normal, 0);
            vec4 sampleDir = vec4(x * v2 + z * v1 + y * v3, 0);
            //vec4 sampleDir = vec4(normal.xyz, 0) * 0.5;
            vec4 cameraSpaceSamplePoint = vec4(position, 1) + sampleDir;
            vec4 ndcSpacePoint = Proj * cameraSpaceSamplePoint;
            vec2 uv = ndcSpacePoint.xy / ndcSpacePoint.w * 0.5 + 0.5;
            float sampleDepth = texture(gPosition, uv.xy).z;

            
            //float delta = cameraSpaceSamplePoint.z - sampleDepth;
            //absDelta = abs(cameraSpaceSamplePoint.z - sampleDepth);

            //if (delta >= 0 && delta < 0.33) {
            //    debugVal = vec4(0.5, 0, 0, 1);
            //}
            //else if (delta >= 0.33 && delta < 0.66) {
            //    debugVal = vec4(0, 0.5, 0, 1);
            //}
            //else if(delta >= 0.66 && delta < 1){
            //    debugVal = vec4(0, 0, 0.5, 1);
            //}
            //else if(delta >= 1 && delta < 10) {
            //    debugVal = vec4(0.5, 0.5, 0, 1);
            //}
            //else {
            //    debugVal = vec4(0.5, 0.5, 0.5, 1);
            //}
            //if (delta < 0) {
            //    debugVal = vec4(0.5, 0, 0, 1);
            //}
            //else {
            //    debugVal = vec4(0, 0, 0.5, 1);
            //}
           
            float cosWeight = dot(normalize(sampleDir).xyz, normal);
            if (cameraSpaceSamplePoint.z > sampleDepth && cosWeight > 0.001) {
                float rangeCheck = smoothstep(0.0, 1.0, r / abs(position.z - sampleDepth));
                occlusion += 1 * rangeCheck * cosWeight ;
                //occlusion += 1.0;

            }
            //float rangeCheck = smoothstep(0.0, 1.0, radius / abs(position.z - sampleDepth));
            //occlusion += (sampleDepth >= cameraSpaceSamplePoint.z + bias ? 1.0 : 0.0) * rangeCheck;

        }
        visibility = 1.0 - (occlusion / SAMPLE_SIZE);
    }

    FragColor = vec4(visibility, visibility, visibility, 1);
}
