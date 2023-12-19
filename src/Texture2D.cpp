#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
Texture2D* Texture2D::LoadFromFile(std::string& image_file_path)
{
    Texture2D* texture2d = new Texture2D();
    stbi_set_flip_vertically_on_load(true);//��תͼƬ������������ͼƬ���ݴ����½ǿ�ʼ��������ΪOpenGL������������ʼ��Ϊ���½ǡ�
    int channels_in_file;//ͨ����
    unsigned char* data = stbi_load(image_file_path.c_str(), &(texture2d->width), &(texture2d->height), &channels_in_file, 0);
    if (data != nullptr)
    {
        //������ɫͨ�������ж���ɫ��ʽ��
        switch (channels_in_file) {
        case 1:
        {
            texture2d->gl_texture_format_ = GL_ALPHA;
            break;
        }
        case 3:
        {
            texture2d->gl_texture_format_ = GL_RGB;
            break;
        }
        case 4:
        {
            texture2d->gl_texture_format_ = GL_RGBA;
            break;
        }
        }

    }
    else
    {
        std::cout << "loadTexture error "<< std::endl;
    }
    //1. ֪ͨ�Կ�����������󣬷��ؾ��;
    glGenTextures(1, &(texture2d->gl_texture_id_));

    //2. ������󶨵��ض�����Ŀ��;
    glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id_);

    //3. ��ͼƬrgb�����ϴ���GPU;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture2d->width, texture2d->height, 0, texture2d->gl_texture_format_, GL_UNSIGNED_BYTE, data);

    //4. ָ���Ŵ���С�˲���ʽ�������˲������Ŵ���С�Ĳ�ֵ��ʽ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //�ͷ�ͼƬ�ļ��ڴ�
    stbi_image_free(data);
    return texture2d;
}