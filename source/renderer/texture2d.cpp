#include "texture2d.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <fstream>
#include "utils/application.h"
Texture2D* Texture2D::LoadFromFile(std::string& image_file_path)
{
    Texture2D* texture2d = new Texture2D();


    //��ȡ cpt ѹ�������ļ�
    ifstream input_file_stream(Application::data_path()+image_file_path, ios::in | ios::binary);
    CptFileHead cpt_file_head;
    input_file_stream.read((char*)&cpt_file_head, sizeof(CptFileHead));

    unsigned char* data = (unsigned char*)malloc(cpt_file_head.compress_size_);
    input_file_stream.read((char*)data, cpt_file_head.compress_size_);
    input_file_stream.close();


    texture2d->gl_texture_format_ = cpt_file_head.gl_texture_format_;
    texture2d->width_ = cpt_file_head.width_;
    texture2d->height_ = cpt_file_head.height_;


    //1. ֪ͨ�Կ�����������󣬷��ؾ��;
    glGenTextures(1, &(texture2d->gl_texture_id_));

    //2. ������󶨵��ض�����Ŀ��;
    glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id_);

   
        //3. ��ѹ�����������ϴ���GPU;
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, texture2d->gl_texture_format_, texture2d->width_, texture2d->height_, 0, cpt_file_head.compress_size_, data);

    //4. ָ���Ŵ���С�˲���ʽ�������˲������Ŵ���С�Ĳ�ֵ��ʽ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    delete (data);

    return texture2d;
}
