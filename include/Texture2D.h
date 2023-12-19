#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <iostream>
#include <string>
#include <glad/glad.h>
class Texture2D
{
private:
	Texture2D() {};
	~Texture2D() {};
public:
	static Texture2D* LoadFromFile(std::string& image_file_path);
public:
	int width;
	int height;
	GLenum gl_texture_format_;
	GLuint gl_texture_id_;//Œ∆¿ÌID
};
#endif //TEXTURE2D_H