#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ew/shader.h>
#include <jlLib/texture.h>
//#include <jlLib/shader.h>

struct Vertex {
	float x, y, z;
	float u, v;
};

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

Vertex vertices[4] = {
	{-1.0, -1.0, 0.0, 0.0, 0.0},
	{1.0, -1.0, 0.0, 1.0, 0.0},
	{1.0, 1.0, 0.0, 1.0, 1.0},
	{-1.0, 1.0, 0.0, 0.0, 1.0}
};
unsigned short indices[6] = {
	0, 1, 2,
	2, 3, 0
};



int main() {

	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures", NULL, NULL);
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

	unsigned int quadVAO = createVAO(vertices, 4, indices, 6);
	glBindVertexArray(quadVAO);

	//load textures
	unsigned int backgroundTexture1 = loadTexture("assets/background_texture1.png", GL_REPEAT, GL_LINEAR);
	unsigned int backgroundTexture2 = loadTexture("assets/background_texture2.png", GL_REPEAT, GL_LINEAR);
	unsigned int characterTexture1 = loadTexture("assets/character_texture1.png", GL_CLAMP_TO_EDGE, GL_NEAREST);

	//values
	float scrollSpeedX = 0.1f; //scroll speed left right
	float scrollSpeedY = 0.0f; //scroll speed up down
	float scale = 1.0f; //scale factor

	float characterScaleX = 1.0f;//char scale x
	float characterScaleY = 1.0f;//char scale y
	float characterOpacity = 1.0f;//opacity

	float characterPosX = 0.0f;
	float characterPosY = 0.0f;

	//shaders
	ew::Shader backgroundShader("assets/background.vert", "assets/background.frag");
	ew::Shader characterShader("assets/character.vert", "assets/character.frag");
	 
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Update character position and scale here
		float currentTime = glfwGetTime();
		characterPosX = currentTime * 0.2;
		characterPosY = 0.5 * sin(currentTime * 0.9) - 0.6;
		characterScaleX = 1.0; // You can adjust the scale factors as needed
		characterScaleY = 1.0;

		// Draw background
		backgroundShader.use();
		backgroundShader.setInt("backgroundTexture1", 0); // Bind to texture unit 0
		backgroundShader.setInt("backgroundTexture2", 1); // Bind to texture unit 1
		backgroundShader.setFloat("iTime", currentTime); // Set iTime to the current time
		backgroundShader.setFloat("scrollSpeedX", scrollSpeedX);
		backgroundShader.setFloat("scrollSpeedY", scrollSpeedY);
		backgroundShader.setVec2("backgroundScale", scale, scale);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, backgroundTexture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, backgroundTexture2);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

		// Draw character
		characterShader.use();
		characterShader.setVec2("characterScale", characterScaleX, characterScaleY);
		characterShader.setFloat("iTime", currentTime); // Set iTime to the current time
		characterShader.setFloat("characterOpacity", characterOpacity);
		characterShader.setVec2("characterPosition", characterPosX, characterPosY);
		characterShader.setVec2("iResolution", SCREEN_WIDTH, SCREEN_HEIGHT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, characterTexture1);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Vertex Buffer Object 
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, vertexData, GL_STATIC_DRAW);

	//Element Buffer Object
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numIndices, indicesData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,x));
	glEnableVertexAttribArray(0);

	//UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

