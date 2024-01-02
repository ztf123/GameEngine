#include "mesh_renderer.h"
using namespace rttr;
RTTR_REGISTRATION
{
registration::class_<MeshRenderer>("MeshRenderer")
.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}
MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::SetMaterial(Material* material)
{
	material_ = material;
}

void MeshRenderer::Render()
{
    Camera* current_camera = Camera::current_camera();
    if (current_camera == nullptr)
    {
        return;
    }
    if ((current_camera->culling_mask()&game_object()->layer()) == 0x00)
    {
        return;
    }
    glm::mat4 view = current_camera->view_mat4();
    glm::mat4 projection = current_camera->projection_mat4();

	auto component_transform = game_object()->GetComponent("Transform");
	Transform* transform = dynamic_cast<Transform*>(component_transform);
	if (!transform)
	{
		return;
	}
    //����ת��
    glm::mat4 model, mvp;
    glm::mat4 trans = glm::translate(transform->position());
    auto rotation = transform->rotation();
    glm::mat4 eulerAngleYXZ = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
    glm::mat4 scale = glm::scale(transform->scale()); //����;
    model = trans * scale * eulerAngleYXZ;
    mvp = projection * view* model;

    auto component_mesh_filter = game_object()->GetComponent("MeshFilter");
    MeshFilter* mesh_filter = dynamic_cast<MeshFilter*>(component_mesh_filter);
    if (!mesh_filter)
    {
        return;
    }
    GLuint gl_program_id = material_->shader()->gl_program_id();
    if (vertex_array_object_ == 0)
    {
        GLint vpos_location = glGetAttribLocation(gl_program_id, "a_pos");
        GLint vcol_location = glGetAttribLocation(gl_program_id, "a_color");
        GLint a_uv_location = glGetAttribLocation(gl_program_id, "a_uv");

        glGenVertexArrays(1, &vertex_array_object_);
        glGenBuffers(1, &vertex_buffer_object_);
        glGenBuffers(1, &element_buffer_object_);

        glBindVertexArray(vertex_array_object_);
        //������������ָ��Ϊ���㻺��������
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
        //�ϴ��������ݵ�����������
        glBufferData(GL_ARRAY_BUFFER, mesh_filter->mesh()->vertex_num_ * sizeof(MeshFilter::Vertex), mesh_filter->mesh()->vertex_data_, GL_STATIC_DRAW);


        //������������ָ��Ϊ������������������
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);
        //�ϴ������������ݵ�����������
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_filter->mesh()->vertex_index_num_ * sizeof(unsigned short), mesh_filter->mesh()->vertex_index_data_, GL_STATIC_DRAW);
        glVertexAttribPointer(vpos_location, 3, GL_FLOAT, false, sizeof(MeshFilter::Vertex), (void*)0);
        glEnableVertexAttribArray(vpos_location);//���ö���Shader����(a_pos)��ָ���붥���������ݽ��й���

        glVertexAttribPointer(vcol_location, 4, GL_FLOAT, false, sizeof(MeshFilter::Vertex), (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(vcol_location);//���ö���Shader����(a_color)��ָ���붥����ɫ���ݽ��й���

        glVertexAttribPointer(a_uv_location, 2, GL_FLOAT, false, sizeof(MeshFilter::Vertex), (void*)(sizeof(float) * (3 + 4)));
        glEnableVertexAttribArray(a_uv_location);//���ö���Shader����(a_color)��ָ���붥����ɫ���ݽ��й���

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    glUseProgram(gl_program_id);
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);//���������޳�

        glUniformMatrix4fv(glGetUniformLocation(gl_program_id, "u_mvp"), 1, GL_FALSE, &mvp[0][0]);
        std::vector<std::pair<std::string, Texture2D*>> textures = material_->textures();
        for (int texture_index = 0; texture_index < textures.size(); ++texture_index)
        {
            GLint u_texture_location = glGetUniformLocation(gl_program_id, textures[texture_index].first.c_str());
            //��������Ԫ0
            glActiveTexture(GL_TEXTURE0 + texture_index);
            //�����ص�ͼƬ���������󶨵�����Ԫ0��Texture2D�ϡ�
            glBindTexture(GL_TEXTURE_2D, textures[texture_index].second->gl_texture_id());
            //����Shader���������Ԫ0��ȡ��ɫ����
            glUniform1i(u_texture_location, texture_index);
        }


        glBindVertexArray(vertex_array_object_);

        //�ϴ��������ݲ����л���
        glDrawElements(GL_TRIANGLES, mesh_filter->mesh()->vertex_index_num_, GL_UNSIGNED_SHORT, 0);//ʹ�ö����������л��ƣ�����0��ʾ����ƫ������
        glBindVertexArray(0);
    }

}
