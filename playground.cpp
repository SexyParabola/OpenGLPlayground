#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
using namespace glm;

#include "../common/shader.hpp"

vec3 colorWheel(double theta) {
    return vec3(
        (sin(theta + 0.000) + 1) / 2,
        (sin(theta + 2.094) + 1) / 2,
        (sin(theta + 4.188) + 1) / 2
    );
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

	std::vector<float> buffer;
	buffer.push_back(-1.0f); 
	buffer.push_back(-1.0f); 

	buffer.push_back(1.0f); 
	buffer.push_back(-1.0f); 

	buffer.push_back(0.0f); 
	buffer.push_back(1.0f); 

	unsigned int vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	std::vector<unsigned int> indexBuffer;
	indexBuffer.push_back(2);
	indexBuffer.push_back(1);
	indexBuffer.push_back(0);

	unsigned int indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

	vec3 color;
	unsigned int iteration = 1;
	do{
		GLfloat vertex_buffer_data[buffer.size()];
		for (int i = 0; i < buffer.size(); i++) {
			vertex_buffer_data[i] = buffer[i];
		}
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_DYNAMIC_DRAW);

		GLuint index_buffer_data[indexBuffer.size()];
		for (int i = 0; i < indexBuffer.size(); i++) {
			index_buffer_data[i] = indexBuffer[i];
		}
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_DYNAMIC_DRAW);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(programID);

		if (glfwGetTime() - tickTimer >= 0.5) {
			tickTimer = glfwGetTime();
			for (int i = 0; i < iteration * 6; i++) {
				buffer.push_back((buffer[i % iteration * 6] - buffer[iteration + 1 % iteration * 6]) / 2);
			}
			iteration++;
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

