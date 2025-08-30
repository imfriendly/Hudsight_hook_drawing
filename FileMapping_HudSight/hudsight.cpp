#include "hudsight.h"


HudsightDrawing::HudsightDrawing()
{
  overlay_file_ = nullptr;
  overlay_index_file_ = NULL;
  overlay_buffer_file_ = NULL;
  last_index_ = -1;
	hs_ = get_hudsight(get_index());
}

HudsightDrawing::~HudsightDrawing()
{
	cleanup_mappings();
}


void HudsightDrawing::cleanup_mappings()
{
  if (overlay_file_) {
    UnmapViewOfFile(overlay_file_);
    overlay_file_ = nullptr;
  }

  if (overlay_index_file_) {
    CloseHandle(overlay_index_file_);
    overlay_index_file_ = NULL;
  }

  if (hs_) {
    UnmapViewOfFile(hs_);
    hs_ = nullptr;
  }

  if (overlay_buffer_file_) {
    CloseHandle(overlay_buffer_file_);
    overlay_buffer_file_ = NULL;
  }
}

int HudsightDrawing::get_index()
{
  if (overlay_file_) {
    return *overlay_file_;
  }

  overlay_index_file_ = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "HudSight 2OverlayIndexBuffer");
  if (overlay_index_file_ == NULL) {
    return -1;
  }

  // Map a view of the file mapping object into the address space of the calling process
  overlay_file_ = static_cast<int*>(MapViewOfFile(overlay_index_file_, FILE_MAP_ALL_ACCESS, 0, 0, 0));
  if (overlay_file_ == NULL) {
    CloseHandle(overlay_index_file_);
    overlay_index_file_ = NULL;
    return -1;
  }

  return *overlay_file_;
}

Hudsight* HudsightDrawing::get_hudsight(int index)
{
  if (index == -1) {
    return nullptr;
  }

  if (last_index_ == index && hs_) {
    return hs_;
  }

  if (hs_ && overlay_buffer_file_) {
    UnmapViewOfFile(hs_);
    hs_ = nullptr;

    CloseHandle(overlay_buffer_file_);
    overlay_buffer_file_ = NULL;
  }

  auto mapping_name = L"HudSight 2OverlayBuffer_" + std::to_wstring(index);
  overlay_buffer_file_ = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, mapping_name.c_str());
  if (overlay_buffer_file_ == NULL) {
    return nullptr;
  }

  hs_ = static_cast<Hudsight*>(MapViewOfFile(overlay_buffer_file_, FILE_MAP_ALL_ACCESS, 0, 0, 0));
  if (hs_ == NULL) {
    CloseHandle(overlay_buffer_file_);
    overlay_buffer_file_ = NULL;
    return nullptr;
  }

  last_index_ = index;

  return hs_;
}

bool HudsightDrawing::frame_start()
{
  int index = get_index();
  if (index == -1) {
    hs_ = nullptr;
    return false;
  }

  hs_ = get_hudsight(index);

	return hs_ != nullptr;
}

void HudsightDrawing::frame_end()
{
  hs_->current_id++;
}

void HudsightDrawing::draw_pixel(int x, int y, Colour colour)
{
	if (hs_ == nullptr)
		return;

	if (x < 0 || x >= hs_->image_width || y < 0 || y >= hs_->image_height)
		return;

	size_t index = (static_cast<size_t>(y) * static_cast<size_t>(hs_->image_width) + static_cast<size_t>(x)) * 4;
	hs_->image_buffer[index] = colour.r;
	hs_->image_buffer[index + 1] = colour.g;
	hs_->image_buffer[index + 2] = colour.b;
	hs_->image_buffer[index + 3] = colour.a;
}

void HudsightDrawing::draw_outline_rectangle(int x, int y, int width, int height, Colour colour)
{
	for (int i = 0; i < width; ++i) {
		draw_pixel(x + i, y, colour);
		draw_pixel(x + i, y + height - 1, colour); 
	}
	for (int i = 0; i < height; ++i) {
		draw_pixel(x, y + i, colour); 
		draw_pixel(x + width - 1, y + i, colour);
	}
}

void HudsightDrawing::draw_filled_rectangle(int x, int y, int width, int height, Colour colour)
{
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			draw_pixel(x + j, y + i, colour);
		}
	}
}
