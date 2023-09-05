#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[9] = {
	//x     //y    //z
	-0.5, -0.5, 0.0, // bottom left
	0.5, -0.5, 0.0, // bottom right
	0.0, 0.5, 0.0 }; // top center

//setting GL position to our OG position - just a string
const char* vertexShaderSource = R"(
	#version 450
	layout(location = 0) in vec3 vPos;
		void main(){
			gl_Position = vec4(vPos,1.0);
			}
)";

//Vec 4 used for colors = RGBA all values are 0-1 range where 0 iz white and 1 is black 
const char* fragmentShaderSource = R"(
#version 450
out vec4 FragColor;
void main(){
	FragColor = vec4(1.0);
}
)";

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Vertex Array Object
	//the VAO points to the buffer just made ++ defines vertex attributes
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	///////Tell vao to pull vertex data from vbo
	/////glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Define position attribute (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);


	//Define a new buffer id
	//Vertex Buffer Object
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Create a new vertex shader object
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Supply the shader object with source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//Compile the shader object
	glCompileShader(vertexShader);

	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		//512 is an arbitrary length, but should be plenty of characters for our error message.
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
	}

	//called a render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glUseProgram(shader);
		//glBindVertexArray(vao);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
