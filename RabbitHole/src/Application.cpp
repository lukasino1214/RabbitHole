#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main(void) {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 576, "RabbitHole | 0.1.4 ", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //glfwSwapInterval(1); limitace FPS

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f
        };

        unsigned int indices[] = {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
        };


        VertexArray va;
        VertexBuffer vb(positions, 3 * 8 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        //layout.Push<float>(2); Pro textury
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 36);

        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        //Texture texture("res/textures/meme.png");
        //texture.Bind(0);
        //shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;
        Camera camera;


        const char* glsl_version = "#version 330 core";

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        glm::vec3 translation(0, 0, 0);

        //glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); 2D režim

        glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 4000.0f);
        glm::mat4 View = glm::lookAt(glm::vec3(4, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // vektory kamera, origin a pohled
        glm::mat4 Model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = Projection * View * Model;

        /*float r = 0.0f;
        float increment = 0.05f;*/

        /*float mouseSpeed = 0.005f;
        float horizontalAngle = 3.14f;
        float verticalAngle = 0.0f;
        float speed = 0.005f;
        glm::vec3 position = glm::vec3(3, -4, -3);*/

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        std::vector<glm::vec3> vec;
        vec.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

        /*for (int i = 0; i < 2001; i++) {
            vec.push_back(glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec1(i*2));
            //std::cout << glm::to_string(vec[i]) << std::endl;
        }*/


        int magic_number = 10;
        std::vector<glm::vec3> cube;
        for (int x = 0; x < magic_number; x++) {
            for (int y = 0; y < magic_number; y++) {
                for (int z = 0; z < magic_number; z++) {
                    cube.push_back(glm::vec3(1.0f*2*x, 1.0f*2*y, 1.0f*2*z));
                }
            }
        }
          
        while (!glfwWindowShouldClose(window)) {



            View = camera.movement(window);

            processInput(window);

            renderer.Clear();

            //glm::vec3 translation = vec[0];
            //std::cout << glm::to_string(translation) << std::endl;

            /*for (int i = 0; i < 2001; ++i) {
                //std::cout << i << std::endl;
                //std::cout << glm::to_string(vec[i]) << std::endl;
                glm::mat4 Model = glm::translate(glm::mat4(1.0f), vec[i]);
                glm::mat4 mvp = Projection * View * Model;
                shader.Bind(va
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }*/

            for (int i = 0; i < magic_number*magic_number*magic_number; i++) {
                glm::mat4 Model = glm::translate(glm::mat4(1.0f), cube[i]);
                glm::mat4 mvp = Projection * View * Model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }
            
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            /*GLuint my_opengl_texture; generogani textury nefunguje :kekw:
            glGenTextures(1, &my_opengl_texture);*/


            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glm::mat4 Model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 mvp = Projection * View * Model;

            shader.Bind();
            shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
            shader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, ib, shader);

            /*if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;*/

            //ImGui::Image((void*)(intptr_t)my_opengl_texture, ImVec2(512, 512)); opengl textura z framebufferu

            static float f = 0.0f;
            static int counter = 0;

            //ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable; peo docking

            ImGui::SliderFloat3("Translation", &translation.x, -20.0f, 20.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}