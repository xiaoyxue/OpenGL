#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;


uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;


out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

void main()
{
    vec4 viewPos = View * Model * vec4(position, 1.0);
    FragPos = viewPos.xyz;
    TexCoords = texCoord;

    mat4 normalMatrix = transpose(inverse(View * Model));
    Normal = vec3((normalMatrix * vec4(normal, 0)).xyz);
    Normal = normalize(Normal);
    gl_Position = Proj * viewPos;
}

//void main() 
//{
//    vec4 worldPos = Model * vec4(position, 1.0);
//    FragPos = worldPos.xyz;
//    TexCoords = texCoord;
//    mat4 normalMat = transpose(inverse(Model));
//    Normal = vec3((normalMat * vec4(normal, 0)).xyz);
//    gl_Position = Proj * Model * vec4(position, 1.0);
//}

#shader fragment
#version 330 core
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gAlbedo;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;


void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition.xyz = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color
    gAlbedo.rgb = vec3(0.95);
}
