#pragma once

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class Camera {
public:
	//Controls(GLFWwindow* window);
	glm::mat4 movement(GLFWwindow* window);
};