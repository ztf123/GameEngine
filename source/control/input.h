#ifndef INPUT_H
#define INPUT_H
#include <unordered_map>
#include <glm/glm.hpp>
struct vec2_ushort
{
	unsigned short x_;
	unsigned short y_;
};
class Input
{
public:
	//判断按键是否按下
	static bool GetKey(unsigned char key_code);

	//判断按键是否按下状态
	static bool GetKeyDown(unsigned char key_code);

	//判断按键是否按下并松开
	static bool GetKeyUp(unsigned char key_code);

	//记录按键事件 0松手 1按下 2持续按下
	static void RecordKey(unsigned short key_code, unsigned char key_action);

	//刷帧
	static void Update();
	//判断是否按了鼠标某个按钮  0左按钮 1右按钮 2表示中间按钮
	static bool GetMouseButton(unsigned short mouse_button_index);
	//指定鼠标按键是否按下状态
	static bool GetMouseButtonDown(unsigned short mouse_button_index);
	//鼠标按键是否松开
	static bool GetMouseButtonUp(unsigned short mouse_button_index);
	static vec2_ushort mousePosition() { return mouse_position_; }
	/// 设置鼠标位置
	/// \param x
	/// \param y
	static void set_mousePosition(unsigned short x, unsigned short y) { mouse_position_.x_ = x; mouse_position_.y_ = y; }

	static short mouse_scroll() { return mouse_scroll_; }
	/// 记录鼠标滚轮事件
	/// \param mouse_scroll
	static void RecordScroll(short mouse_scroll) { mouse_scroll_ += mouse_scroll; }

private:
	static vec2_ushort mouse_position_;
	static short mouse_scroll_;
	static std::unordered_map<unsigned short, unsigned char> key_event_map_;
};
#endif // !INPUT_H
