#version 330 core

out vec4 color;

uniform vec3 u_Color;

void main()
{
	vec4 Color = vec4(u_Color.r, u_Color.g, u_Color.b, 0.01);
	color = Color;
	
}