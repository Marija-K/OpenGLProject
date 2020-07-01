#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <vector>




// a class that generates the platforms on the stage
class Stage {
	public:
	std::vector<glm::vec3> generatePlatforms();

	// Constructor
	Stage();

	void Init();

	static float boxfloor;
	static float boxceiling;
	static float leftboxwall;
	static float rightboxwall;
	static float backboxwall;
	static float frontboxwall;

	static float box[];

	int sizebot;
	int sizetop;
	int sizecheckbot;
	int sizechecktop;

	void setSizeBot(int size);
	void setSizeTop(int size);
	void setSizeCheckBot(int size);
	void setSizeCheckTop(int size);

};


