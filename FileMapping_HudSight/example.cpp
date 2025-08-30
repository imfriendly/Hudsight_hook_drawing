#include "hudsight.h"

int main() {
	float rotation = 0.0f;

	auto hudsight = HudsightDrawing();
	while (true) {
		Sleep(1);

		if (!hudsight.frame_start())
			continue;

		rotation += 2.f;
		if (rotation >= 360.0f)
			rotation -= 360.0f;

		hudsight.get_data()->rotation_degrees = rotation;

		hudsight.draw_filled_rectangle(10, 10, 100, 100, Colour(255, 0, 0, 255));

		hudsight.frame_end();
	}
}