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

vec3 colorWheel(double theta) {
    return vec3(
        (sin(theta + 0.000) + 1) / 2,
        (sin(theta + 2.094) + 1) / 2,
        (sin(theta + 4.188) + 1) / 2
    );
}

struct point {
	float x;
	float y;
	point(float X, float Y){
		x = X; y = Y;
	}
};

point middlePoint(point v1, point v2) {
	return point(
		(v2.x + v1.x) / 2.0,
		(v2.y + v1.y) / 2.0
	);
}

void crunch(std::vector<point> &vBuffer, const std::vector<unsigned int> iBuffer) {
	for(int i = 0; i < iBuffer.size(); i+=3) {
		vBuffer.push_back(middlePoint( vBuffer[iBuffer[i + 0]], vBuffer[iBuffer[i + 1]] ));
		vBuffer.push_back(middlePoint( vBuffer[iBuffer[i + 1]], vBuffer[iBuffer[i + 2]] ));
		vBuffer.push_back(middlePoint( vBuffer[iBuffer[i + 2]], vBuffer[iBuffer[i + 0]] ));
	}
}

void tess(std::vector<unsigned int> &iBuffer, unsigned int &iter){
	 std::vector<unsigned int> temp;
	 std::vector<unsigned int> dBuffer;
	 const unsigned int s = iter * 3;
	for(int i = 0; i < iBuffer.size(); i++) {
		dBuffer.push_back(s + i);
	}
	 for (int oof = 0; oof < iBuffer.size(); oof+=3) {
		for (int i = 0; i < 3; i++) {
			temp.push_back(iBuffer[i + oof]);
			temp.push_back(dBuffer[(i + oof) % iBuffer.size()]);
			temp.push_back(dBuffer[(i + oof + 1) % iBuffer.size()]);
		}
	 }
	 iBuffer = temp;
	 iter++;
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
	window = glfwCreateWindow( 768 * 2.4, 768 * 2.2, "Playground", NULL, NULL);
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

	std::vector<point> vertexBuffer;
	vertexBuffer.push_back(point(0.0, 1.0));
	vertexBuffer.push_back(point(1.0, -1.0)); 
	vertexBuffer.push_back(point(-1.0, -1.0));

	unsigned int vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	std::vector<unsigned int> indexBuffer;
	indexBuffer.push_back(0);
	indexBuffer.push_back(1);
	indexBuffer.push_back(2);

	unsigned int indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

	vec3 color;
	unsigned int iteration = 1;
	do{
		GLfloat vertex_buffer_data[vertexBuffer.size() * 2];
		for (int i = 0; i < vertexBuffer.size() * 2; i = i + 2) {
			vertex_buffer_data[i] = vertexBuffer[i / 2].x;
			vertex_buffer_data[i + 1] = vertexBuffer[i / 2].y;
		}
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_DYNAMIC_DRAW);

		GLuint index_buffer_data[indexBuffer.size()];
		for (int i = 0; i < indexBuffer.size(); i++) {
			index_buffer_data[i] = indexBuffer[i];
		}
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_DYNAMIC_DRAW);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(programID);

		if (glfwGetTime() - tickTimer >= 1.5f) {
			tickTimer = glfwGetTime();

			crunch(vertexBuffer, indexBuffer);
			tess(indexBuffer, iteration);
			printBuffer(indexBuffer);  
			printBuffer(vertexBuffer);
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

