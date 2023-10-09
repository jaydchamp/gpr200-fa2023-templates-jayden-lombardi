#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/ewMath/vec3.h>
#include <ew/procGen.h>
#include <jlLib/transformations.h> //I added this

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

//Square aspect ratio for now. We will account for this with projection later.
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;

const int numCubes = 4;

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

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//instances of all cubes
	myLib::Transform transforms[numCubes]; 
	transforms[0].position = ew::Vec3(-0.5f, 0.5f, 0.0f);
	transforms[0].rotation = ew::Vec3(0.0f, 0.0f, 0.0f);
	transforms[0].scale = ew::Vec3(1.0f, 1.0f, 1.0f);

	transforms[1].position = ew::Vec3(0.5f, 0.5f, 0.0f);
	transforms[1].rotation = ew::Vec3(0.0f, 0.0f, 0.0f);
	transforms[1].scale = ew::Vec3(1.0f, 1.0f, 1.0f);

	transforms[2].position = ew::Vec3(0.5f, -0.5f, 0.0f);
	transforms[2].rotation = ew::Vec3(0.0f, 0.0f, 0.0f);
	transforms[2].scale = ew::Vec3(1.0f, 1.0f, 1.0f);

	transforms[3].position = ew::Vec3(-0.5f, -0.5f, 0.0f);
	transforms[3].rotation = ew::Vec3(0.0f, 0.0f, 0.0f);
	transforms[3].scale = ew::Vec3(1.0f, 1.0f, 1.0f);
	
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms for all cubes
		shader.use();
		for (int i = 0; i < numCubes; i++) {
			shader.setMat4("_Model", transforms[i].getModelMatrix());
			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			for (size_t i = 0; i < numCubes; i++) 
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform"))	
				{
					ImGui::DragFloat3("Position", &transforms[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &transforms[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &transforms[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

