#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "component.h"
#include "glm/glm.hpp"
using namespace rttr;
class Transform:public Component
{
public:
	Transform();
	~Transform();
	void set_position(glm::vec3 position) { position_ = position; };
	void set_rotation(glm::vec3 rotation) { rotation_ = rotation; };
	void set_scale(glm::vec3 scale) { scale_ = scale; };
	glm::vec3 position() { return position_; };
	glm::vec3 rotation() { return rotation_; };
	glm::vec3 scale() { return scale_; };
private:
	glm::vec3 position_;
	glm::vec3 rotation_;
	glm::vec3 scale_;
};
#endif // !TRANSFORM_H
