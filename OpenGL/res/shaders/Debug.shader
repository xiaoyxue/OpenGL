#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec2 v_TexCoord;

void main()
{
	gl_Position = vec4(position.x, position.y, 0.999, 1);
	v_TexCoord = texCoord;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	float texColorX = texture(u_Texture, v_TexCoord).x;
	//color = vec4(texColorX, texColorX, texColorX, 1);
	color = texColor;
}
