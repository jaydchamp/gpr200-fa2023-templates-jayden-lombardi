#include <stdio.h>
#include <math.h>

#include <jlLib/shader.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace jlLib;

//unsigned int createShader(GLenum shaderType, const char* sourceCode);
//unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indiciesData, int numIndicies);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

struct Vertex {
	float x, y, z;
	float u, v;
};
Vertex vertices[4] = {
	//x  y  z   u   v 
	{-0.5, -0.5, 0.0, 0.0}, //Bottom Left
	{ 0.5, -0.5, 1.0, 0.0}, //Bottom Right
	{ 0.5,  0.5, 1.0, 1.0}, //Top Right
	{-0.5,  0.5, 0.0, 1.0}  //Top Left
};

unsigned int indicies[6] = {
	0,1,1, //triangle 1
	1,1,0  //triangle 2
};

float triangleColor[3] = { 1.0f, 0.5f, 0.0f };
float triangleBrightness = 1.0f;
bool showImGUIDemoWindow = true;

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
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//WireFrame
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Shaded
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	jlLib::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();
	shader.setFloat("_MyFloat", 4.2);
	shader.setVec2("_MyVec2", 15, 12);


	//std::string vertexShaderSource = jlLib::loadShaderSourceFromFile("assets/vertexShader.vert");
	//std::string fragmentShaderSource = jlLib::loadShaderSourceFromFile("assets/fragmentShader.frag");
	//unsigned int shader = createShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
	//unsigned int vao = createVAO(vertices, 3);

	//glUseProgram(shader);
	//glBindVertexArray(vao);



	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		//glUniform3f(glGetUniformLocation(shader, "_Color"), triangleColor[0], triangleColor[1], triangleColor[2]);
		//glUniform1f(glGetUniformLocation(shader,"_Brightness"), triangleBrightness);

		//put these in instead of the two above
		shader.setVec3("_Color", triangleColor[0], triangleColor[1], triangleColor[2]);
		shader.setFloat("_Brightness", triangleBrightness);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Color", triangleColor);
			ImGui::SliderFloat("Brightness", &triangleBrightness, 0.0f, 1.0f);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}


//unsigned int createShader(GLenum shaderType, const char* sourceCode) {
//	//Create a new vertex shader object
//	unsigned int shader = glCreateShader(shaderType);
//	//Supply the shader object with source code
//	glShaderSource(shader, 1, &sourceCode, NULL);
//	//Compile the shader object
//	glCompileShader(shader);
//	int success;
//	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//	if (!success) {
//		//512 is an arbitrary length, but should be plenty of characters for our error message.
//		char infoLog[512];
//		glGetShaderInfoLog(shader, 512, NULL, infoLog);
//		printf("Failed to compile shader: %s", infoLog);
//	}
//	return shader;
//}

//unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
//	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
//	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
//
//	unsigned int shaderProgram = glCreateProgram();
//	//Attach each stage
//	glAttachShader(shaderProgram, vertexShader);
//	glAttachShader(shaderProgram, fragmentShader);
//	//Link all the stages together
//	glLinkProgram(shaderProgram);
//	int success;
//	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//	if (!success) {
//		char infoLog[512];
//		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//		printf("Failed to link shader program: %s", infoLog);
//	}
//	//The linked program now contains our compiled code, so we can delete these intermediate objects
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//	return shaderProgram;
//}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indiciesData, int numIndicies) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, vertexData, GL_STATIC_DRAW);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndicies, indiciesData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

