#ifndef MESHRENDERER_H
#define MESHRENDERER_H
#include "material.h"
#include "component/component.h"
#include "rttr/registration.h"
#include "component/transform.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "meshfilter.h"
#include "glad/glad.h"
class MeshRenderer :public Component
{
public:
	MeshRenderer();
	~MeshRenderer();
    void SetMaterial(Material* material);//设置Material
    Material* material() { return material_; }
    void SetView(glm::mat4 view) { view_ = view; };
    void SetProjection(glm::mat4 projection) { projection = projection_; };
    void Render();//渲染
private:
    Material* material_;
    glm::mat4 view_;
    glm::mat4 projection_;
    unsigned int vertex_buffer_object_ = 0;//顶点缓冲区对象
    unsigned int element_buffer_object_ = 0;//索引缓冲区对象
    unsigned int vertex_array_object_ = 0;//顶点数组对象
};
#endif MESH_RENDERER_H