#include <string>
#include <fstream>
#include "VertexData.h"
using namespace std;
struct MeshFileHead
{
	char type_[4];
	int vertex_num_;
	int vertex_index_num_;
};
struct Mesh
{
	unsigned short vertex_num_;//�������
	unsigned short vertex_index_num_;//��������
	Vertex* vertex_data_;//��������
	unsigned short* vertex_index_data_;//������������
	
};
class MeshFilter
{
public:
	void ExportMesh(string save_path, vector<Vertex> kVertexRemoveDumplicateVector, vector<unsigned short> kVertexIndexVector);
	void LoadMesh(std::string mesh_file_path);
	Mesh* mesh() { return mesh_; };
	Mesh* mesh_;
};