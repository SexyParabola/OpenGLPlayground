#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 o_color;

out vec3 fragmentColor;

uniform mat4 MVP;

void main(){

    gl_Position = MVP * vec4(vertexPosition, 1);
    fragmentColor = o_color;
}
