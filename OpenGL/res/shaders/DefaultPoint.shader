#shader vertex
#version 330 core

layout(location = 0) in vec4 u_Position;
layout(location = 1) in vec4 u_Color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec4 v_Color;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * u_Position;
	v_Color = u_Color;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 v_Color;

void main()
{
	color = v_Color;
}