#ifndef SCREEN_H
#define SCREEN_H
class Screen {
public:
	static int width() { return width_; }
	static int height() { return height_; }
	static float aspect_ratio() { return aspect_ratio_; }
	static void set_width(int width) { width_ = width; }
	static void set_height(int height) { height_ = height; }
	static void set_width_height(int width, int height) { width_ = width; height_ = height; calculate_aspect_ratio(); }

private:
	static void calculate_aspect_ratio() { aspect_ratio_ = width_ * 1.0f / height_; }
private:
	static int width_;
	static int height_;
	static float aspect_ratio_;
};
#endif // !SCREEN_H
