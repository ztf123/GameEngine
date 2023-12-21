#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <iostream>
#include <string>
#include <glad/glad.h>
using namespace std;
class Texture2D
{
private:
	Texture2D() :mipmap_level_(0) {};
	~Texture2D() {};
public:
	static Texture2D* LoadFromFile(std::string& image_file_path);
public:

    int mipmap_level_;
	int width;
	int height;
	GLenum gl_texture_format_;
	GLuint gl_texture_id_;//纹理ID
     //tcp文件头
    struct CptFileHead
    {
        char type_[3];
        int mipmap_level_;
        int width_;
        int height_;
        int gl_texture_format_;
        int compress_size_;
    };
};
#endif //TEXTURE2D_H