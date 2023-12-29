#include <renderer/shader.h>
unordered_map<string, Shader*> Shader::kShaderMap;
void Shader::Parse(string shader_name)
{
	this->shader_name = shader_name;
	string vertex_shader_file_path = Application::data_path()+ shader_name + ".vs";
	string fragment_shader_file_path = Application::data_path() + shader_name + ".fs";
	ifstream vertex_shader_input_file_stream(vertex_shader_file_path);
	string vertex_shader_source((std::istreambuf_iterator<char>(vertex_shader_input_file_stream)), std::istreambuf_iterator<char>());


	ifstream fragment_shader_input_file_stream(fragment_shader_file_path);
	string fragment_shader_source((std::istreambuf_iterator<char>(fragment_shader_input_file_stream)), std::istreambuf_iterator<char>());
    CreateGPUProgram(vertex_shader_source.c_str(), fragment_shader_source.c_str());
	
}

void Shader::CreateGPUProgram(const char* vertex_shader_text, const char* fragment_shader_text)
{
    GLuint vertex_shader, fragment_shader;
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
        std::cout << "compile vs error:" << message << endl;
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
    gl_program_id_ = glCreateProgram();
    //����Shader
    glAttachShader(gl_program_id_, vertex_shader);
    glAttachShader(gl_program_id_, fragment_shader);
    //Link
    glLinkProgram(gl_program_id_);
    //��ȡ������
    GLint link_status = GL_FALSE;
    glGetProgramiv(gl_program_id_, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE)
    {
        GLchar message[256];
        glGetProgramInfoLog(gl_program_id_, sizeof(message), 0, message);
        cout << "link error:" << message << endl;
    }
}

Shader* Shader::Find(string shader_name)
{
    
    auto iter = kShaderMap.find(shader_name);
    if (iter != kShaderMap.end())
    {
        return iter->second;
    }
    Shader* shader = new Shader();
    shader->Parse(shader_name);
    kShaderMap.insert(pair<string, Shader*>(shader_name, shader));
    return shader;
}
