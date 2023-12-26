#include <string>
#include <fstream>
#include "VertexData.h"
using namespace std;

class MeshFilter
{
public:
    MeshFilter();
    ~MeshFilter();
public:
    //����
    struct Vertex
    {
        glm::vec3 pos_;
        glm::vec4 color_;
        glm::vec2 uv_;
    };

    //Mesh�ļ�ͷ
    struct MeshFileHead {
        char type_[4];
        unsigned short vertex_num_;//�������
        unsigned short vertex_index_num_;//��������
    };

    //Mesh����
    struct Mesh {
        unsigned short vertex_num_;//�������
        unsigned short vertex_index_num_;//��������
        Vertex* vertex_data_;//��������
        unsigned short* vertex_index_data_;//������������
    };

    void LoadMesh(string mesh_file_path);//����Mesh�ļ�

    Mesh* mesh() { return mesh_; };//Mesh����
private:
    Mesh* mesh_;//Mesh����
};