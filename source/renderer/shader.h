#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <unordered_map>
#include "utils/application.h"
using namespace std;

class Shader
{
public:
	void Parse(string shader_name);
	unsigned int gl_program_id() { return gl_program_id_; }//glCreateProgram()返回的GPU程序句柄;
private:
	void CreateGPUProgram(const char* vertex_shader_text, const char* fragment_shader_text);
	string shader_name;
	unsigned int gl_program_id_;//glCreateProgram()返回的GPU程序句柄;
public:
	static Shader* Find(string shader_name);
private:
	static  unordered_map<string, Shader*> kShaderMap;//已经创建的Shader
};
#endif SHADER_H