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
	//�жϰ����Ƿ���
	static bool GetKey(unsigned char key_code);

	//�жϰ����Ƿ���״̬
	static bool GetKeyDown(unsigned char key_code);

	//�жϰ����Ƿ��²��ɿ�
	static bool GetKeyUp(unsigned char key_code);

	//��¼�����¼� 0���� 1���� 2��������
	static void RecordKey(unsigned short key_code, unsigned char key_action);

	//ˢ֡
	static void Update();
	//�ж��Ƿ������ĳ����ť  0��ť 1�Ұ�ť 2��ʾ�м䰴ť
	static bool GetMouseButton(unsigned short mouse_button_index);
	//ָ����갴���Ƿ���״̬
	static bool GetMouseButtonDown(unsigned short mouse_button_index);
	//��갴���Ƿ��ɿ�
	static bool GetMouseButtonUp(unsigned short mouse_button_index);
	static vec2_ushort mousePosition() { return mouse_position_; }
	/// �������λ��
	/// \param x
	/// \param y
	static void set_mousePosition(unsigned short x, unsigned short y) { mouse_position_.x_ = x; mouse_position_.y_ = y; }

	static short mouse_scroll() { return mouse_scroll_; }
	/// ��¼�������¼�
	/// \param mouse_scroll
	static void RecordScroll(short mouse_scroll) { mouse_scroll_ += mouse_scroll; }

private:
	static vec2_ushort mouse_position_;
	static short mouse_scroll_;
	static std::unordered_map<unsigned short, unsigned char> key_event_map_;
};
#endif // !INPUT_H
