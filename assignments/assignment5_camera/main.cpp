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
#include <jlLib/procGen.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void moveCamera(GLFWwindow* window, myLib::Camera* camera, myLib::CameraControls* controls, float deltaTime);
void resetCameraAndControls(myLib::Camera* camera, myLib::CameraControls* controls);

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
	//create mesh data
	ew::MeshData sphereMeshData = myLib::createSphere(0.5f, 64);
	ew::MeshData cylinderMeshData = myLib::createSphere(0.5f, 64);
	ew::MeshData planeMeshData = myLib::createSphere(0.5f, 64);
	//create mesh render
	ew::Mesh sphereMesh(sphereMeshData);
	ew::Mesh cylinderMesh(cylinderMeshData);
	ew::Mesh planeMesh(planeMeshData);
	//initialize transform
	ew::Transform sphereTransform;
	ew::Transform cylinderTransform;
	ew::Transform planeTransform;
	sphereTransform.position = ew::Vec3(1.0f, 0.0f, 0.0f);
	cylinderTransform.position = ew::Vec3(1.0f, 0.0f, 0.0f);
	planeTransform.position = ew::Vec3(1.0f, 0.0f, 0.0f);

	camera.position = ew::Vec3(0.0f, 0.0f, 5.0f);
	camera.target = ew::Vec3(0.0f, 0.0f, 0.0f);
	camera.fov = 60.0f;
	camera.aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;
	camera.orthoSize = 6.0f; 

	float prevTime = (float)glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		//Calculate deltaTime
		float time = (float)glfwGetTime(); //current timestamp
		float deltaTime = time - prevTime;

		moveCamera(window, &camera, &cameraControls, deltaTime);

		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();

		shader.setMat4("_Model", cubeTransforms[0].getModelMatrix());  
		shader.setMat4("_View", camera.ViewMatrix());
		shader.setMat4("_Projection", camera.ProjectionMatrix());
		cubeMesh.draw(); 

		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw();

		shader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw();

		shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw();

		//set the MODEL VIEW AND PROJECTION matrices for the sphere, cuylinder and plane

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
				if (ImGui::Button("Reset Camera")) { 
					resetCameraAndControls(&camera, &cameraControls); 
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

void moveCamera(GLFWwindow* window, myLib::Camera* camera, myLib::CameraControls* controls, float deltaTime)
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

	ew::Vec3 right = ew::Normalize(ew::Cross(forward, ew::Vec3(0, 1, 0)));
	ew::Vec3 up = ew::Normalize(ew::Cross(right, forward));

	//keyboard controls for cam movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)//forwar
	{
		camera->position += forward * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //back
	{
		camera->position -= forward * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //left
	{
		camera->position -= right * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //right
	{
		camera->position += right * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) //down
	{
		camera->position += up * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) //up
	{
		camera->position -= up * controls->moveSpeed * deltaTime;
	}

	//setting target to a point in front of the camera along its forward direction, our LookAt will be updated accordingly when rendering.
	camera->target = camera->position + forward;

	//Remember previous mouse position
	controls->prevMouseX = mouseX; 
	controls->prevMouseY = mouseY;

}

void resetCameraAndControls(myLib::Camera* camera, myLib::CameraControls* controls) 
{
	// Set camera and controls back to their default values
	camera->position = ew::Vec3(0.0f, 0.0f, 5.0f);
	camera->target = ew::Vec3(0.0f, 0.0f, 0.0f);
	camera->fov = 60.0f;
	camera->aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT); 
	camera->nearPlane = 0.1f; 
	camera->farPlane = 100.0f; 
	camera->orthographic = false; 
	camera->orthoSize = 6.0f; 
	controls->yaw = 0.0f;
	controls->pitch = 0.0f;
	controls->mousesSens = 0.1f;
	controls->moveSpeed = 0.001f;
}