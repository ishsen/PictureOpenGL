#pragma warning(disable:4996)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include<string> 

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include <vector>

#include"Model.h"



const unsigned int width = 1366;
const unsigned int height = 768;

struct ModelShader
{
	Model model;
	Shader shaderProgram;
	float size = 1.0f;
	float color[4] = { 0.8f, 0.3f, 0.02f, 1.0f };
	float trans[3] = { 0.0f, 0.0f, 0.0f };
	float rot[3] = { 1.0f, 1.0f, 1.0f };

	
};

//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//	glViewport(0, 0, width, height);
//}
//
//void processInput(GLFWwindow* window);
//
//void processInput(GLFWwindow* window) {
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//}

void saveImage(GLFWwindow* w) {
	int width, height;
	glfwGetFramebufferSize(w, &width, &height);
	GLsizei nrChannels = 3;
	GLsizei stride = nrChannels * width;
	stride += (stride % 4) ? (4 - stride % 4) : 0;
	GLsizei bufferSize = stride * height;
	std::vector<char> buffer(bufferSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());


	stbi_flip_vertically_on_write(true);
	stbi_write_png("sample1.png", width, height, nrChannels, buffer.data(), stride);
}

const unsigned int SCR_WIDTH = 1366;
const unsigned int SCR_HEIGHT = 768;








int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;

		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);
	//glfwMaximizeWindow(window);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	gladLoadGL();

	glViewport(0, 0, width, height);


	std::vector <ModelShader> models{
		{ Model("models/bunny/scene.gltf"), Shader("default.vert", "default.frag")},
		{ Model("models/bunny/scene.gltf"), Shader("default.vert", "default.frag")},
		{ Model("models/sword/scene.gltf"), Shader("default.vert", "default.frag")},
	


	};

	float lightColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	//float lightPos[3] = { 0.5f, 0.5f, 0.5f };
	
	glm::mat4 lightModel = glm::mat4(1.0f);

	lightModel = glm::translate(lightModel, lightPos);


	// Generates Shader object using shaders default.vert and default.frag
	//Shader shaderProgram("default.vert", "default.frag");
	for (unsigned int i = 0; i < models.size(); i++)
	{

		models.at(i).shaderProgram.Activate();
		glUniform4f(glGetUniformLocation(models.at(i).shaderProgram.ID, "lightColor"), lightColor[0], lightColor[1], lightColor[2], lightColor[3]);
		glUniform3f(glGetUniformLocation(models.at(i).shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(models.at(i).shaderProgram.ID, "rot"), models.at(i).rot[0], models.at(i).rot[1], models.at(i).rot[2]);
		glUniform1f(glGetUniformLocation(models.at(i).shaderProgram.ID, "size"), models.at(i).size);
		glUniform4f(glGetUniformLocation(models.at(i).shaderProgram.ID, "color"), models.at(i).color[0], models.at(i).color[1], models.at(i).color[2], models.at(i).color[3]);
		glUniform3f(glGetUniformLocation(models.at(i).shaderProgram.ID, "trans"), models.at(i).trans[0], models.at(i).trans[1], models.at(i).trans[2]);


	}
	// Take care of all the light related things


	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	
	
	// Creates camera object



	Camera camera(width, height, glm::vec3(0.0f, 0.5f, 3.0f));


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();





	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Variables to be changed in the ImGUI window

	//double prevTime = glfwGetTime();


	// Exporting variables to shaders
	//glUseProgram(shaderProgram);



	//GLuint sizeID = glGetUniformLocation(shaderProgram.ID, "size");
	//GLuint colorID = glGetUniformLocation(shaderProgram.ID, "color");
	//glUniform4f(glGetUniformLocation(shaderProgram.ID, "color"), color[0], color[1], color[2], color[3]);

	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);


	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// generate texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1366, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);



	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1366, 768);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	bool opt_ex1_spinningcube = true;
	bool opt_ex1_1_morecubes = false;
	bool opt_Camera = false;

	bool show_demo_window = true;
	bool activateCamera = true;
	int active = 0;
	bool my_tool_active = true;


	float camMatrix[3] = { 45.0f, 0.1f, 100.0f };
;	std::string pickModel = "models/bunny/scene.gltf";

	while (!glfwWindowShouldClose(window))
	{

		//processInput(window);

		//glDrawBuffer(GL_BACK);

		//processInput(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", nullptr, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);


				//ImGui::Text("size = %d x %d", my_image_width, my_image_height);


				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Examples")) {

				bool lastState = opt_ex1_spinningcube;
				ImGui::MenuItem("Exemplo 1 Spinning Cube", NULL, &opt_ex1_spinningcube);
				if (lastState != opt_ex1_spinningcube) {
					opt_ex1_1_morecubes = false;
					opt_Camera = false;
				}

				lastState = opt_ex1_1_morecubes;
				ImGui::MenuItem("Exemplo 1.1 More Cubes", NULL, &opt_ex1_1_morecubes);
				if (lastState != opt_ex1_1_morecubes) {
					opt_ex1_spinningcube = false;
					opt_Camera = false;
				}

				lastState = opt_Camera;
				ImGui::MenuItem("Exemplo 2 Camera", NULL, &opt_Camera);
				if (lastState != opt_Camera) {
					opt_ex1_spinningcube = false;
					opt_ex1_1_morecubes = false;
				}

				ImGui::EndMenu();

			}

			ImGui::EndMenuBar();

		}



		ImGui::Begin("Camera");
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Translate");
		ImGui::SliderFloat("X_c", &camera.Position.x, -5.0f, 5.0f);
		ImGui::SliderFloat("Y_c", &camera.Position.y, -5.0f, 5.0f);
		ImGui::SliderFloat("Z_c", &camera.Position.z, -5.0f, 5.0f);

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Adjust Focus");
		ImGui::SliderFloat("Focus", &camMatrix[0], 0.0f, 100.0f);

		ImGui::End();

		ImGui::Begin("Add");

		

		if (ImGui::CollapsingHeader("Available Items"))
		{
			if (ImGui::Selectable("Bunny")) 
				pickModel = "models/bunny/scene.gltf";
			if (ImGui::Selectable("Sword")) 
				pickModel = "models/sword/scene.gltf";
		
			
		}



		ImGui::Separator();

		if (ImGui::Button("Add to Scene"))
		{
			ModelShader addModel = { Model(pickModel.c_str()),  Shader("default.vert", "default.frag")};
			models.insert(models.end(), addModel);

			addModel.shaderProgram.Activate();
			glUniform4f(glGetUniformLocation(addModel.shaderProgram.ID, "lightColor"), lightColor[0], lightColor[1], lightColor[2], lightColor[4]);
			glUniform3f(glGetUniformLocation(addModel.shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(addModel.shaderProgram.ID, "rot"), addModel.rot[0], addModel.rot[1], addModel.rot[2]);
			glUniform1f(glGetUniformLocation(addModel.shaderProgram.ID, "size"), addModel.size);
			glUniform4f(glGetUniformLocation(addModel.shaderProgram.ID, "color1"), addModel.color[0], addModel.color[1], addModel.color[2], addModel.color[3]);
			glUniform3f(glGetUniformLocation(addModel.shaderProgram.ID, "trans"), addModel.trans[0], addModel.trans[1], addModel.trans[2]);
		}




		ImGui::End();




		// Create a window called "My First Tool", with a menu bar.
		ImGui::Begin("Nodes", &my_tool_active, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					saveImage(window);
				}
				if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Edit a color (stored as ~4 floats)
	

		// Plot some values
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Nodes");
		ImGui::BeginChild("Scrolling");
		for (unsigned int i = 0; i < models.size(); i++)
		{
			char name[] = ".gltf";
			if (ImGui::Selectable((std::to_string(models.at(i).shaderProgram.ID) + name).c_str()))
				active = i;

	
		}
		ImGui::EndChild();


	

		// Display contents in a scrolling region

	
		ImGui::End();

		
		ImGui::Begin("Control");
		ImGui::BeginGroup();

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Scale");
		ImGui::SliderFloat("Scale", &models.at(active).size, 0.5f, 2.0f);

		ImGui::Separator();

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Translate");


		ImGui::SliderFloat("X_t", &models.at(active).trans[0], -2.0f, 2.0f);
		ImGui::SliderFloat("Y_t", &models.at(active).trans[1], -2.0f, 2.0f);
		ImGui::SliderFloat("Z_t", &models.at(active).trans[2], -2.0f, 2.0f);

		ImGui::Separator();

		
	


		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Change Hue");
		ImGui::ColorEdit4("ColorHue", models.at(active).color);

		ImGui::EndGroup();

		ImGui::Separator();

		ImGui::BeginGroup();


		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Change Light");
		ImGui::ColorEdit4("ColorLight", lightColor);

		ImGui::Separator();

		
		ImGui::SliderFloat("X_r", &models.at(active).rot[0], -5.0f, 5.0f);
		ImGui::SliderFloat("Y_r", &models.at(active).rot[1], -5.0f, 5.0f);
		ImGui::SliderFloat("Z_r", &models.at(active).rot[2], -5.0f, 5.0f);

	
		ImGui::EndGroup();
		//ImGui::SliderFloat("LightColor", &models.at(active).trans[1], -2.0f, 2.0f);
		//glUniform4f(glGetUniformLocation(models.at(i).shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		//glUniform3f(glGetUniformLocation(models.at(i).shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z)

	
	/*	if (ImGui::Button("Save"))
		{

			saveImage(window);

		}*/
		ImGui::End();



		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		//glEnable(GL_DEPTH_TEST);

		//glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window, activateCamera);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(camMatrix[0], camMatrix[1], camMatrix[2]);

		// Draw a model

		for (unsigned int i = 0; i < models.size(); i++)
		{


			models.at(i).model.Draw(models.at(i).shaderProgram, camera);
			glUniform4f(glGetUniformLocation(models.at(i).shaderProgram.ID, "lightColor"), lightColor[0], lightColor[1], lightColor[2], lightColor[3]);
			glUniform3f(glGetUniformLocation(models.at(i).shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform1f(glGetUniformLocation(models.at(i).shaderProgram.ID, "size"), models.at(i).size);
			//glUniform1f(sizeID, size);
			glUniform3f(glGetUniformLocation(models.at(i).shaderProgram.ID, "rot"), models.at(i).rot[0], models.at(i).rot[1], models.at(i).rot[2]);
			glUniform4f(glGetUniformLocation(models.at(i).shaderProgram.ID, "color1"), models.at(i).color[0], models.at(i).color[1], models.at(i).color[2], models.at(i).color[3]);
			//glUniform4f(colorID, color[0], color[1], color[2], color[3]);
			glUniform3f(glGetUniformLocation(models.at(i).shaderProgram.ID, "trans"), models.at(i).trans[0], models.at(i).trans[1], models.at(i).trans[2]);

		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST);
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
		//glClear(GL_COLOR_BUFFER_BIT);






		ImGui::Begin("Viewport");

		// Get the size of the child (i.e. the whole draw size of the windows).
		ImVec2 wsize = ImGui::GetWindowSize();
		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		ImGui::Image((ImTextureID)textureColorbuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));


		ImGui::End();

		ImGui::End();






		ImGui::Render();


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		//glClear(GL_COLOR_BUFFER_BIT);




		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}


		glfwSwapBuffers(window);

		glfwPollEvents();


	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete all the objects we've created
	

	for (unsigned int i = 0; i < models.size(); i++)
	{


		models.at(i).shaderProgram.Delete();
		//model1.Draw(shaderProgram, camera);
	};


	glfwDestroyWindow(window);


	glfwTerminate();
	return 0;
}