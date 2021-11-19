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
	//gl_Position = u_Proj * u_View * u_Model * position;
	//v_TexCoord = texCoord;
	v_TexCoord = texCoord;
	gl_Position = position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
uniform sampler2D u_Texture;

void main()
{
	vec4 textureColor = texture(u_Texture, v_TexCoord);
	color = textureColor;
	//color = vec4(1, 1, 1, 1);
}