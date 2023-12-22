#include "meshfilter.h"

void MeshFilter::ExportMesh(string save_path, vector<Vertex> kVertexRemoveDumplicateVector, vector<unsigned short> kVertexIndexVector)
{
    ofstream output_file_stream(save_path, ios::out | ios::binary);
    MeshFileHead mesh_file_head;
    mesh_file_head.type_[0] = 'm';
    mesh_file_head.type_[1] = 'e';
    mesh_file_head.type_[2] = 's';
    mesh_file_head.type_[3] = 'h';
    mesh_file_head.vertex_num_ = kVertexRemoveDumplicateVector.size();
    mesh_file_head.vertex_index_num_ = kVertexIndexVector.size();
    //д���ļ�ͷ
    output_file_stream.write((char*)&mesh_file_head, sizeof(mesh_file_head));
    //д�붥������
    output_file_stream.write((char*)&kVertexRemoveDumplicateVector[0], kVertexRemoveDumplicateVector.size() * sizeof(Vertex));
    //д����������
    output_file_stream.write((char*)&kVertexIndexVector[0], kVertexIndexVector.size() * sizeof(unsigned short));
    output_file_stream.close();
}

void MeshFilter::LoadMesh(std::string mesh_file_path)
{
    //��ȡ Mesh�ļ�ͷ
    ifstream input_file_stream(mesh_file_path, ios::in | ios::binary);
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