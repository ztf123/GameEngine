#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <iostream>


#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform2.hpp>
#include<glm/gtx/euler_angles.hpp>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <stdlib.h>
#include <stdio.h>
#include "ShaderSource.h"
#include "VertexData.h"

#include "texture2d.h"
using namespace std;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
GLFWwindow* window;
GLuint vertex_shader, fragment_shader, program;
GLint mvp_location, vpos_location, vcol_location, u_diffuse_texture_location, a_uv_location;
GLuint vao, vbo_pos, vbo_color, vbo_uv;
Texture2D* texture2d=nullptr;
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

void compile_shader()
{
    //��������Shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    //ָ��ShaderԴ��
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    //����Shader
    glCompileShader(vertex_shader);
    //��ȡ������
    GLint compile_status = GL_FALSE;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE)
    {
        GLchar message[256];
        glGetShaderInfoLog(vertex_shader, sizeof(message), 0, message);
        cout << "compile vs error:" << message << endl;
    }

    //����Ƭ��Shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    //ָ��ShaderԴ��
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    //����Shader
    glCompileShader(fragment_shader);
    //��ȡ������
    compile_status = GL_FALSE;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE)
    {
        GLchar message[256];
        glGetShaderInfoLog(fragment_shader, sizeof(message), 0, message);
        cout << "compile fs error:" << message << endl;
    }


    //����GPU����
    program = glCreateProgram();
    //����Shader
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    //Link
    glLinkProgram(program);
    //��ȡ������
    GLint link_status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE)
    {
        GLchar message[256];
        glGetProgramInfoLog(program, sizeof(message), 0, message);
        cout << "link error:" << message << endl;
    }
}
void create_buffer() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo_pos);
    glGenBuffers(1, &vbo_color);
    glGenBuffers(1, &vbo_uv);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kPositions), kPositions, GL_STATIC_DRAW);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, false, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(vpos_location);//���ö���Shader����(a_pos)��ָ���붥���������ݽ��й���

    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kColors), kColors, GL_STATIC_DRAW);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, false, sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(vcol_location);//���ö���Shader����(a_color)��ָ���붥����ɫ���ݽ��й���

    glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kUvs), kUvs, GL_STATIC_DRAW);
    glVertexAttribPointer(a_uv_location, 2, GL_FLOAT, false, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(a_uv_location);//���ö���Shader����(a_color)��ָ���붥����ɫ���ݽ��й���

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}
//����Texture
void CreateTexture(std::string image_file_path)
{
    texture2d = Texture2D::LoadFromFile(image_file_path);
}
void InitSpdLog()
{
    try
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::trace);
        console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt", true);
        file_sink->set_level(spdlog::level::trace);

        spdlog::sinks_init_list sink_list = { file_sink, console_sink };

        // you can even set multi_sink logger as default logger
        spdlog::set_default_logger(std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list({ console_sink, file_sink })));
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
int main(int argc,char** argv)
{
    InitSpdLog();

    init_opengl();

    for (int i = 1; i < argc; ++i) {
        //��GPU�У����Դ��б����ѹ���õ��������ݣ����ص��ڴ棬�����浽Ӳ�̡�
//    std::string src_image_file_path("../data/images/urban.jpg");
        std::string src_image_file_path(argv[i]);
        //    src_image_file_path= ReplaceAll(src_image_file_path,"\\","/");
        SPDLOG_INFO("src_image_file_path:{}", src_image_file_path);
        std::string cpt_file_path = src_image_file_path;
        auto last_index_of_point = cpt_file_path.find_last_of('.');
        cpt_file_path.replace(last_index_of_point, cpt_file_path.size() - last_index_of_point, ".cpt");
        Texture2D::CompressImageFile(src_image_file_path, cpt_file_path);
    }



    SPDLOG_INFO("finish");
    glfwDestroyWindow(window);
    glfwTerminate();
    system("pause");
    exit(EXIT_SUCCESS);


    //��ʼ��opengl
    init_opengl();
    compile_shader();
    CreateTexture("data/urban.jpg");
    //��ȡshader����ID
    mvp_location = glGetUniformLocation(program, "u_mvp");
    vpos_location = glGetAttribLocation(program, "a_pos");
    vcol_location = glGetAttribLocation(program, "a_color");
    a_uv_location = glGetAttribLocation(program, "a_uv");
    u_diffuse_texture_location = glGetUniformLocation(program, "u_diffuse_texture");

    create_buffer();
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
       

        //��ȡ������
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(49.f / 255, 77.f / 255, 121.f / 255, 1.f);
        //����ת��
        glm::mat4 model, view, projection, mvp;
        glm::mat4 trans = glm::translate(glm::vec3(0, 0, 0));
        static float rotate_eulerAngle = 0.f;
        rotate_eulerAngle += 1;
        glm::mat4 rotation = glm::eulerAngleYXZ(glm::radians(rotate_eulerAngle), glm::radians(rotate_eulerAngle), glm::radians(rotate_eulerAngle));
        glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f)); //����;
        model = trans * scale * rotation;

        view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        projection = glm::perspective(glm::radians(60.f), ratio, 1.f, 1000.f);

        mvp = projection * view * model;
       

        //ָ��GPU����(����ָ��������ɫ����Ƭ����ɫ��)
        glUseProgram(program);
        {
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);//���������޳�

            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
            //��ͼ����
             //��������Ԫ0
            glActiveTexture(GL_TEXTURE0);
            //�����ص�ͼƬ���������󶨵�����Ԫ0��Texture2D�ϡ�
            glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id_);
            //����Shader���������Ԫ0��ȡ��ɫ����
            glUniform1i(u_diffuse_texture_location, 0);

            glBindVertexArray(vao);

            //�ϴ��������ݲ����л���
            glDrawArrays(GL_TRIANGLES, 0, 6*6);
        }


        glfwSwapBuffers(window);
        glfwPollEvents();


    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}


