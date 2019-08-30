#version 330 core

out vec4 color;

uniform vec3 u_Color;

in vec3 fragmentColor;

void main()
{
	vec4 Color = vec4(fragmentColor.r, fragmentColor.g, fragmentColor.b, 1.0);
	//vec4 Color = vec4(u_Color.r, u_Color.g, u_Color.b, 0.0);
	color = Color;
	
}