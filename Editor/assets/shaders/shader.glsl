#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(binding = 0, std140) uniform Transforms
{
    mat4 u_M;
    mat4 u_V;
    mat4 u_P;
}tranforms;

void main()
{
	gl_Position = tranforms.u_P * tranforms.u_V * tranforms.u_M * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 finalColor;

void main()
{
	finalColor = vec4(1.0, 0.0, 0.0, 1.0);
}
