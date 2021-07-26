#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;


uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform vec3 cameraPosition;

out vec2 v_TexCoord;
out vec3 V;
out vec3 N;
void main()
{
	V = normalize(cameraPosition - (u_Model * vec4(position, 1)).xyz);
	mat4 normalMatrix = transpose(inverse(u_Model));
	N = vec3((normalMatrix * vec4(normal, 0)).xyz);
	N = normalize(N);
	v_TexCoord = texCoord;
	gl_Position = u_Proj * u_View * u_Model * vec4(position, 1);
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 N;
in vec3 V;

uniform sampler2D u_Texture0;
uniform sampler2D ssao;

float resolutionX, resolutionY;

void main()
{
	
	float NdotV = dot(N, V);
	if (NdotV < 0) {
		color = vec4(0, 0, 0, 1);
	}
	else {
		resolutionX = 1280;
		resolutionY = 1280;
		vec2 uv = vec2(gl_FragCoord.x / resolutionX, gl_FragCoord.y / resolutionY);
		float ssaoValue = texture(ssao, uv.xy).x;
		vec4 texColor = texture(u_Texture0, v_TexCoord);
		color = vec4(ssaoValue, ssaoValue, ssaoValue, ssaoValue);
	}
}