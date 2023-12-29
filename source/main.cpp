#define GLFW_INCLUDE_NONE
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>




#include "renderer/texture2d.h"
#include "renderer/shader.h"
#include "renderer/material.h"

#include "component/game_object.h"
#include "component/transform.h"
#include "renderer/meshfilter.h"
#include "renderer/mesh_renderer.h"
using namespace std;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
GLFWwindow* window;
GLint mvp_location, vpos_location, vcol_location, u_diffuse_texture_location, a_uv_location;
GLuint vao, vbo, ebo;
Texture2D* texture2d=nullptr;
Shader* shader;
MeshFilter* meshFilter=nullptr;
Material* material = nullptr;
void init_opengl()
{
    cout << "init opengl" << endl;

    //设置错误回调
    glfwSetErrorCallback(error_callback);

    //初始化glfw
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //创建窗口
    window = glfwCreateWindow(960, 640, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    //设置当前上下文
    glfwMakeContextCurrent(window);
    //初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
    //双缓冲区间隔
    glfwSwapInterval(1);


}



//创建Texture
void CreateTexture(std::string image_file_path)
{
    texture2d = Texture2D::LoadFromFile(image_file_path);
}

int main()
{

    //初始化opengl
    init_opengl();

    GameObject* go = new GameObject("something");
    Transform* transform= dynamic_cast<Transform*>(go->AddComponent("Transform"));
    MeshFilter* mesh_filter= dynamic_cast<MeshFilter*>(go->AddComponent("MeshFilter"));
    meshFilter->LoadMesh("model/fishsoup_pot.mesh");

    MeshRenderer* mesh_renderer = dynamic_cast<MeshRenderer*>(go->AddComponent("MeshRenderer"));
    Material* material = new Material();
    material->Parse("material/fishsoup_pot.mat");
    mesh_renderer->SetMaterial(material);

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
       

        //获取画面宽高
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(49.f / 255, 77.f / 255, 121.f / 255, 1.f);
       
        glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 projection = glm::perspective(glm::radians(60.f), ratio, 1.f, 1000.f);
        mesh_renderer->SetView(view);
        mesh_renderer->SetProjection(projection);
        mesh_renderer->Render();
        glfwSwapBuffers(window);
        glfwPollEvents();


    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

