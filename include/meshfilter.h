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
	unsigned short vertex_num_;//顶点个数
	unsigned short vertex_index_num_;//索引个数
	Vertex* vertex_data_;//顶点数据
	unsigned short* vertex_index_data_;//顶点索引数据
	
};
class MeshFilter
{
public:
	void ExportMesh(string save_path, vector<Vertex> kVertexRemoveDumplicateVector, vector<unsigned short> kVertexIndexVector);
	void LoadMesh(std::string mesh_file_path);
	Mesh* mesh() { return mesh_; };
	Mesh* mesh_;
};