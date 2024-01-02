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
#include "renderer/camera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
GLFWwindow* window;

void init_opengl()
{
    cout << "init opengl" << endl;

    //���ô���ص�
    glfwSetErrorCallback(error_callback);

    //��ʼ��glfw
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //��������
    window = glfwCreateWindow(960, 640, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    //���õ�ǰ������
    glfwMakeContextCurrent(window);
    //��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
    //˫���������
    glfwSwapInterval(1);


}




int main()
{

    //��ʼ��opengl
    init_opengl();

    GameObject* go = new GameObject("something");
    go->set_layer(0x01);
    Transform* transform= dynamic_cast<Transform*>(go->AddComponent("Transform"));
    MeshFilter* mesh_filter= dynamic_cast<MeshFilter*>(go->AddComponent("MeshFilter"));
    mesh_filter->LoadMesh("model/fishsoup_pot.mesh");

    MeshRenderer* mesh_renderer = dynamic_cast<MeshRenderer*>(go->AddComponent("MeshRenderer"));
    Material* material = new Material();
    material->Parse("material/fishsoup_pot.mat");
    mesh_renderer->SetMaterial(material);


    //������� GameObject
    GameObject* go_camera = new GameObject("main_camera");
    //���� Transform ���
    Transform* transform_camera = dynamic_cast<Transform*>(go_camera->AddComponent("Transform"));
    transform_camera->set_position(glm::vec3(0, 0, 10));
    //���� Camera ���
    Camera* camera = dynamic_cast<Camera*>(go_camera->AddComponent("Camera"));
    camera->set_depth(0);
    //�������2 GameObject
    auto go_camera_2 = new GameObject("main_camera");
    //���� Transform ���
    auto transform_camera_2 = dynamic_cast<Transform*>(go_camera_2->AddComponent("Transform"));
    transform_camera_2->set_position(glm::vec3(1, 0, 10));
    //���� Camera ���
    auto camera_2 = dynamic_cast<Camera*>(go_camera_2->AddComponent("Camera"));
    //�ڶ�������������֮ǰ����ɫ����Ȼ�õ�һ�����������Ⱦ������ͱ���� û�ˡ�
    camera_2->set_clear_flag(GL_DEPTH_BUFFER_BIT);
    camera_2->set_depth(1);
    camera->set_culling_mask(0x02);
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
       

        //��ȡ������
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);


        //�������
        camera->SetView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        camera->SetProjection(60.f, ratio, 1.f, 1000.f);
        camera_2->SetView(glm::vec3(transform_camera_2->position().x, 0, 0), glm::vec3(0, 1, 0));
        camera_2->SetProjection(60.f, ratio, 1.f, 1000.f);

        //��ת����
        static float rotate_eulerAngle = 0.f;
        rotate_eulerAngle += 0.1f;
        glm::vec3 rotation = transform->rotation();
        rotation.y = rotate_eulerAngle;
        transform->set_rotation(rotation);

        Camera::Foreach([&]() {
            mesh_renderer->Render();
            });
        glfwSwapBuffers(window);
        glfwPollEvents();


    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

