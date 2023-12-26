#include <glm/glm.hpp>
#include "material.h"
class MeshRenderer {
	MeshRenderer();
	~MeshRenderer();
    void SetMaterial(Material* material);//设置Material
    Material* material() { return material_; }

    void Render();//渲染
private:
    Material* material_;

    unsigned int vertex_buffer_object_ = 0;//顶点缓冲区对象
    unsigned int element_buffer_object_ = 0;//索引缓冲区对象
    unsigned int vertex_array_object_ = 0;//顶点数组对象
};