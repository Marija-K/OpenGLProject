#include <OpenGLPrj.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Camera.hpp>
#include <Shader.hpp>
#include <Stage.hpp>

#include <iostream>
#include <string>
#include <vector>

const std::string program_name = ("Gravity Platformer");

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(std::vector<std::string> faces);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
static Camera camera(glm::vec3(2.0f, 0.0f, 0.0f));
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

// platforms
static Stage stage;



// timing
static float deltaTime = 0.0f; // time between current frame and last frame
static float lastFrame = 0.0f;


int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(
      GLFW_OPENGL_FORWARD_COMPAT,
      GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                        program_name.c_str(), nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // build and compile our shader program
  // ------------------------------------
  std::string shader_location("../res/shaders/");

  std::string used_shaders("shader");
  std::string cubemap_shaders("cubemap");

  Shader ourShader(shader_location + used_shaders + std::string(".vert"),
                   shader_location + used_shaders + std::string(".frag"));

  Shader cubemapShader(shader_location + cubemap_shaders + std::string(".vert"),
				   shader_location + cubemap_shaders + std::string(".frag"));

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------

  /*float floor = -1.0f;
  float ceiling = 9.0f;
  float leftwall = -5.0f;
  float rightwall = 5.0f;
  float backwall = -5.0f;
  float frontwall = 5.0f;
  

  float points[] = {
	  leftwall, floor, backwall, 0.0f, 0.0f, rightwall, floor, backwall, 1.0f, 0.0f,
	  rightwall, ceiling, backwall, 1.0f, 1.0f, rightwall, ceiling, backwall, 1.0f, 1.0f,  //back wall
	  leftwall, ceiling, backwall, 0.0f, 1.0f, leftwall, floor, backwall, 0.0f, 0.0f,

	  leftwall, floor, frontwall, 0.0f, 0.0f, rightwall, floor, frontwall, 1.0f, 0.0f,
	  rightwall, ceiling, frontwall, 1.0f, 1.0f, rightwall, ceiling, frontwall, 1.0f, 1.0f,  //front wall
	  leftwall, ceiling, frontwall, 0.0f, 1.0f, leftwall, floor, frontwall, 0.0f, 0.0f,

	  leftwall, ceiling, frontwall, 0.0f, 0.0f, leftwall, ceiling, backwall, 1.0f, 0.0f,
	  leftwall, floor, backwall, 1.0f, 1.0f, leftwall, floor, backwall, 1.0f, 1.0f,  //left wall
	  leftwall, floor, frontwall, 0.0f, 1.0f, leftwall, ceiling, frontwall, 0.0f, 0.0f,

	  rightwall, ceiling, frontwall, 0.0f, 0.0f, rightwall, ceiling, backwall, 1.0f, 0.0f,
	  rightwall, floor, backwall, 1.0f, 1.0f, rightwall, floor, backwall, 1.0f, 1.0f,  //right wall
	  rightwall, floor, frontwall, 0.0f, 1.0f, rightwall, ceiling, frontwall, 0.0f, 0.0f,

	  leftwall, ceiling, backwall, 0.0f, 0.0f, rightwall, ceiling, backwall, 1.0f, 0.0f,
	  rightwall, ceiling, frontwall, 1.0f, 1.0f, rightwall, ceiling, frontwall, 1.0f, 1.0f,  //top wall
	  leftwall, ceiling, frontwall, 0.0f, 1.0f, leftwall, ceiling, backwall, 0.0f, 0.0f,

	  leftwall, floor, backwall, 0.0f, 0.0f, rightwall, floor, backwall, 1.0f, 0.0f,
	  rightwall, floor, frontwall, 1.0f, 1.0f, rightwall, floor, frontwall, 1.0f, 1.0f,  //bottom wall
	  leftwall, floor, frontwall, 0.0f, 1.0f, leftwall, floor, backwall, 0.0f, 0.0f

  };

  float vertices[] = {
	  //point              //texture   //point              //texture
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  // back face
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // front face
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // left face
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  // right face
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  // top face
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // bottom face
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};*/

  


  float skyboxVertices[] = {
	  // positions          
	  -1.0f,  1.0f, -1.0f,
	  -1.0f, -1.0f, -1.0f,
	   1.0f, -1.0f, -1.0f,
	   1.0f, -1.0f, -1.0f,
	   1.0f,  1.0f, -1.0f,
	  -1.0f,  1.0f, -1.0f,

	  -1.0f, -1.0f,  1.0f,
	  -1.0f, -1.0f, -1.0f,
	  -1.0f,  1.0f, -1.0f,
	  -1.0f,  1.0f, -1.0f,
	  -1.0f,  1.0f,  1.0f,
	  -1.0f, -1.0f,  1.0f,

	   1.0f, -1.0f, -1.0f,
	   1.0f, -1.0f,  1.0f,
	   1.0f,  1.0f,  1.0f,
	   1.0f,  1.0f,  1.0f,
	   1.0f,  1.0f, -1.0f,
	   1.0f, -1.0f, -1.0f,

	  -1.0f, -1.0f,  1.0f,
	  -1.0f,  1.0f,  1.0f,
	   1.0f,  1.0f,  1.0f,
	   1.0f,  1.0f,  1.0f,
	   1.0f, -1.0f,  1.0f,
	  -1.0f, -1.0f,  1.0f,

	  -1.0f,  1.0f, -1.0f,
	   1.0f,  1.0f, -1.0f,
	   1.0f,  1.0f,  1.0f,
	   1.0f,  1.0f,  1.0f,
	  -1.0f,  1.0f,  1.0f,
	  -1.0f,  1.0f, -1.0f,

	  -1.0f, -1.0f, -1.0f,
	  -1.0f, -1.0f,  1.0f,
	   1.0f, -1.0f, -1.0f,
	   1.0f, -1.0f, -1.0f,
	  -1.0f, -1.0f,  1.0f,
	   1.0f, -1.0f,  1.0f
  };



  std::vector<glm::vec3> boxPositions;
  std::vector<glm::vec3> cubePositions{
	  glm::vec3(0.7f, 6.0f, -115.0f),
	  glm::vec3(0.9f, 6.0f, -115.0f),
	  glm::vec3(0.5f, 5.8f, -115.0f),
	  glm::vec3(1.1f, 5.8f, -115.0f),
	  glm::vec3(0.5f, 5.6f, -115.0f),
	  glm::vec3(0.5f, 5.4f, -115.0f),
	  glm::vec3(0.5f, 5.4f, -115.0f),
	  glm::vec3(0.9f, 5.4f, -115.0f),
	  glm::vec3(1.1f, 5.4f, -115.0f),
	  glm::vec3(0.5f, 5.2f, -115.0f),
	  glm::vec3(1.1f, 5.2f, -115.0f),
	  glm::vec3(0.7f, 5.0f, -115.0f),
	  glm::vec3(0.9f, 5.0f, -115.0f),  // G
	  glm::vec3(1.7f, 6.0f, -115.0f),
	  glm::vec3(1.9f, 6.0f, -115.0f),
	  glm::vec3(1.5f, 5.8f, -115.0f),
	  glm::vec3(2.1f, 5.8f, -115.0f),
	  glm::vec3(1.5f, 5.6f, -115.0f),
	  glm::vec3(2.1f, 5.6f, -115.0f),
	  glm::vec3(1.5f, 5.4f, -115.0f),
	  glm::vec3(1.5f, 5.4f, -115.0f),
	  glm::vec3(2.1f, 5.4f, -115.0f),
	  glm::vec3(1.5f, 5.2f, -115.0f),
	  glm::vec3(2.1f, 5.2f, -115.0f),
	  glm::vec3(1.7f, 5.0f, -115.0f),
	  glm::vec3(1.9f, 5.0f, -115.0f), // first O
	  glm::vec3(2.7f, 6.0f, -115.0f),
	  glm::vec3(2.9f, 6.0f, -115.0f),
	  glm::vec3(2.5f, 5.8f, -115.0f),
	  glm::vec3(3.1f, 5.8f, -115.0f),
	  glm::vec3(2.5f, 5.6f, -115.0f),
	  glm::vec3(3.1f, 5.6f, -115.0f),
	  glm::vec3(2.5f, 5.4f, -115.0f),
	  glm::vec3(2.5f, 5.4f, -115.0f),
	  glm::vec3(3.1f, 5.4f, -115.0f),
	  glm::vec3(2.5f, 5.2f, -115.0f),
	  glm::vec3(3.1f, 5.2f, -115.0f),
	  glm::vec3(2.7f, 5.0f, -115.0f),
	  glm::vec3(2.9f, 5.0f, -115.0f), // second O
	  glm::vec3(3.5f, 6.0f, -115.0f),
	  glm::vec3(3.7f, 6.0f, -115.0f),
	  glm::vec3(3.9f, 6.0f, -115.0f),
	  glm::vec3(3.5f, 5.8f, -115.0f),
	  glm::vec3(4.1f, 5.8f, -115.0f),
	  glm::vec3(3.5f, 5.6f, -115.0f),
	  glm::vec3(4.1f, 5.6f, -115.0f),
	  glm::vec3(3.5f, 5.4f, -115.0f),
	  glm::vec3(3.5f, 5.4f, -115.0f),
	  glm::vec3(4.1f, 5.4f, -115.0f),
	  glm::vec3(3.5f, 5.2f, -115.0f),
	  glm::vec3(4.1f, 5.2f, -115.0f),
	  glm::vec3(3.5f, 5.0f, -115.0f),
	  glm::vec3(3.7f, 5.0f, -115.0f),
	  glm::vec3(3.9f, 5.0f, -115.0f), // D
	  glm::vec3(0.7f, 4.6f, -115.0f),
	  glm::vec3(0.9f, 4.6f, -115.0f),
	  glm::vec3(1.1f, 4.6f, -115.0f),
	  glm::vec3(1.1f, 4.4f, -115.0f),
	  glm::vec3(1.1f, 4.2f, -115.0f),
	  glm::vec3(1.1f, 4.0f, -115.0f),
	  glm::vec3(0.5f, 3.8f, -115.0f),
	  glm::vec3(1.1f, 3.8f, -115.0f),
	  glm::vec3(0.7f, 3.6f, -115.0f),
	  glm::vec3(0.9f, 3.6f, -115.0f), //J
	  glm::vec3(1.7f, 4.6f, -115.0f),
	  glm::vec3(1.9f, 4.6f, -115.0f),
	  glm::vec3(1.5f, 4.4f, -115.0f),
	  glm::vec3(2.1f, 4.4f, -115.0f),
	  glm::vec3(1.5f, 4.2f, -115.0f),
	  glm::vec3(2.1f, 4.2f, -115.0f),
	  glm::vec3(1.5f, 4.0f, -115.0f),
	  glm::vec3(1.5f, 4.0f, -115.0f),
	  glm::vec3(2.1f, 4.0f, -115.0f),
	  glm::vec3(1.5f, 3.8f, -115.0f),
	  glm::vec3(2.1f, 3.8f, -115.0f),
	  glm::vec3(1.7f, 3.6f, -115.0f),
	  glm::vec3(1.9f, 3.6f, -115.0f), // third O
	  glm::vec3(2.5f, 4.6f, -115.0f),
	  glm::vec3(2.7f, 4.6f, -115.0f),
	  glm::vec3(2.9f, 4.6f, -115.0f),
	  glm::vec3(2.5f, 4.4f, -115.0f),
	  glm::vec3(3.1f, 4.4f, -115.0f),
	  glm::vec3(2.5f, 4.2f, -115.0f),
	  glm::vec3(2.7f, 4.2f, -115.0f),
	  glm::vec3(2.9f, 4.2f, -115.0f),
	  glm::vec3(2.5f, 4.0f, -115.0f),
	  glm::vec3(3.1f, 4.0f, -115.0f),
	  glm::vec3(2.5f, 3.8f, -115.0f),
	  glm::vec3(3.1f, 3.8f, -115.0f),
	  glm::vec3(2.5f, 3.6f, -115.0f),
	  glm::vec3(2.7f, 3.6f, -115.0f),
	  glm::vec3(2.9f, 3.6f, -115.0f), // B
	  glm::vec3(3.5f, 4.6f, -115.0f),
	  glm::vec3(3.9f, 4.6f, -115.0f),
	  glm::vec3(4.3f, 4.6f, -115.0f),
	  glm::vec3(3.5f, 4.4f, -115.0f),
	  glm::vec3(3.9f, 4.4f, -115.0f),
	  glm::vec3(4.3f, 4.4f, -115.0f),
	  glm::vec3(3.5f, 4.2f, -115.0f),
	  glm::vec3(3.9f, 4.2f, -115.0f),
	  glm::vec3(4.3f, 4.2f, -115.0f),
	  glm::vec3(3.5f, 4.0f, -115.0f),
	  glm::vec3(3.9f, 4.0f, -115.0f),
	  glm::vec3(4.3f, 4.0f, -115.0f),
	  glm::vec3(3.5f, 3.6f, -115.0f),
	  glm::vec3(3.9f, 3.6f, -115.0f),
	  glm::vec3(4.3f, 3.6f, -115.0f) // !!!

  };
  
  
  boxPositions = stage.generatePlatforms();

  int sizebot = stage.sizebot;
  int sizetop = stage.sizetop;
  int sizecheckbot = stage.sizecheckbot;
  int sizechecktop = stage.sizechecktop;

  float box [180];

  for (int i = 0; i < 180; i++) {
	  box[i] = stage.box[i];
  }
     
  

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int skyboxVAO, skyboxVBO;
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  
	std::vector<std::string> faces
	  {
		  "../res/textures/Starscape.jpg",
		  "../res/textures/Starscape.jpg",
		  "../res/textures/Starscape.jpg",
		  "../res/textures/Starscape.jpg",
		  "../res/textures/Starscape.jpg",
		  "../res/textures/Starscape.jpg"
	  };
  unsigned int cubemapTexture = loadCubemap(faces);

  // load and create a texture
  // -------------------------
  unsigned int texture1, texture2, texture3, texture4;
  // texture 1
  // ---------
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(
      true); // tell stb_image.h to flip loaded texture's on the y-axis.
  // The FileSystem::getPath(...) is part of the GitHub repository so we can
  // find files on any IDE/platform; replace it with your own image path.
  unsigned char *data = stbi_load("../res/textures/red block.jpg", &width,
                                  &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  // texture 2
  // ---------
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  data = stbi_load("../res/textures/blue block.jpg", &width, &height,
                   &nrChannels, 0);
  if (data) {
    // note that the awesomeface.png has transparency and thus an alpha channel,
    // so make sure to tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  

  glGenTextures(1, &texture3);
  glBindTexture(GL_TEXTURE_2D, texture3);
  // set the texture wrapping parameters
  glTexParameteri(
	  GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
	  GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  data = stbi_load("../res/textures/green block.jpg", &width, &height,
	  &nrChannels, 0);
  if (data) {
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		  GL_UNSIGNED_BYTE, data);
	  glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
	  std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  glGenTextures(1, &texture4);
  glBindTexture(GL_TEXTURE_2D, texture4);
  // set the texture wrapping parameters
  glTexParameteri(
	  GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
	  GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  data = stbi_load("../res/textures/test block.jpg", &width, &height,
	  &nrChannels, 0);
  if (data) {
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		  GL_UNSIGNED_BYTE, data);
	  glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
	  std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  // tell opengl for each sampler to which texture unit it belongs to (only has
  // to be done once)
  // -------------------------------------------------------------------------------------------
  ourShader.use();
  ourShader.setInt("texture1", 0);

  cubemapShader.use();
  cubemapShader.setInt("texture1", 0);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // per-frame time logic
    // --------------------
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
	camera.Update(deltaTime);
	

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    

    // activate shader
	ourShader.use();

    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.Zoom), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);
	

    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("view", view);

	
	
	camera.CheckFall(boxPositions, sizebot, sizetop, sizecheckbot, sizechecktop);
	camera.CheckCollision(boxPositions, sizebot, sizetop, sizecheckbot, sizechecktop);

    // render boxes
    glBindVertexArray(VAO);
	for (unsigned int i = 0; i < sizebot; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, boxPositions[i]);
		if(camera.Reverse)
		model = glm::translate(model, glm::vec3(0.0f, 11.0f, 0.0f));
		ourShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture2);

	ourShader.use();

	for (unsigned int i = sizebot; i < sizetop + sizebot; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, boxPositions[i]);
		if (camera.Reverse)
		model = glm::translate(model, glm::vec3(0.0f, -11.0f, 0.0f));
		ourShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);

	ourShader.use();

	for (unsigned int i = sizetop + sizebot; i < sizetop + sizebot + sizecheckbot + sizechecktop; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, boxPositions[i]);
		ourShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
    
	


	if (camera.Position.z <= -100.0f) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);

		ourShader.use();

		float timeValue = glfwGetTime();
		float showValue = sin(timeValue);

		if (showValue>=0.9f && showValue<=1.0f || showValue>=-0.4f && showValue<=0.4f || showValue >= -1.0f && showValue <= -0.9f)
		{
			for (unsigned int i = 0; i < 107; i++)
			{
				// calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 model = glm::mat4(1.0f);
				
				model = glm::translate(model, cubePositions[i]);
				model = glm::scale(model, glm::vec3(0.2f));
				ourShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		glBindVertexArray(0);
	}
	
	

	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	cubemapShader.use();
	view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	cubemapShader.setMat4("view", view);
	cubemapShader.setMat4("projection", projection);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else
	{
		if (key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_A || key == GLFW_KEY_D || key == GLFW_KEY_SPACE) {
			camera.ProcessKeyboard(key, action);
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposd, double yposd) {
  float xpos = static_cast<float>(xposd);
  float ypos = static_cast<float>(yposd);
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(std::vector<std::string> faces){
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
