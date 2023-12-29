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
    int mipmap_level() { return mipmap_level_; }
    int width() { return width_; }
    int height() { return height_; }
    GLenum gl_texture_format() { return gl_texture_format_; }
    GLuint gl_texture_id() { return gl_texture_id_; }
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
private:
    int mipmap_level_;
	int width_;
	int height_;
	GLenum gl_texture_format_;
	GLuint gl_texture_id_;//纹理ID

};
#endif //TEXTURE2D_H