#include <glm/glm.hpp>
#include "material.h"
class MeshRenderer {
	MeshRenderer();
	~MeshRenderer();
    void SetMaterial(Material* material);//����Material
    Material* material() { return material_; }

    void Render();//��Ⱦ
private:
    Material* material_;

    unsigned int vertex_buffer_object_ = 0;//���㻺��������
    unsigned int element_buffer_object_ = 0;//��������������
    unsigned int vertex_array_object_ = 0;//�����������
};