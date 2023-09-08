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

unsigned int createVAO(float* vertexData, int numVertices)
{
	//INSERT VBO
	//Define a new buffer id - Vertex Buffer Object
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * numVertices, vertexData, GL_STATIC_DRAW);
	//sizeof(float) * 3 * numVertices this will calculate size of array in bytes 

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//pull vertex data "originally from vbo"
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//define position wit numvertices "originally three floats"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);


	return vao;
}

unsigned int createShader(GLenum shaderType, const char* sourceCode)
{
	//create V or F shader object
	unsigned int anyShader = glCreateShader(shaderType);
	//Supply object with source code
	glShaderSource(anyShader, 1, &sourceCode, NULL);
	//compile shader object
	glCompileShader(anyShader);

	//check
	int success;
	glGetShaderiv(anyShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		//512 is arbitrary length characters for our error message.
		char infoLog[512];
		glGetShaderInfoLog(anyShader, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
		return 0;
	}

	return anyShader;
}

unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	unsigned int shaderProgram = glCreateProgram();
	//attach each stage
	glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Link all the stages together
	glLinkProgram(shaderProgram);

	//check
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
		return 0;
	}
	//The linked program now contains our compiled code, so we can delete these intermediate objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

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


	unsigned int shader = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	unsigned int vao = createVAO(vertices, 3);

	//called a render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
