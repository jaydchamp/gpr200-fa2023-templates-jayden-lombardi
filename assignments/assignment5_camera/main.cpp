#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <jlLib/camera.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void moveCamera(GLFWwindow* window, myLib::Camera* camera, myLib::CameraControls* controls);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
ew::Transform cubeTransforms[NUM_CUBES];

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

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{	cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;}

	myLib::Camera camera;
	myLib::CameraControls cameraControls;

	camera.position = ew::Vec3(0.0f, 0.0f, 5.0f);
	camera.target = ew::Vec3(0.0f, 0.0f, 0.0f);
	camera.fov = 60.0f;
	camera.aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;
	camera.orthoSize = 6.0f; 

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		moveCamera(window, &camera, &cameraControls);

		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();

		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix()); 
			shader.setMat4("_View", camera.ViewMatrix());
			shader.setMat4("_Projection", camera.ProjectionMatrix());
			cubeMesh.draw(); 
		}

		//printf("Camera Position: %.2f, %.2f, %.2f\n", camera.position.x, camera.position.y, camera.position.z);
		//printf("Camera Target: %.2f, %.2f, %.2f\n", camera.target.x, camera.target.y, camera.target.z);
		//printf("CUBE1 Position: %.2f, %.2f, %.2f\n", cubeTransforms[0].position.x, cubeTransforms[0].position.y, cubeTransforms[0].position.z);
		//printf("CUBE2 Position: %.2f, %.2f, %.2f\n", cubeTransforms[1].position.x, cubeTransforms[1].position.y, cubeTransforms[1].position.z);
		//printf("CUBE3 Position: %.2f, %.2f, %.2f\n", cubeTransforms[2].position.x, cubeTransforms[2].position.y, cubeTransforms[2].position.z);
		//printf("CUBE4 Position: %.2f, %.2f, %.2f\n", cubeTransforms[3].position.x, cubeTransforms[3].position.y, cubeTransforms[3].position.z);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");

			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Camera Position", &camera.position.x, 0.05f);
				ImGui::DragFloat3("Camera Target", &camera.target.x, 0.05f);
				ImGui::DragFloat("FOV", &camera.fov, 1.0f);
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 1.0f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Size", &camera.orthoSize, 0.1f);
				}
			}

			ImGui::Text("Cubes");
			for (size_t i = 0; i < NUM_CUBES; i++) {
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f); 
					ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.05f); 
				}
				ImGui::PopID(); 
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
}

void moveCamera(GLFWwindow* window, myLib::Camera* camera, myLib::CameraControls* controls)
{
	//If right mouse is not held, release cursor and return early.
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) { 
		//Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
		controls->firstMouse = true;
		return;
	}
	//GLFW_CURSOR_DISABLED hides the cursor, but the position will still be changed as we move our mouse.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

	//Get screen mouse position this frame
	double mouseX, mouseY; 
	glfwGetCursorPos(window, &mouseX, &mouseY); 

	//If we just started right clicking, set prevMouse values to current position.
	//This prevents a bug where the camera moves as soon as we click.
	if (controls->firstMouse) { 
		controls->firstMouse = false; 
		controls->prevMouseX = mouseX; 
		controls->prevMouseY = mouseY; 
	}

	//yaw += (cursorX - prevCursorX) * mouseSens;
	//pitch -= (cursorY - preCursorY) * mouseSens;

	//TODO: Get mouse position delta for this frame
	double xOffset = mouseX - controls->prevMouseX;
	double yOffset = controls->prevMouseY - mouseY;

	//TODO: Add to yaw and pitch
	xOffset *= controls->mousesSens;
	yOffset *= controls->mousesSens;

	controls->yaw += xOffset;
	controls->pitch += yOffset;

	//TODO: Clamp pitch between -89 and 89 degrees
	if (controls->pitch > 89.0)
	{
		controls->pitch = 89.0;
	}
	if (controls->pitch < -89.0)
	{
		controls->pitch = -89.0;
	}

	//TODO: Update prevCursorX and prevCursorY
	ew::Vec3 forward;
	forward.x = cos(ew::Radians(controls->yaw)) * cos(ew::Radians(controls->pitch));
	forward.y = sin(ew::Radians(controls->pitch));
	forward.z = sin(ew::Radians(controls->yaw)) * cos(ew::Radians(controls->pitch));
	forward = ew::Normalize(forward);

	//setting target to a point in front of the camera along its forward direction, our LookAt will be updated accordingly when rendering.
	camera->target = camera->position + forward;

	//Remember previous mouse position
	controls->prevMouseX = mouseX; 
	controls->prevMouseY = mouseY;
}