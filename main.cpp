#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

const GLuint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;
const GLfloat CAMERA_MOVEMENT_STEP = 0.08;

GLfloat y_rotation;
GLfloat x_rotation;
glm::vec3 camera_position;
glm::vec3 scale;

struct IndividualTransformations {
    GLfloat rot_x;
    GLfloat rot_y;
    GLfloat rot_z;
    glm::vec3 translation;
    glm::vec3 scale;
};

glm::mat4 netTransformation(IndividualTransformations transformations)
{
    //start with an identity matrix
    glm::mat4 result = glm::mat4(1.0f);

    //apply the scale first
    result = glm::scale(result, transformations.scale);

    //then the rotation
    result = glm::rotate(result, transformations.rot_x, glm::vec3(1.0f, 0.0f, 0.0f));
    result = glm::rotate(result, transformations.rot_y, glm::vec3(0.0f, 1.0f, 0.0f));
    result = glm::rotate(result, transformations.rot_z, glm::vec3(0.0f, 0.0f, 1.0f));

    //then finally the translation
    result = glm::translate(result, transformations.translation);

    return result;
}

/*glm::mat4 netTransformation(IndividualTransformations transformations) {

//  glm::mediump_float x_rot = transformations.rotation.x;
//  glm::mediump_float y_rot = transformations.rotation.y;
//  glm::mediump_float z_rot = transformations.rotation.z;

  //glm::mat4 complete_rotation(1.0f);
  glm::mat4 rx = glm::rotate(glm::mat4(1.0f), transformations.rot_x, glm::vec3(1.0f, 0.0f, 0.0f));	//rotate around x axis
  glm::mat4 ry = glm::rotate(glm::mat4(1.0f), transformations.rot_y, glm::vec3(0.0f, 1.0f, 0.0f));	//rotate around y axis
  glm::mat4 rz = glm::rotate(glm::mat4(1.0f), transformations.rot_z, glm::vec3(0.0f, 1.0f, 0.0f));	//rotate around z axis

  glm::mat4 complete_rotation = rz * ry * rx;

  return glm::translate(glm::mat4(1.0f),transformations.translation) * complete_rotation * glm::scale(glm::mat4(1.0f), transformations.scale);//glm::rotate(glm::mat4(1.0f), transformations.rotation)
  //return individualTransformations.rotation * individualTransformations.translation * individualTransformations.scale;
}*/

void DrawCube(GLuint transformLoc, IndividualTransformations individualTransformations, GLuint cubeVAO) {

    glm::mat4 model_matrix = /*startingPosition * */ netTransformation(individualTransformations); // It is assumed that the startingPosition is the origin
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 12*3);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {

        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {

        camera_position.z += CAMERA_MOVEMENT_STEP;
    }
    else if (key == GLFW_KEY_E && action == GLFW_PRESS) {

        camera_position.z -= CAMERA_MOVEMENT_STEP;
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS) {

        camera_position.x -= CAMERA_MOVEMENT_STEP;
    }
    else if(key == GLFW_KEY_A && action == GLFW_PRESS) {

        camera_position.x += CAMERA_MOVEMENT_STEP;
    }
    else if(key == GLFW_KEY_W && action == GLFW_PRESS) {

        camera_position.y -= CAMERA_MOVEMENT_STEP;
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS) {

        camera_position.y += CAMERA_MOVEMENT_STEP;
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {

        y_rotation += 10.0f;
    }
    else if (key == GLFW_KEY_T && action == GLFW_PRESS) {

        y_rotation -= 10.0f;
    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS) {

        x_rotation += 10.0f;
    }
    else if (key == GLFW_KEY_G && action == GLFW_PRESS) {

        x_rotation -= 10.0f;
    }
    else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (key == GLFW_KEY_X && action == GLFW_PRESS) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Horse Program", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }


    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );

    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;

    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);


    // Build and compile our shader program

    // Vertex shader

    // Read the Vertex Shader code from the file
    string vertex_shader_path = "vertex_shader.glsl";
    string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

    if (VertexShaderStream.is_open()) {

        string Line = "";

        while (getline(VertexShaderStream, Line)) {
            VertexShaderCode += "\n" + Line;
        }

        VertexShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
        getchar();
        exit(-1);
    }

    // Read the Fragment Shader code from the file
    string fragment_shader_path = "fragment_shader.glsl";
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

    if (FragmentShaderStream.is_open()) {
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
        FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
        getchar();
        exit(-1);
    }
    //Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader); //free up memory
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    //----------------------------------------------------------------------------------------------------------


    vector <glm::vec3> gridVert;


    //Grid
    // Inner part of grid
    for(float i = -50; i < 50; i+=1) {

        gridVert.push_back( glm::vec3(i,-0.5,+50) );
        gridVert.push_back( glm::vec3(i,-0.5,-50) );

        gridVert.push_back( glm::vec3(-50,-0.5,i) );
        gridVert.push_back( glm::vec3(+50,-0.5,i) );
    }

    // Outer boundary of grid
    gridVert.push_back( glm::vec3(-50,-0.5,+50) );
    gridVert.push_back( glm::vec3(+50,-0.5,+50) );

    gridVert.push_back( glm::vec3(-50,-0.5,-50) );
    gridVert.push_back( glm::vec3(+50,-0.5,-50) );

    gridVert.push_back( glm::vec3(-50,-0.5,-50) );
    gridVert.push_back( glm::vec3(-50,-0.5,+50) );

    gridVert.push_back( glm::vec3(+50,-0.5,-50) );
    gridVert.push_back( glm::vec3(+50,-0.5,+50) );

    //Cube
    vector<glm::vec3> vertCube =
    {
        // Front face triangle 1
        glm::vec3(0.5,-0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,-0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal

        // Front face triangle 2
        glm::vec3(0.5,-0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal

        // Back face triangle 1
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,-0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal

        // Back face triangle 2
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(0.5,0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal

        // Left face triangle 1
        glm::vec3(-0.5,-0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,-0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal

        // Left face triangle 2
        glm::vec3(-0.5,-0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal

        // Right face triangle 1
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(0.5,-0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal

        // Right face triangle 2
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(0.5,0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal

        // Top face triangle 1
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal

        // Top face triangle 2
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(0.5,0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal

        // Bottom face triangle 1
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,-0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,-0.5,0.5),

        // Bottom face triangle 2
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5), // Teal
        glm::vec3(-0.5,-0.5,-0.5),
        glm::vec3(0.05, 0.74, 0.5) // Teal
    };

    //Coordinate Axes

    vector <glm::vec3> axeVert;
    //x-axis
    axeVert.push_back(glm::vec3(0,-0.5,0));
    axeVert.push_back(glm::vec3(1.0,0.0,0.0)); //Red
    axeVert.push_back(glm::vec3(5,-0.5,0));
    axeVert.push_back(glm::vec3(1.0, 0.0, 0.0)); //Red
    //y-axis
    axeVert.push_back(glm::vec3(0,-0.5,0));
    axeVert.push_back(glm::vec3(0.0, 1.0, 0.0)); //Green
    axeVert.push_back(glm::vec3(0,4.5,0));
    axeVert.push_back(glm::vec3(0.0, 1.0, 0.0)); //Green
    //z-axis
    axeVert.push_back(glm::vec3(0,-0.5,0));
    axeVert.push_back(glm::vec3(0.0, 0.0, 1.0)); //Blue
    axeVert.push_back(glm::vec3(0, -0.5, 5));
    axeVert.push_back(glm::vec3(0.0, 0.0, 1.0)); //Blue






    //------ MODEL MATRIX ---------
    //glm::mat4 mm;
    //glm::mat4 scale;
    //glm::mat4 translate;
    //glm::mat4 rotate;

    //------ VIEW MATRIX ---------

    //view_matrix = glm::lookAt(c_pos, c_pos + c_dir, c_up);

    //------ PROJECTION MATRIX -------
    //glm::mat4 pm = glm::perspective(45.f, 800.f / 600.f, 0.1f, 100.f);

    //GLuint mm_addr = glGetUniformLocation(shaderProgram, "model_matrix");
    //GLuint vm_addr = glGetUniformLocation(shaderProgram, "view_matrix");
    //GLuint pm_addr = glGetUniformLocation(shaderProgram, "projection_matrix");
    //glUniformMatrix4fv(mm_addr, 1, false, glm::value_ptr(mm));
    //glUniformMatrix4fv(pm_addr, 1, false, glm::value_ptr(pm));



    GLuint cubeVAO;
    GLuint gridVAO;
    GLuint cubeVBO;
    GLuint gridVBO;
    GLuint axeVAO;
    GLuint axeVBO;
    GLuint colorVBO;


    //Cube
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);


    glBufferData(GL_ARRAY_BUFFER, vertCube.size() * sizeof(glm::vec3), &vertCube.front(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertCube), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertCube), (GLvoid*)(3*sizeof(GLfloat)));


    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(glm::uvec3), &ind.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

    //Grid
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);

    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, gridVert.size() * sizeof(glm::vec3), &gridVert.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);


    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

    //Axes
    glGenVertexArrays(1, &axeVAO);
    glGenBuffers(1, &axeVBO);

    glBindVertexArray(axeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axeVBO);


    glBufferData(GL_ARRAY_BUFFER, axeVert.size() * sizeof(glm::vec3), &axeVert.front(), GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(axeColor) * sizeof(GLfloat), &axeColor[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(axeVert), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(axeVert), (void*)12);


    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

    //Color
    /*glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axeColor), &axeColor[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 0, (void*)0); */

    scale = glm::vec3(1.0f);

    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
    GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view_matrix");
    GLuint transformLoc = glGetUniformLocation(shaderProgram, "model_matrix");
    GLint colorLoc = glGetUniformLocation(shaderProgram, "color");

    //HORSE


    //Head

    //Neck

    //Torso
  	IndividualTransformations torsoTransform;
  	torsoTransform.scale = glm::vec3(2.0f, 1.0f, 1.0f);
  	torsoTransform.translation = glm::vec3(1);
  	torsoTransform.rot_x = 90.0f;
  	DrawCube(/*glm::mat4(1),*/ transformLoc, torsoTransform, cubeVAO);
  	//translat = glm::translate(model_matrix, glm::vec3(2.5,0,2.5));

    //Right Front Lower Arm

    //Right Front Upper Arm

    //Left Front Lower Arm

    //Left Front Upper Arm

    //Right Back Lower Leg

    //Right Back Upper Leg

    //Left Back Lower Leg

    //Left Back Upper Leg


    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    glm::mat4 scal;
    glm::mat4 translat;
    glm::mat4 rotate;


    // Game loop


    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer

        glClearColor(0.3f, 0.2f, 0.1f, 1.0f); // Light brown
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);



        glm::mat4 view_matrix;
        view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -3.0f));
        view_matrix = translate(view_matrix, camera_position);
        //view_matrix = glm::lookAt(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 model_matrix;
        model_matrix = glm::scale(model_matrix, glm::vec3(1.0f));
        model_matrix = glm::rotate(model_matrix, glm::radians(y_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, glm::radians(x_rotation), glm::vec3(1.0f, 0.0f, 0.0f));
        //model_matrix = glm::rotate(model_matrix, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 200.0f);

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
        glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));


        //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

        glBindVertexArray(cubeVAO);

        glDrawArrays(GL_TRIANGLES, 0, 12*3);

        glBindVertexArray(gridVAO);

        glDrawArrays(GL_LINES, 0, 2*2*100+2*4);

        glBindVertexArray(axeVAO);

        glDrawArrays(GL_LINE_STRIP, 0, 6);

        //Cube Transformations

        //Lower Parts

        //Front Left Lower Leg
        translat = glm::translate(model_matrix, glm::vec3(2.5,0,2.5));

        scal = glm::scale(model_matrix, glm::vec3(0.25, 1, 0.25));

        model_matrix = translat * scal;

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));

        glBindVertexArray(cubeVAO);

        glDrawArrays(GL_TRIANGLES, 0, 12*3);


        //Front Right Lower Leg
        /*translat = glm::translate(model_matrix, glm::vec3(4,0,2));

        scal = glm::scale(model_matrix, glm::vec3(0.25, 1, 0.25));

        model_matrix = translat * scal;


        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));

        glBindVertexArray(cubeVAO);

        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        glBindVertexArray(0);

        //Back Left Lower Leg

        translat = glm::translate(model_matrix, glm::vec3(2,0,4));

        scal = glm::scale(model_matrix, glm::vec3(0.25, 1, 0.25));

        model_matrix = translat * scal;


        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));

        glBindVertexArray(cubeVAO);

        glDrawArrays(GL_TRIANGLES, 0, 12*3);

        //Back Right Lower Leg

        translat = glm::translate(model_matrix, glm::vec3(4,0,4));

        scal = glm::scale(model_matrix, glm::vec3(0.25, 1, 0.25));

        model_matrix = translat * scal;


        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));

        glBindVertexArray(cubeVAO);

        glDrawArrays(GL_TRIANGLES, 0, 12*3); */


        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

/*enum class TransformationMatrix {

  MODEL_MATRIX,
  VIEW_MATRIX,
  PROJECTION_MATRIX
}

DrawCube(glm::mat4 startingPosition, GLuint modelViewOrProjectionLoc, IndividualTransformations individualTransformations, GLuint cubeVAO, TransformationMatrix choice) {

  switch(choice) {
    case TransformationMatrix::MODEL_MATRIX: {

    }
  }
}*/
