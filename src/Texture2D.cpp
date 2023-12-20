#include "texture2d.h"
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <timetool/stopwatch.h>
Texture2D* Texture2D::LoadFromFile(std::string& image_file_path)
{
    Texture2D* texture2d = new Texture2D();
    stbi_set_flip_vertically_on_load(true);//��תͼƬ������������ͼƬ���ݴ����½ǿ�ʼ��������ΪOpenGL������������ʼ��Ϊ���½ǡ�
    int channels_in_file;//ͨ����
    unsigned char* data = stbi_load(image_file_path.c_str(), &(texture2d->width), &(texture2d->height), &channels_in_file, 0);

    spdlog::info("stbi_load {},is null:{}", image_file_path, data == nullptr);
    int image_data_format = GL_RGB;
    if (data != nullptr)
    {
        //������ɫͨ�������ж���ɫ��ʽ��
        switch (channels_in_file) {
        case 1:
        {
            image_data_format = GL_ALPHA;
            break;
        }
        case 3:
        {
            image_data_format = GL_RGB;
            texture2d->gl_texture_format_ = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
            break;
        }
        case 4:
        {
            image_data_format = GL_RGBA;
            texture2d->gl_texture_format_ = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
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
    glTexImage2D(GL_TEXTURE_2D, 0, texture2d->gl_texture_format_, texture2d->width, texture2d->height, 0, image_data_format, GL_UNSIGNED_BYTE, data);

    //4. ָ���Ŵ���С�˲���ʽ�������˲������Ŵ���С�Ĳ�ֵ��ʽ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //�ͷ�ͼƬ�ļ��ڴ�
    stbi_image_free(data);
    return texture2d;
}

void Texture2D::CompressImageFile(std::string& image_file_path,std::string& save_image_file_path)
{
    Texture2D* texture2d = LoadFromFile(image_file_path);
    //1. ��ȡѹ���Ƿ�ɹ�
    GLint compress_success = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &compress_success);
    SPDLOG_INFO("compress_success:{}", compress_success);

    //2. ��ȡѹ���õ��������ݳߴ�
    GLint compress_size = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compress_size);
    SPDLOG_INFO("compress_size:{}", compress_size);

    //3. ��ȡ���������ѹ����ʽ
    GLint compress_format = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &compress_format);
    SPDLOG_INFO("compress_format:{}", compress_format);

    //4. ��GPU�У����Դ��б����ѹ���õ��������ݣ����ص��ڴ�
    void* img = malloc(compress_size);
    glGetCompressedTexImage(GL_TEXTURE_2D, 0, img);
    std::ofstream output_file_stream(save_image_file_path, std::ios::out | std::ios::binary);
    CptFileHead cpt_file_head;
    cpt_file_head.type_[0] = 'c';
    cpt_file_head.type_[1] = 'p';
    cpt_file_head.type_[2] = 't';
    cpt_file_head.mipmap_level_ = texture2d->mipmap_level_;
    cpt_file_head.width_ = texture2d->width;
    cpt_file_head.height_ = texture2d->height;
    cpt_file_head.gl_texture_format_ = compress_format;
    cpt_file_head.compress_size_ = compress_size;
    
    output_file_stream.write((char*)&cpt_file_head, sizeof(CptFileHead));
    output_file_stream.write((char*)img, compress_size);
    output_file_stream.close();
}
