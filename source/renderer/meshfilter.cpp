#include "meshfilter.h"
RTTR_REGISTRATION
{
    registration::class_<MeshFilter>("MeshFilter")
    .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}
MeshFilter::MeshFilter()
    :mesh_(nullptr) {

}

void MeshFilter::LoadMesh(std::string mesh_file_path)
{
    //��ȡ Mesh�ļ�ͷ
    ifstream input_file_stream(Application::data_path()+mesh_file_path, ios::in | ios::binary);
    MeshFileHead mesh_file_head;
    input_file_stream.read((char*)&mesh_file_head, sizeof(mesh_file_head));
    //��ȡ��������
    unsigned char* vertex_data = (unsigned char*)malloc(mesh_file_head.vertex_num_ * sizeof(Vertex));
    input_file_stream.read((char*)vertex_data, mesh_file_head.vertex_num_ * sizeof(Vertex));
    //��ȡ������������
    unsigned short* vertex_index_data = (unsigned short*)malloc(mesh_file_head.vertex_index_num_ * sizeof(unsigned short));
    input_file_stream.read((char*)vertex_index_data, mesh_file_head.vertex_index_num_ * sizeof(unsigned short));
    input_file_stream.close();

    mesh_ = new Mesh();
    mesh_->vertex_num_ = mesh_file_head.vertex_num_;
    mesh_->vertex_index_num_ = mesh_file_head.vertex_index_num_;
    mesh_->vertex_data_ = (Vertex*)vertex_data;
    mesh_->vertex_index_data_ = vertex_index_data;
}
MeshFilter::~MeshFilter() {
    delete(mesh_);
    mesh_ = nullptr;
}
