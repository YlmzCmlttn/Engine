#type vertex
#version 460

layout(location = 0) in vec3 a_Position;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(a_Position, 1.0);
}

#type fragment
#version 430

layout(location = 0) out vec4 finalColor;

void main()
{
	finalColor = vec4(1.0,0.0,0.0,1.0);
}