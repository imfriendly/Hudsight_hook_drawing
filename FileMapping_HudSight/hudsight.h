#pragma once

#include <stdint.h>
#include <Windows.h>
#include <string>


class Colour {
	Colour() = delete;
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	explicit Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		: r(red), g(green), b(blue), a(alpha) {
	}
};

class Hudsight
{
public:
	int32_t offset_width; //0x0000
	int32_t offset_height; //0x0004
	int32_t image_width; //0x0008
	int32_t image_height; //0x000C
	int32_t image_bytes_per_line; //0x0010
	float scale_width; //0x0014
	float scale_height; //0x0018
	float rotation_degrees; //0x001C
	float colour_mask_r; //0x0020
	float colour_mask_g; //0x0024
	float colour_mask_b; //0x0028
	float colour_mask_a; //0x002C
	int32_t current_id; //0x0030
	int32_t visible; //0x0034
	int64_t must_be_two; //0x0038
	uint8_t image_buffer[1]; // Size: image_width * image_height * 4
};

class HudsightDrawing {
	Hudsight* hs_;

private:
	int* overlay_file_;
	HANDLE overlay_index_file_;
	HANDLE overlay_buffer_file_;
	int last_index_;

	void cleanup_mappings();

	int get_index();
	Hudsight* get_hudsight(int index);

public:
	HudsightDrawing();
	~HudsightDrawing();

	bool frame_start();
	void frame_end();

	void draw_pixel(int x, int y, Colour colour);
	void draw_outline_rectangle(int x, int y, int width, int height, Colour colour);
	void draw_filled_rectangle(int x, int y, int width, int height, Colour colour);

	Hudsight* get_data() { return hs_; }
};