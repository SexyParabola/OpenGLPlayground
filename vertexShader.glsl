#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;

void main(){

    gl_Position.xyz = vertexPosition;
    gl_Position.w = 1;

}
