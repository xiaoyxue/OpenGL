#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Id;

void main()
{
	color = u_Id;
}