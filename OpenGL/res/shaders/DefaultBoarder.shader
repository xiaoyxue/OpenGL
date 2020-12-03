#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}


#shader fragment
#version 330 core

uniform int width;
uniform vec2 res;

layout(location = 0) out vec4 color;

void main()
{

	if ((gl_FragCoord.x < width) || (gl_FragCoord.x > res.x - width) || (gl_FragCoord.y < width) || (gl_FragCoord.y > res.y - width))
	{
		color = vec4(0, 0, 0, 1);
		return;
	}
	else {
		//color = vec4(1, 0, 0, 1);
		discard;
	}
		
}