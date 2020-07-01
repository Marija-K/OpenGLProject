#include <Stage.hpp>

#include <GLFW\glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

Stage::Stage() {

}

void Stage::Init() {

}

void Stage::setSizeBot(int size)
{
	sizebot = size;
}

void Stage::setSizeTop(int size)
{
	sizetop = size;
}

void Stage::setSizeCheckBot(int size)
{
	sizecheckbot = size;
}

void Stage::setSizeCheckTop(int size)
{
	sizechecktop = size;
}

float Stage::boxfloor = -2.0f;
float Stage::boxceiling = -1.0f;
float Stage::leftboxwall = -0.5f;
float Stage::rightboxwall = 0.5f;
float Stage::backboxwall = -0.5f;
float Stage::frontboxwall = 0.5f;

float Stage::box []= {
			leftboxwall, boxfloor, backboxwall, 0.0f, 0.0f, rightboxwall, boxfloor, backboxwall, 1.0f, 0.0f,
			rightboxwall, boxceiling, backboxwall, 1.0f, 1.0f, rightboxwall, boxceiling, backboxwall, 1.0f, 1.0f,  //back wall
			leftboxwall, boxceiling, backboxwall, 0.0f, 1.0f, leftboxwall, boxfloor, backboxwall, 0.0f, 0.0f,

			leftboxwall, boxfloor, frontboxwall, 0.0f, 0.0f, rightboxwall, boxfloor, frontboxwall, 1.0f, 0.0f,
			rightboxwall, boxceiling, frontboxwall, 1.0f, 1.0f, rightboxwall, boxceiling, frontboxwall, 1.0f, 1.0f,  //front wall
			leftboxwall, boxceiling, frontboxwall, 0.0f, 1.0f, leftboxwall, boxfloor, frontboxwall, 0.0f, 0.0f,

			leftboxwall, boxceiling, frontboxwall, 0.0f, 0.0f, leftboxwall, boxceiling, backboxwall, 1.0f, 0.0f,
			leftboxwall, boxfloor, backboxwall, 1.0f, 1.0f, leftboxwall, boxfloor, backboxwall, 1.0f, 1.0f,  //left wall
			leftboxwall, boxfloor, frontboxwall, 0.0f, 1.0f, leftboxwall, boxceiling, frontboxwall, 0.0f, 0.0f,

			rightboxwall, boxceiling, frontboxwall, 0.0f, 0.0f, rightboxwall, boxceiling, backboxwall, 1.0f, 0.0f,
			rightboxwall, boxfloor, backboxwall, 1.0f, 1.0f, rightboxwall, boxfloor, backboxwall, 1.0f, 1.0f,  //right wall
			rightboxwall, boxfloor, frontboxwall, 0.0f, 1.0f, rightboxwall, boxceiling, frontboxwall, 0.0f, 0.0f,

			leftboxwall, boxceiling, backboxwall, 0.0f, 0.0f, rightboxwall, boxceiling, backboxwall, 1.0f, 0.0f,
			rightboxwall, boxceiling, frontboxwall, 1.0f, 1.0f, rightboxwall, boxceiling, frontboxwall, 1.0f, 1.0f,  //top wall
			leftboxwall, boxceiling, frontboxwall, 0.0f, 1.0f, leftboxwall, boxceiling, backboxwall, 0.0f, 0.0f,

			leftboxwall, boxfloor, backboxwall, 0.0f, 0.0f, rightboxwall, boxfloor, backboxwall, 1.0f, 0.0f,
			rightboxwall, boxfloor, frontboxwall, 1.0f, 1.0f, rightboxwall, boxfloor, frontboxwall, 1.0f, 1.0f,  //bottom wall
			leftboxwall, boxfloor, frontboxwall, 0.0f, 1.0f, leftboxwall, boxfloor, backboxwall, 0.0f, 0.0f

};

std::vector<glm::vec3> Stage::generatePlatforms() {
	srand(time(0));
	std::vector<glm::vec3> boxPositions;
	int sizebot = 1;
	int sizetop = 1;
	int sizecheckbot = 0;
	int sizechecktop = 0;
	int randnum = 4;

	boxPositions.push_back(glm::vec3(2.0f, 0.0f, 0.0f));

	for (float i = 0.0f; i < 100.0f; i += 1.0f) {
		for (float j = 0.0f; j < 5.0f; j += 1.0f) {
			if (i == 0.0f && j == 2.0f) {
				continue;
			}
			if (i == 20.0f || i == 40.0f || i == 60.0f || i == 80.0f) {
				continue;
			}
			if ((rand() % 10) + 1 < randnum)
			{
				continue;
			}
			else {
				boxPositions.push_back(glm::vec3(j, 0.0f, -i));
				sizebot++;
			}			
		}
		if ((int)i%20 == 0)
			{
				randnum++;
			}
	}

	randnum = 4;

	boxPositions.push_back(glm::vec3(2.0f, 11.0f, 0.0f));

	for (float i = 0.0f; i < 100.0f; i += 1.0f) {
		for (float j = 0.0f; j < 5.0f; j += 1.0f) {
			if (i == 0.0f && j == 2.0f) {
				continue;
			}
			if (i == 20.0f || i == 40.0f || i == 60.0f || i == 80.0f) {
				continue;
			}
			if ((rand() % 10) + 1 < randnum)
			{
				continue;
			}
			else {
				boxPositions.push_back(glm::vec3(j, 11.0f, -i));
				sizetop++;
			}
		}
		if ((int)i % 20 == 0)
		{
			randnum++;
		}
	}

	for (float i = 20.0f; i <= 100.0f; i += 20.0f) {
		for (float j = 0.0f; j < 5.0f; j += 1.0f) {
			boxPositions.push_back(glm::vec3(j, 0.0f, -i));
			sizecheckbot++;
		}
	}
	for (float i = 101.0f; i < 105.0f; i += 1.0f) {
		for (float j = 0.0f; j < 5.0f; j += 1.0f) {
			boxPositions.push_back(glm::vec3(j, 0.0f, -i));
			sizecheckbot++;
		}
	}

	for (float i = 20.0f; i <= 100.0f; i += 20.0f) {
		for (float j = 0.0f; j < 5.0f; j += 1.0f) {
			boxPositions.push_back(glm::vec3(j, 11.0f, -i));
			sizechecktop++;
		}
	}

	for (float i = 101.0f; i < 105.0f; i += 1.0f) {
		for (float j = 0.0f; j < 5.0f; j += 1.0f) {
			boxPositions.push_back(glm::vec3(j, 11.0f, -i));
			sizechecktop++;
		}
	}

	setSizeBot(sizebot);
	setSizeTop(sizetop);
	setSizeCheckBot(sizecheckbot);
	setSizeCheckTop(sizechecktop);



	return boxPositions;
}



