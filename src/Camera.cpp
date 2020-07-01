#include <Camera.hpp>
#include <GLFW\glfw3.h>
#include <iostream>



Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
               float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
  Position = glm::vec3(posX, posY, posZ);
  WorldUp = glm::vec3(upX, upY, upZ);
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

void Camera::Init() {
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

// Processes input received from any keyboard-like input system. Accepts input
// parameter in the form of camera defined ENUM (to abstract it from windowing
// systems)
void Camera::ProcessKeyboard(int key, int action) {

	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS)
			direction[0] = true;
		else if (action == GLFW_RELEASE)
			direction[0] = false;
	}
	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS)
			direction[1] = true;
		else if (action == GLFW_RELEASE)
			direction[1] = false;
	}
	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS)
			direction[2] = true;
		else if (action == GLFW_RELEASE)
			direction[2] = false;
	}
	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS)
			direction[3] = true;
		else if (action == GLFW_RELEASE)
			direction[3] = false;
	}

	if (key == GLFW_KEY_SPACE) {
		if (action == GLFW_PRESS) {
			if (!inAir)
			{
				Reverse = !Reverse;			
				velocity.y = 8.0f;
				direction[4] = true;
				Pitch = -Pitch;
			}				
		}
	}
}

void Camera::Update(float deltaTime) {
	velocity.x = MovementSpeed * deltaTime;
	velocity.z = MovementSpeed * deltaTime;
	
	
	if (direction[4]) {
		accelerationDOWN -= GRAVITY * deltaTime;
		velocity.y += accelerationDOWN * deltaTime;
	}

    CheckPosition();
	Position.y = velocity.y;
	
	if (direction[0])
			Position += glm::normalize(glm::cross(WorldUp, Right)) * velocity.z;
	if (direction[1])
			Position -= glm::normalize(glm::cross(WorldUp, Right)) * velocity.z;

	if (direction[2])
		Position -= Right * velocity.x;
	if (direction[3])
		Position += Right * velocity.x;
	updateCameraVectors();
}

void Camera::CheckPosition() {

	if (Fall)
	{
		direction[4] = true;
	}
	else if (velocity.y <= Floor) {
		inAir = false;
		velocity.y = Floor;
		direction[4] = false;
		accelerationDOWN = 0.0f;

	}
	if (velocity.y < -10.0f)
	{
		

		if (Position.z <= -80.0f) {

			Position.z = -80.0f;

		}
		else if (Position.z <= -60.0f) {

			Position.z = -60.0f;

		}
		else if (Position.z <= -40.0f) {

			Position.z = -40.0f;

		}
		else if (Position.z <= -20.0f) {

			Position.z = -20.0f;

		}
		else {			
			Position.z = 0.0f;			
		}
		velocity.y = Floor;
		Position.x = 2.0f;	
		Pitch = 0.0f;
		Fall = false;
		Yaw = -90.0f;
		if (Reverse)
		{
			Reverse = !Reverse;
		}
		
	}
	if (velocity.y > Floor && velocity.y < Ceiling || velocity.y < Floor)
	{
		inAir = true;
	}
	
}

void Camera::CheckFall(std::vector<glm::vec3> holes, int botsize, int topsize, int botsizecheck, int topsizecheck) {
			if (velocity.y < Floor - 0.5f)
			{
				Fall = true;
			}
			else
			{
				Fall = !onBlock(holes, botsize, topsize, botsizecheck, topsizecheck);
			}
}

bool Camera::onBlock(std::vector<glm::vec3> holes, int botsize, int topsize, int botsizecheck, int topsizecheck) {
	bool onBlock = false;
	if (!Reverse) {
		for (int i = 0; i < botsize; i++) {
				if (Position.z <= (holes[i].z + 0.5f) && Position.z >= (holes[i].z - 0.5f) && Position.x <= (holes[i].x + 0.5f) && Position.x >= (holes[i].x - 0.5f)) {
					onBlock = true;
				}	
		}
		for (int i = botsize + topsize; i < botsize + topsize + botsizecheck; i++) {
			if (Position.z <= (holes[i].z + 0.5f) && Position.z >= (holes[i].z - 0.5f) && Position.x <= (holes[i].x + 0.5f) && Position.x >= (holes[i].x - 0.5f)) {
				onBlock = true;
			}
		}
	}
	else if (Reverse) {
		for (int i = botsize; i < botsize + topsize; i++) {
				if (Position.z <= (holes[i].z + 0.5f) && Position.z >= (holes[i].z - 0.5f) && Position.x <= (holes[i].x + 0.5f) && Position.x >= (holes[i].x - 0.5f)) {
					onBlock = true;
				}
		}
		for (int i = botsize + topsize + botsizecheck; i < botsize + topsize + botsizecheck + topsizecheck; i++) {
			if (Position.z <= (holes[i].z + 0.5f) && Position.z >= (holes[i].z - 0.5f) && Position.x <= (holes[i].x + 0.5f) && Position.x >= (holes[i].x - 0.5f)) {
				onBlock = true;
			}
		}
	}
	return onBlock;
}

void Camera::CheckCollision(std::vector<glm::vec3> holes, int botsize, int topsize, int botsizecheck, int topsizecheck) {
	if (!Reverse) {
		for (int i = 0; i < botsize; i++) {
			if (Position.z >= (holes[i].z + 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[0]==true) {
				Position.z -= 0.02f;
				direction[0] = false;
			}
			if (Position.z <= (holes[i].z - 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[1] == true) {
				Position.z += 0.02f;
				direction[1] = false;
			}
			if (Position.x >= (holes[i].x + 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[2] == true) {
				Position.x -= 0.02f;
				direction[2] = false;
			}
			if (Position.x <= (holes[i].x - 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[3] == true) {
				Position.x += 0.02f;
				direction[3] = false;
			}
		}		
		for (int i = botsize + topsize; i < botsize + topsize + botsizecheck; i++) {
			if (Position.z >= (holes[i].z + 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[0] == true) {
				Position.z -= 0.02f;
				direction[0] = false;
			}
			if (Position.z <= (holes[i].z - 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[1] == true) {
				Position.z += 0.02f;
				direction[1] = false;
			}
			if (Position.x >= (holes[i].x + 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[2] == true) {
				Position.x -= 0.02f;
				direction[2] = false;
			}
			if (Position.x <= (holes[i].x - 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[3] == true) {
				Position.x += 0.02f;
				direction[3] = false;
			}
		}

	}
	else if (Reverse) {
		for (int i = botsize; i < botsize + topsize; i++) {
			if (Position.z >= (holes[i].z + 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[0] == true) {
				Position.z -= 0.02f;
				direction[0] = false;
			}
			if (Position.z <= (holes[i].z - 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[1] == true) {
				Position.z += 0.02f;
				direction[1] = false;
			}
			if (Position.x >= (holes[i].x + 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[2] == true) {
				Position.x -= 0.02f;
				direction[2] = false;
			}
			if (Position.x <= (holes[i].x - 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[3] == true) {
				Position.x += 0.02f;
				direction[3] = false;
			}
		}
		for (int i = botsize + topsize + botsizecheck; i < botsize + topsize + botsizecheck + topsizecheck; i++) {
			if (Position.z >= (holes[i].z + 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[0] == true) {
				Position.z -= 0.02f;
				direction[0] = false;
			}
			if (Position.z <= (holes[i].z - 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[1] == true) {
				Position.z += 0.02f;
				direction[1] = false;
			}
			if (Position.x >= (holes[i].x + 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[2] == true) {
				Position.x -= 0.02f;
				direction[2] = false;
			}
			if (Position.x <= (holes[i].x - 0.5f) && velocity.y < Floor - 0.5f && velocity.y > Floor - 2.5f && direction[3] == true) {
				Position.x += 0.02f;
				direction[3] = false;
			}
		}
	}
}

// Processes input received from a mouse input system. Expects the offset
// value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset,
                                  GLboolean constrainPitch) {
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw += xoffset; 
  Pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch) {
    if (Pitch > PitchConstraint)
      Pitch = PitchConstraint;
    if (Pitch < -PitchConstraint)
      Pitch = -PitchConstraint;
  }

  // Update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires
// input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset) {
  if (Zoom >= 1.0f && Zoom <= 45.0f)
    Zoom -= yoffset;
  if (Zoom <= 1.0f)
    Zoom = 1.0f;
  if (Zoom >= 45.0f)
    Zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors() {
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = static_cast<float>(cos(glm::radians(static_cast<double>(Yaw))) *
							   cos(glm::radians(static_cast<double>(Pitch))));
  front.y = static_cast<float>(sin(glm::radians(static_cast<double>(Pitch))));
  front.z = static_cast<float>(sin(glm::radians(static_cast<double>(Yaw))) *
                               cos(glm::radians(static_cast<double>(Pitch))));
  Front = glm::normalize(front);
  // Also re-calculate the Right and Up vector
  Right = glm::normalize(glm::cross(
      Front, WorldUp)); // Normalize the vectors, because their length gets
                        // closer to 0 the more you look up or down which
                        // results in slower movement.
  Up = glm::normalize(glm::cross(Right, Front));
}
