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
bool showImGUIDemoWindow = true;

float sunY = 0.0f;
float sunColor[3] = { 1.0f, 0.3f, 0.0f }; 
float skyColor[3] = { 0.0f, 0.0f, 0.5f };
float sunRadius = 0.25f; 
float hillLine = 0.0f; 

struct Vertex {
	float x, y, z;
	float u, v;
};
Vertex vertices[4] = {
	//x     y    z    u    v 
	{-0.75, -0.75, 0.0, 0.0, 0.0}, //Bottom Left
	{ 0.75, -0.75, 0.0, 1.0, 0.0}, //Bottom Right
	{ 0.75,  0.75, 0.0, 1.0, 1.0}, //Top Right
	{-0.75,  0.75, 0.0, 0.0, 1.0}  //Top Left
};

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indiciesData, int numIndicies);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

unsigned int indicies[6] = {
	0,1,2, //triangle 1
	2,3,0  //triangle 2
};

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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	jlLib::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();
	unsigned int vao = createVAO(vertices, 4, indicies, 6);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		shader.use();
		shader.setFloat("iTime", static_cast<float>(glfwGetTime())); // Set iTime
		shader.setVec2("iResolution", SCREEN_WIDTH, SCREEN_HEIGHT); // set iResolution

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			//frame setup
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			//not sure why these dont work at all?
			ImGui::Begin("Uniform Variables");
			ImGui::SliderFloat("Sun Height", &sunY, -10.0f, 10.0f);
			ImGui::ColorEdit3("sunColor", sunColor);
			ImGui::ColorEdit3("skyColor", skyColor);
			ImGui::SliderFloat("sunRadius", &sunRadius, 0.0f, 1.0f);
			ImGui::SliderFloat("hillLine", &hillLine, -1.0f, 1.0f);
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indiciesData, int numIndicies) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // changed these from GL_ARRAY_BUFFER to GL_ELEMENT_ARRAY_BUFFER

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndicies, indiciesData, GL_STATIC_DRAW);

	//Position
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

