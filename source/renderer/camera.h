#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include "component/component.h"
#include <vector>
class Camera:public Component{
public:
	Camera();
	~Camera();
	void SetView(const glm::vec3& cameraFowrad, const glm::vec3 cameraUp);
	void SetProjection(float fovDegrees, float aspectRatio, float nearClip, float farClip);
	glm::mat4& view_mat4() { return view_mat4_; }
	glm::mat4& projection_mat4() { return projection_mat4_; }
	void set_clear_color(float r, float g, float b, float a) { clear_color_ = glm::vec4(r, g, b, a); }
	void set_clear_flag(unsigned int clear_flag) { clear_flag_ = clear_flag; }
	void Clear();
	unsigned char depth() { return depth_; }
	void set_depth(unsigned char depth);
	unsigned char culling_mask() { return culling_mask_; }
	void set_culling_mask(unsigned char culling_mask) { culling_mask_ = culling_mask; }
private:
	unsigned char depth_;
	unsigned char culling_mask_;
	glm::mat4 view_mat4_;
	glm::mat4 projection_mat4_;
	glm::vec4 clear_color_;
	unsigned int clear_flag_;
public:
	static void Sort();
	static void Foreach(std::function<void()> func);
	static Camera* current_camera() { return current_camera_; }
private:
	static std::vector<Camera*> all_camera_;
	static Camera* current_camera_;
};
#endif // !CAMERA_H
