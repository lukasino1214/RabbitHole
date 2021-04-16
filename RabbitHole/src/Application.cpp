#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <algorithm>
#include <utility>
#include <set>

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

unsigned int UP_FACE[6] = { 4, 5, 0, 0, 5, 1 };
unsigned int DOWN_FACE[6] = { 3, 2, 7, 7, 2, 6 };
unsigned int BACKWARD_FACE[6] = { 4, 0, 7, 7, 0, 3 };
unsigned int RIGHT_FACE[6] = { 0, 1, 3, 3, 1, 2 };
unsigned int FORWARD_FACE[6] = { 1, 5, 2, 2, 5, 6 };
unsigned int LEFT_FACE[6] = { 5, 4, 6, 6, 4, 7 };

namespace Voxel {
    bool FindVoxel(std::vector<glm::vec3> voxels, glm::vec3 voxel) {

        if (std::find(voxels.begin(), voxels.end(), voxel) != voxels.end()) {
            return true;
        }

        else {
            return false;
        }
    }

    std::vector<unsigned int> VoxelIndexBuffer(std::vector<glm::vec3> voxels, glm::vec3 voxel) {

        std::vector<unsigned int> voxel_ib;

        if (Voxel::FindVoxel(voxels, voxel + glm::vec3(0.0f, -2.0f, 0.0f)) == false) {
            voxel_ib.insert(voxel_ib.end(), UP_FACE, UP_FACE + 6);
        }

        if (Voxel::FindVoxel(voxels, voxel + glm::vec3(0.0f, 2.0f, 0.0f)) == false) {
            voxel_ib.insert(voxel_ib.end(), DOWN_FACE, DOWN_FACE + 6);
        }

        if (Voxel::FindVoxel(voxels, voxel + glm::vec3(0.0f, 0.0f, 2.0f)) == false) {
            voxel_ib.insert(voxel_ib.end(), LEFT_FACE, LEFT_FACE + 6);
        }

        if (Voxel::FindVoxel(voxels, voxel + glm::vec3(0.0f, 0.0f, -2.0f)) == false) {
            voxel_ib.insert(voxel_ib.end(), RIGHT_FACE, RIGHT_FACE + 6);
        }

        if (Voxel::FindVoxel(voxels, voxel + glm::vec3(2.0f, 0.0f, 0.0f)) == false) {
            voxel_ib.insert(voxel_ib.end(), FORWARD_FACE, FORWARD_FACE + 6);
        }

        if (Voxel::FindVoxel(voxels, voxel + glm::vec3(-2.0f, 0.0f, 0.0f)) == false) {
            voxel_ib.insert(voxel_ib.end(), BACKWARD_FACE, BACKWARD_FACE + 6);
        }

        return voxel_ib;
    }

    /*std::set<std::pair<std::vector<int>, >> VoxelCache(){

    */
}


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main(void) {
    GLFWwindow* window;
    int screen_width = 1024;
    int screen_height = 576;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(screen_width, screen_height, "RabbitHole | 0.1.4 ", NULL, NULL);
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
            0, 1, 3, 3, 1, 2, //leva strana
            1, 5, 2, 2, 5, 6, //prava strana
            5, 4, 6, 6, 4, 7, //vpredu strana
            4, 0, 7, 7, 0, 3, //vzadu strana
            3, 2, 7, 7, 2, 6, //dole strana
            4, 5, 0, 0, 5, 1 //nahore strana
        };

        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
       // positions   // texCoords
       -1.0f,  1.0f,  0.0f, 1.0f,
       -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

       -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
        };


        VertexArray va;
        VertexBuffer vb(positions, 3 * 8 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        //layout.Push<float>(2); Pro textury
        va.AddBuffer(vb, layout);

        //IndexBuffer ib(indices, 36);

        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        //Texture texture("res/textures/meme.png");
        //texture.Bind(0);
        //shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        //ib.Unbind();
        shader.Unbind();

        Shader screenshader("res/shaders/frame.shader");
        screenshader.Bind();
        screenshader.SetUniform1i("screenTexture", 0);

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
        glm::mat4 View = glm::lookAt(glm::vec3(4, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // vektory kamera, origin a pohled scale(1, -1, 1)
        glm::mat4 Model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = Projection * View * Model;

        unsigned int quadVAO, quadVBO;
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        std::vector<glm::vec3> vec;
        vec.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

        int magic_number = 8;
        std::vector<glm::vec3> cube;
        for (std::size_t x = 0; x < magic_number; x++) {
            for (std::size_t y = 0; y < magic_number; y++) {
                for (std::size_t z = 0; z < magic_number; z++) {
                    cube.push_back(glm::vec3(1.0f*2*x, 1.0f*2*y, 1.0f*2*z));
                }
            }
        }

        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // create a color attachment texture
        unsigned int textureColorbuffer;
        glGenTextures(1, &textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 576); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        while (!glfwWindowShouldClose(window)) {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

            // make sure we clear the framebuffer's content
            //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            renderer.Clear(); //<--------- zapnout
            View = camera.movement(window);
            //std::cout << glm::to_string(View) << std::endl;
            processInput(window);

            for (std::size_t i = 0; i < cube.size(); i++) {
                //auto indices_voxel = searchvoxel(cube, cube[i]);
                auto indices_voxel = Voxel::VoxelIndexBuffer(cube, cube[i]);
                int size = indices_voxel.size();

                if (0 < size) {
                    IndexBuffer ib(indices_voxel, size);
                    ib.Bind();
                    glm::mat4 Model = glm::translate(glm::mat4(1.0f), cube[i]);
                    glm::mat4 mvp = Projection * View * Model;
                    shader.Bind();
                    shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
                    shader.SetUniformMat4f("u_MVP", mvp);
                    renderer.Draw(va, ib, shader);
                    ib.Unbind();
                    shader.Unbind();
                }
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
            // clear all relevant buffers
            //glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
            glClear(GL_COLOR_BUFFER_BIT);

            /*screenshader.Bind();
            glBindVertexArray(quadVAO);
            glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
            glDrawArrays(GL_TRIANGLES, 0, 6);

            /* screenshader("res/shaders/frame.shader");
            screenshader.Bind();*/
            /*screenshader.Unbind();*/

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            static float f = 0.0f;
            static int counter = 0;

            //ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable; peo docking

            ImGui::SliderFloat3("Translation", &translation.x, -20.0f, 20.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Image((void*)(intptr_t)textureColorbuffer, ImVec2(screen_width/2, screen_height/2), ImVec2(0, 1), ImVec2(1, 0));


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}