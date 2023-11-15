#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;
const int MAX_LIGHTS = 4;

float prevTime;
ew::Vec3 bgColor = ew::Vec3(0.1f);

ew::Camera camera;
ew::CameraController cameraController;

struct Light {
	ew::Vec3 position; // world space
	ew::Vec3 color; //rgb value
};

struct Material {
	float ambientK; //Ambient (0-1)
	float diffuseK; //Diffuse (0-1)
	float specular; //Specular (0-1)
	float shininess; //Shininess
};

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
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

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/defaultLit.vert", "assets/defaultLit.frag");
	ew::Shader lightShader("assets/unlit.vert", "assets/unlit.frag");
	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg",GL_REPEAT,GL_LINEAR);

	//Create cube
	ew::Mesh cubeMesh(ew::createCube(1.0f));
	ew::Mesh planeMesh(ew::createPlane(5.0f, 5.0f, 10));
	ew::Mesh sphereMesh(ew::createSphere(0.5f, 64));
	ew::Mesh cylinderMesh(ew::createCylinder(0.5f, 1.0f, 32));

	//Initialize transforms
	ew::Transform cubeTransform;
	ew::Transform planeTransform;
	ew::Transform sphereTransform;
	ew::Transform cylinderTransform;
	planeTransform.position = ew::Vec3(0, -1.0, 0);
	sphereTransform.position = ew::Vec3(-1.5f, 0.0f, 0.0f);
	cylinderTransform.position = ew::Vec3(1.5f, 0.0f, 0.0f);

	Material material;
	material.ambientK = 0.5;
	material.diffuseK = 0.5;
	material.specular = 0.5;
	material.shininess = 20;

	Light light[MAX_LIGHTS];
	light[0].position = ew::Vec3(-3.0f, 1.0f, 0.0f);
	light[0].color = ew::Vec3(0.0f, 1.0f, 0.0f);
	light[1].position = ew::Vec3(3.0f, 1.0f, 0.0f);
	light[1].color = ew::Vec3(0.0f, 0.0f, 1.0f);
	light[2].position = ew::Vec3(-1.5f, 1.0f, -3.0f);
	light[2].color = ew::Vec3(1.0f, 0.0f, 0.0f);
	light[3].position = ew::Vec3(1.5f, 1.0f, -3.0f);
	light[3].color = ew::Vec3(1.0f, 1.0f, 0.0f);
	int numLights = 1; 
	bool orbitSpheres = false;

	resetCamera(camera,cameraController);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		//Update camera
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);

		//RENDER
		glClearColor(bgColor.x, bgColor.y,bgColor.z,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, brickTexture);

		//spheres orbitting or not?
		if (orbitSpheres) 
		{
			for (int i = 0; i < numLights; i++) 
			{
				light[i].position.x = 3.0f * cos(0.5f * time + i * 1.57f); 
				light[i].position.z = 3.0f * sin(0.5f * time + i * 1.57f); 
			}
		}

		//Draw shapes
		shader.use();
		shader.setInt("_Texture", 0);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		shader.setVec3("_Color", ew::Vec3(material.ambientK));
		shader.setVec3("_CameraPosition", camera.position);

		shader.setFloat("_Material.ambientK", material.ambientK);
		shader.setFloat("_Material.diffuseK", material.diffuseK);
		shader.setFloat("_Material.specular", material.specular);
		shader.setFloat("_Material.shininess", material.shininess);

		//
		for (int i = 0; i < numLights; i++) 
		{
			shader.setVec3("_Lights[" + std::to_string(i) + "].position", light[i].position);
			shader.setVec3("_Lights[" + std::to_string(i) + "].color", light[i].color);
		}

		shader.setMat4("_Model", cubeTransform.getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw();

		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw();

		shader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw();

		//render spheres:
		lightShader.use();
		lightShader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());

		ew::Transform lightTransform;
		lightTransform.position = light[0].position;
		lightTransform.scale = ew::Vec3(0.1f); //scale down
		lightShader.setMat4("_Model", lightTransform.getModelMatrix());
		lightShader.setVec3("_Color", light[0].color);
		lightShader.setVec3("_CameraPosition", camera.position);
		sphereMesh.draw();

		//rendering is only done if there is more than 1 lights requested from the user
		//using "i < numLights" instead of "i < MAX_LIGHTS"
		if (numLights > 1) 
		{
			for (int i = 1; i < numLights; i++) 
			{
				ew::Transform additionalLightTransform; 
				additionalLightTransform.position = light[i].position; 
				additionalLightTransform.scale = ew::Vec3(0.1f); // Scale down 
				lightShader.setMat4("_Model", additionalLightTransform.getModelMatrix()); 
				lightShader.setVec3("_Color", light[i].color);
				lightShader.setVec3("_CameraPosition", camera.position);
				sphereMesh.draw();
			}
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				ImGui::ColorEdit3("BG color", &bgColor.x); 
				ImGui::SliderInt("Number of Lights", &numLights, 1, MAX_LIGHTS);
				ImGui::Checkbox("Orbit Spheres", &orbitSpheres);
				if (ImGui::Button("Reset")) {
					resetCamera(camera, cameraController);
				}
			}
			//Begin with 1 light and allow more upon request
			//for every unqiue light add a new collapsing header
			for (int i = 0; i < numLights; ++i) {
				ImGui::PushID(i); //unique lights
				if (ImGui::CollapsingHeader(("Light " + std::to_string(i + 1)).c_str())) {
					// Show constantly changing light position
					ImGui::Text("Light Position:", light[i].position.x, light[i].position.y, light[i].position.z);
					// Allow for RGB color changing
					ImGui::ColorEdit3(("Color##" + std::to_string(i)).c_str(), &light[i].color.x);
				}
				ImGui::PopID();
			}

			if (ImGui::CollapsingHeader("Material")) {
				//AmbientK Slider 0-1
				ImGui::SliderFloat("Ambient", &material.ambientK, 0, 1);
				//DiffuseK Slider 0-1
				ImGui::SliderFloat("Diffuse", &material.diffuseK, 0, 1);
				//SpecularK Slider 0-1
				ImGui::SliderFloat("Specular", &material.specular, 0, 1);
				//Shinniniess slider 0-1024
				ImGui::SliderFloat("Shiny", &material.shininess, 0, 1024);
			}

			ImGui::End();
			
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
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0);
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}


