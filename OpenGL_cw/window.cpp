#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include "water.h"
#include "lamp.h"
#include "model.h"
#include "grass.h"
#include "skybox.h"
#include "waterFrameBuffer.h"
#include "stb_image.h"

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 1.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

// mouse
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// light 
glm::vec3 lightPos(1.2f, 2.3f, 2.0f);
glm::vec3 lightColor(1.0, 1.0, 0.75);

// model file path
char backpackPath[] = "LibsOpenGL/backpack/backpack.obj";
char treePath[] = "LibsOpenGL/tree/tree.obj";
char seagullPath[] = "LibsOpenGL/seagull/seagull.obj";
char sneakersPath[] = "LibsOpenGL/sneakers/shoes.obj";
char flowersPath[] = "LibsOpenGL/flowers/flowers.obj";
char cliffsPath[] = "LibsOpenGL/cliffs/cliffs.obj";

float waterHeight = -0.5;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

/*void loadTexture(unsigned int& texture, char path[]) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}*/

// function to draw flowers
void drawFlowers(Shader& flowersShader, Model& flowersModel) {
    flowersShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, -0.25f, -2.7f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    flowersShader.setMat4("model", model);
    flowersShader.setMat4("projection", projection);
    flowersShader.setMat4("view", view);
    flowersShader.setVec3("lightColor", lightColor);
    flowersShader.setVec3("viewPos", camera.Position);
    flowersShader.setVec3("lightPos", lightPos);
    flowersModel.Draw(flowersShader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, -0.25f, -2.8f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
    flowersShader.setMat4("model", model);
    flowersModel.Draw(flowersShader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.8f, -0.35f, -3.25f));
    model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    flowersShader.setMat4("model", model);
    flowersModel.Draw(flowersShader);
}

// function to draw sneakers
void drawSneakers(Shader& sneakersShader, Model& sneakersModel) {
    sneakersShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(65.0f, 65.0f, 65.0f));
    model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.2f, -0.45f, -2.6f));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    sneakersShader.setMat4("model", model);
    sneakersShader.setMat4("projection", projection);
    sneakersShader.setMat4("view", view);
    sneakersShader.setVec3("lightColor", lightColor);
    sneakersShader.setVec3("viewPos", camera.Position);
    sneakersShader.setVec3("lightPos", lightPos);
    sneakersModel.Draw(sneakersShader);
}

// function to draw seagulls
void drawSeagull(Shader& seagullShader, Model& seagullModel) {
    seagullShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.7f, 1.2f, -1.0f));
    model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    seagullShader.setMat4("model", model);
    seagullShader.setMat4("projection", projection);
    seagullShader.setMat4("view", view);
    seagullShader.setVec3("lightColor", lightColor);
    seagullShader.setVec3("viewPos", camera.Position);
    seagullShader.setVec3("lightPos", lightPos);
    seagullModel.Draw(seagullShader);

    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f, 1.5f, -0.5f));
    model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
    seagullShader.setMat4("model", model);
    seagullModel.Draw(seagullShader);

    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(-0.7f, 1.2f, -0.75f));
    model = glm::scale(model, glm::vec3(3.2f, 3.2f, 3.2f));
    seagullShader.setMat4("model", model);
    seagullModel.Draw(seagullShader);
}

// function to draw tree
void drawTree(Shader& treeShader, Model& treeModel) {
    treeShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.3f, -0.49f, -3.5f)); 
    model = glm::scale(model, glm::vec3(0.45f, 0.45f, 0.45f));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    treeShader.setMat4("model", model);
    treeShader.setMat4("projection", projection);
    treeShader.setMat4("view", view);
    treeShader.setVec3("lightColor", lightColor);
    treeShader.setVec3("viewPos", camera.Position);
    treeShader.setVec3("lightPos", lightPos);
    treeModel.Draw(treeShader);
}

// function to draw backpack
void drawBackpack(Shader& backpackShader, Model& backpackModel) {
    backpackShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.45f, 0.06f, -2.8f));
    model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    backpackShader.setMat4("model", model);
    backpackShader.setMat4("projection", projection);
    backpackShader.setMat4("view", view);
    backpackShader.setVec3("lightColor", lightColor);
    backpackShader.setVec3("viewPos", camera.Position);
    backpackShader.setVec3("lightPos", lightPos);
    backpackModel.Draw(backpackShader);
}

// function to draw rocks
void drawCliffs(Shader& cliffsShader, Model& cliffsModel) {
    cliffsShader.use();
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(-3.5f, -0.5f, -3.0f));
    model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
    model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    cliffsShader.setMat4("model", model);
    cliffsShader.setMat4("projection", projection);
    cliffsShader.setMat4("view", view);
    cliffsShader.setVec3("lightColor", lightColor);
    cliffsShader.setVec3("viewPos", camera.Position);
    cliffsShader.setVec3("lightPos", lightPos);
    cliffsModel.Draw(cliffsShader);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // creating window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL reflection", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // telling GLFW to capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell program to flip texture
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    Water water = Water();
    Lamp lamp = Lamp();
    Grass grass = Grass();
    Skybox skybox = Skybox();
    WaterFrameBuffer waterFBs = WaterFrameBuffer();
    
    //stbi_set_flip_vertically_on_load(true);
    Shader backpackShader = Shader("modelLoadVS.txt", "modelLoadFS.txt");
    Model backpackModel(backpackPath);

    // tell program not to flip texture
    stbi_set_flip_vertically_on_load(false);
    Shader treeShader = Shader("modelLoadVS.txt", "modelLoadFS.txt");
    Model treeModel(treePath);

    Shader seagullShader = Shader("modelLoadVS.txt", "modelLoadFS.txt");
    Model seagullModel(seagullPath);

    Shader sneakersShader = Shader("modelLoadVS.txt", "modelLoadFS.txt");
    Model sneakersModel(sneakersPath);

    Shader flowersShader = Shader("modelLoadVS.txt", "modelLoadFS.txt");
    Model flowersModel(flowersPath);

    Shader cliffsShader = Shader("modelLoadVS.txt", "modelLoadFS.txt");
    Model cliffsModel(cliffsPath);

    // bind water vertices array
    unsigned int VBO, waterVAO, EBO;
    glGenVertexArrays(1, &waterVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(waterVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(water.vertices), water.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // bind skybox vertices array
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox.vertices), &skybox.vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // bind grass vertices array
    unsigned int grassVAO;
    glGenVertexArrays(1, &grassVAO);
    glBindVertexArray(grassVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grass.vertices), grass.vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // bind light cube array
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lamp.vertices), lamp.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);

    
    glBindVertexArray(0);

    // load skybox texture
    unsigned int skyboxTexture = skybox.loadCubemap();
    skybox.shader.use();
    skybox.shader.setInt("skybox", 0);

    water.useShader();
    water.shader.setInt("reflection", 0);

    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_CLIP_DISTANCE0);
        glEnable(GL_DEPTH_TEST);

        // switching to the custom framebuffer
        waterFBs.bindReflectionFrameBuffer();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // changing camera position
        float distance = 2 * (camera.Position.y - waterHeight);
        camera.Position.y -= distance;
        camera.invertPitch();

        // set transformation matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

        // draw scene
        drawBackpack(backpackShader, backpackModel);
        drawSneakers(sneakersShader, sneakersModel);
        drawSeagull(seagullShader, seagullModel);
        drawTree(treeShader, treeModel);
        drawFlowers(flowersShader, flowersModel);
        drawCliffs(cliffsShader, cliffsModel);

        grass.useShader();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(8.0, 4.5, 1.0));
        grass.shader.setVec3("lightColor", lightColor);
        grass.shader.setMat4("model", model);
        grass.shader.setMat4("view", view);
        grass.shader.setMat4("projection", projection);
        grass.shader.setVec3("viewPos", camera.Position);
        grass.shader.setVec3("lightPos", lightPos);
        glBindVertexArray(grassVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glDepthFunc(GL_LEQUAL); 
        skybox.shader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skybox.shader.setMat4("view", view);
        skybox.shader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        // change camera position back
        camera.Position.y += distance;
        camera.invertPitch();

        glDisable(GL_CLIP_DISTANCE0);

        // switching to default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // change view matrix back
        view = camera.GetViewMatrix();

        // draw water
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, waterHeight, 0.0f));
        model = glm::scale(model, glm::vec3(8.0, 4.5, 4.5));
        view = camera.GetViewMatrix();
        water.useShader();
        water.shader.setVec3("lightColor", lightColor);
        water.shader.setVec3("objectColor", 0.0f, 0.0f, 1.0f);
        water.shader.setMat4("model", model);
        water.shader.setMat4("view", view);
        water.shader.setMat4("projection", projection);
        water.shader.setVec3("viewPos", camera.Position);
        water.shader.setVec3("lightPos", lightPos);
        glBindVertexArray(waterVAO);
        glActiveTexture(GL_TEXTURE0);
        // bind reflection texture
        glBindTexture(GL_TEXTURE_2D, waterFBs.getReflectionTexture());
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // draw grass
        grass.useShader();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(8.0, 4.5, 1.0));
        grass.shader.setVec3("lightColor", lightColor);
        grass.shader.setMat4("model", model);
        grass.shader.setMat4("view", view);
        grass.shader.setMat4("projection", projection);
        grass.shader.setVec3("viewPos", camera.Position);
        grass.shader.setVec3("lightPos", lightPos);
        glBindVertexArray(grassVAO); 
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // draw light cube
        lamp.shader.use();
        lamp.shader.setMat4("projection", projection);
        lamp.shader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lamp.shader.setMat4("model", model);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // draw models
        drawBackpack(backpackShader, backpackModel);
        drawTree(treeShader, treeModel);
        drawSeagull(seagullShader, seagullModel);
        drawSneakers(sneakersShader, sneakersModel);
        drawFlowers(flowersShader, flowersModel);
        drawCliffs(cliffsShader, cliffsModel);

        // draw skybox
        glDepthFunc(GL_LEQUAL);  
        skybox.shader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skybox.shader.setMat4("view", view);
        skybox.shader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &waterVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();

    return 0;
}
