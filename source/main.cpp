#define GLFW_INCLUDE_NONE
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext.hpp>

#include "renderer/texture2d.h"
#include "renderer/shader.h"
#include "renderer/material.h"
#include "component/game_object.h"
#include "component/transform.h"
#include "renderer/meshfilter.h"
#include "renderer/mesh_renderer.h"
#include "renderer/camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "control/input.h"
#include "control/key_code.h"
#include "utils/screen.h"
using namespace std;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
//���̻ص�
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Input::RecordKey(key, action);
}
//��갴���ص�
 void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Input::RecordKey(button, action);
    std::cout << "mouse_button_callback:" << button << "," << action << std::endl;
}

//����ƶ��ص�
 void mouse_move_callback(GLFWwindow* window, double x, double y)
{
    Input::set_mousePosition(x, y);
  
}


//�����ֻص�
 void mouse_scroll_callback(GLFWwindow* window, double x, double y)
{
    Input::RecordScroll(y);
  
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
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
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
    camera_2->set_culling_mask(0x02);

    //��һ֡�����λ��
    vec2_ushort last_frame_mouse_position = Input::mousePosition();

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
       

        //��ȡ������
        glfwGetFramebufferSize(window, &width, &height);
        Screen::set_width_height(width, height);
        glViewport(0, 0, width, height);
       

        //�������
        camera->SetView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        camera->SetProjection(60.f, Screen::aspect_ratio(), 1.f, 1000.f);
        camera_2->SetView(glm::vec3(transform_camera_2->position().x, 0, 0), glm::vec3(0, 1, 0));
        camera_2->SetProjection(60.f, Screen::aspect_ratio(), 1.f, 1000.f);

        //��ת����
        if (Input::GetKeyDown(KEY_CODE_R))
        {
            static float rotate_eulerAngle = 0.f;
            rotate_eulerAngle += 0.1f;
            glm::vec3 rotation = transform->rotation();
            rotation.y = rotate_eulerAngle;
            transform->set_rotation(rotation);
        }
        //��ת���
        if (Input::GetKeyDown(KEY_CODE_Z) && Input::GetMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            float degrees = Input::mousePosition().x_ - last_frame_mouse_position.x_;

            glm::mat4 old_mat4 = glm::mat4(1.0f);
            std::cout << glm::to_string(old_mat4) << std::endl;

            glm::mat4 rotate_mat4 = glm::rotate(old_mat4, glm::radians(degrees), glm::vec3(0.0f, 1.0f, 0.0f));//�������������ϵλ�ã�����������ת�ľ�����������㣬����ֱ���÷���
            glm::vec4 old_pos = glm::vec4(transform_camera->position(), 1.0f);
            glm::vec4 new_pos = rotate_mat4 * old_pos;//��ת���� * ԭ�������� = ������������ת��
            std::cout << glm::to_string(new_pos) << std::endl;
          
            transform_camera->set_position(glm::vec3(new_pos));
        }
        last_frame_mouse_position = Input::mousePosition();

        //�����ֿ������Զ��
        transform_camera->set_position(transform_camera->position() * (10 - Input::mouse_scroll()) / 10.f);
        Input::Update();

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

