#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <iostream>


#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform2.hpp>
#include<glm/gtx/euler_angles.hpp>

#include <stdlib.h>
#include <stdio.h>
#include "ShaderSource.h"
#include "VertexData.h"
#include "meshfilter.h"
#include "texture2d.h"
#include "shader.h"
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
Mesh* mesh;
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


void create_buffer() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    //将缓冲区对象指定为顶点缓冲区对象
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //上传顶点数据到缓冲区对象
    glBufferData(GL_ARRAY_BUFFER,mesh->vertex_num_ * sizeof(Vertex), mesh->vertex_data_, GL_STATIC_DRAW);


    //将缓冲区对象指定为顶点索引缓冲区对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //上传顶点索引数据到缓冲区对象
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->vertex_index_num_ * sizeof(unsigned short), mesh->vertex_index_data_, GL_STATIC_DRAW);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(vpos_location);//启用顶点Shader属性(a_pos)，指定与顶点坐标数据进行关联

    glVertexAttribPointer(vcol_location, 4, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(vcol_location);//启用顶点Shader属性(a_color)，指定与顶点颜色数据进行关联

    glVertexAttribPointer(a_uv_location, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(float) * (3 + 4)));
    glEnableVertexAttribArray(a_uv_location);//启用顶点Shader属性(a_color)，指定与顶点颜色数据进行关联

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
//创建Texture
void CreateTexture(std::string image_file_path)
{
    texture2d = Texture2D::LoadFromFile(image_file_path);
}

int main()
{
    VertexRemoveDumplicate();
    MeshFilter meshFilter;
   //meshFilter.ExportMesh("data/model/cube.mesh",kVertexRemoveDumplicateVector, kVertexIndexVector);
   meshFilter.LoadMesh("data/model/cube.mesh");
   mesh = meshFilter.mesh();
    //初始化opengl
    init_opengl();
    shader = Shader::Find("data/shader/unlit");
    CreateTexture("data/urban.cpt");
    //获取shader属性ID
    mvp_location = glGetUniformLocation(shader->gl_program_id(), "u_mvp");
    vpos_location = glGetAttribLocation(shader->gl_program_id(), "a_pos");
    vcol_location = glGetAttribLocation(shader->gl_program_id(), "a_color");
    a_uv_location = glGetAttribLocation(shader->gl_program_id(), "a_uv");
    u_diffuse_texture_location = glGetUniformLocation(shader->gl_program_id(), "u_diffuse_texture");

    create_buffer();
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
        //坐标转换
        glm::mat4 model, view, projection, mvp;
        glm::mat4 trans = glm::translate(glm::vec3(0, 0, 0));
        static float rotate_eulerAngle = 0.f;
        rotate_eulerAngle += 1;
        glm::mat4 rotation = glm::eulerAngleYXZ(glm::radians(rotate_eulerAngle), glm::radians(rotate_eulerAngle), glm::radians(rotate_eulerAngle));
        glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f)); //缩放;
        model = trans * scale * rotation;

        view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        projection = glm::perspective(glm::radians(60.f), ratio, 1.f, 1000.f);

        mvp = projection * view * model;
       

        //指定GPU程序(就是指定顶点着色器、片段着色器)
        glUseProgram(shader->gl_program_id());
        {
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);//开启背面剔除

            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
            //贴图设置
             //激活纹理单元0
            glActiveTexture(GL_TEXTURE0);
            //将加载的图片纹理句柄，绑定到纹理单元0的Texture2D上。
            glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id_);
            //设置Shader程序从纹理单元0读取颜色数据
            glUniform1i(u_diffuse_texture_location, 0);

            glBindVertexArray(vao);

            //上传顶点数据并进行绘制
            glDrawElements(GL_TRIANGLES,36, GL_UNSIGNED_SHORT, 0);//使用顶点索引进行绘制，最后的0表示数据偏移量。
            glBindVertexArray(0);
            }


        glfwSwapBuffers(window);
        glfwPollEvents();


    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}


