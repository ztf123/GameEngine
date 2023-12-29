#ifndef MATERIAL_H
#define MATERIAL_H
#include "Texture2D.h"
#include <string>
#include "shader.h"
//#include <application.h>
using namespace std;
class Material
{
public:
	Material();
	~Material();
	void Parse(string material_path);
	Shader* shader() { return shader_; }
	void SetUniformMatrix4fv(std::string shader_property_name, float* pointer);
	void SetUniform1i(std::string shader_property_name, int value);
	std::vector<std::pair<std::string, Texture2D*>>& textures() { return textures_; }
private:
	Shader* shader_;
	std::vector<std::pair<std::string, Texture2D*>> textures_;

	std::vector<std::pair<std::string, float*>> uniform_matrix4fv_vec;
	std::vector<std::pair<std::string, int>> uniform_1i_vec;
};
#endif MATERIAL_H