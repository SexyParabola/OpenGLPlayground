#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
using namespace glm;

#include "../common/shader.hpp"
#include "Triangles.hpp"

vec3 colorWheel(double theta) {
    return vec3(
        (sin(theta + 0.000) + 1) / 2,
        (sin(theta + 2.094) + 1) / 2,
        (sin(theta + 4.188) + 1) / 2
    );
}

void printBuffer(std::vector<unsigned int> buffer) {
	std::cout << "[";
	for (int i = 0; i < buffer.size(); i++) {
		if (buffer.size() - 1 != i) {
			std::cout << buffer[i] << ", ";
		}else{
			std::cout << buffer[i] << "]" << std::endl; 
		}
	}
}

void printBuffer(std::vector<point> buffer) {
	std::cout << "[";
	for (int i = 0; i < buffer.size(); i++) {
		if (buffer.size() - 1 != i) {
			std::cout << "(" << buffer[i].x << "x, " << buffer[i].y << "y), ";
		}else{
			std::cout << "(" << buffer[i].x << "x, " << buffer[i].y << "y)]" << std::endl; 
		}
	}
}

point middlePoint(point v1, point v2) {
	return point(
		(v2.x + v1.x) / 2.0,
		(v2.y + v1.y) / 2.0
	);
}

void sierpinskiTessellation(std::vector<triangle> &triangleBuffer) {
	std::vector<triangle> tempBuffer;
	for (int i = 0; i < triangleBuffer.size(); i++) {
		tempBuffer.push_back(triangle(
			triangleBuffer[i].p1,
			middlePoint(triangleBuffer[i].p1, triangleBuffer[i].p2),
			middlePoint(triangleBuffer[i].p1, triangleBuffer[i].p3)
		));
			tempBuffer.push_back(triangle(
			triangleBuffer[i].p2,
			middlePoint(triangleBuffer[i].p2, triangleBuffer[i].p1),
			middlePoint(triangleBuffer[i].p2, triangleBuffer[i].p3)
		));
			tempBuffer.push_back(triangle(
			triangleBuffer[i].p3,
			middlePoint(triangleBuffer[i].p3, triangleBuffer[i].p1),
			middlePoint(triangleBuffer[i].p3, triangleBuffer[i].p2)
		));
	}
	triangleBuffer = tempBuffer;

}

#define pi 3.14159

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024 * 2.4, 768 * 2.2, "Playground", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	float tickTimer = glfwGetTime();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	unsigned int VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	unsigned int programID = LoadShaders( "vertexShader.glsl", "fragmentShader.glsl" );

	TriagleManager tm;

	tm.addTriangle(
		point( 0.0f , 1.0f ),
		point( 1.0f , -1.0f ),
		point( -1.0f , -1.0f )
	);
 
	unsigned int vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	unsigned int indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

	vec3 color;
	do{
		const std::vector<point> vertexBuffer = tm.getVertexBuffer();
		//printBuffer(vertexBuffer);
		GLfloat vertex_buffer_data[vertexBuffer.size() * 2];
		for (int i = 0; i < vertexBuffer.size() * 2; i += 2) {
			vertex_buffer_data[i] = vertexBuffer[i / 2].x;
			vertex_buffer_data[i + 1] = vertexBuffer[i / 2].y;
		}
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_DYNAMIC_DRAW);

		const std::vector<unsigned int> indexBuffer = tm.getIndexBuffer();
		//printBuffer(indexBuffer);
		GLuint index_buffer_data[indexBuffer.size()];
		for (int i = 0; i < indexBuffer.size(); i++) {
			index_buffer_data[i] = indexBuffer[i];
		}
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_DYNAMIC_DRAW);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(programID);

		static int lim = 0;
		if (glfwGetTime() - tickTimer >= 0.5) {
			tickTimer = glfwGetTime();
			if ( lim < 9 ) {
				tess(tm.triangleBuffer);
				lim++;
			}else{
				lim = 0;
				tm.triangleBuffer.clear();
				tm.addTriangle(
					point( 0.0f , 1.0f ),
					point( 1.0f , -1.0f ),
					point( -1.0f , -1.0f )
				);
			}
		}

		int location = glGetUniformLocation(programID, "u_Color");
		color = colorWheel(glfwGetTime());
		glUniform3f(location, color.r, color.g, color.b);
		//glClearColor(1.0f - color.r, 1.0f - color.g, 1.0f - color.b, 0.0f);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);
		//glDrawArrays(GL_TRIANGLES, 0, (int)(buffer.size() / 2));

		glDisableVertexAttribArray(0);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	glDeleteBuffers(1, &vertexBufferID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

