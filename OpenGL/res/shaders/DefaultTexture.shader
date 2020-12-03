#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 v_color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Trace;

out vec2 v_TexCoord;
out vec4 v_Color;
void main()
{
	//gl_Position = u_Proj * u_View * u_Model * position;
	gl_Position = u_Proj * u_Trace * u_View * u_Model * position;
	v_TexCoord = texCoord;
	v_Color = v_color;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;

uniform sampler2D u_Texture0;

void main()
{
	vec4 texColor = texture(u_Texture0, v_TexCoord);
	color = texColor;
}