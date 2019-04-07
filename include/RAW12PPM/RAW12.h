#pragma once

#include <iostream>
#include <fstream>
#include <stdint.h>

class RAW12 {
private:
	const char* filename;
	unsigned int _width, _height;
	unsigned int _total_pixels, _size_in_bytes;

	uint16_t* red_channel_12bits, *green1_channel_12bits, *green2_channel_12bits, *blue_channel_12bits;

	std::ifstream RAW12_file;

	uint16_t* buffer_16bits;
	uint8_t* buffer_8bits;

	uint16_t* result_12bits;
	uint8_t* result_8bits;

public:
	RAW12();
	RAW12(const char* RAW12_filename, int width, int height);
	void buffer_8bits_to_12bits();
	void extract_channels();

	unsigned int get_width() {
		return _width;
	}
	unsigned int get_height() {
		return _height;
	}
	unsigned int get_total_pixels() {
		return _total_pixels;
	}
	uint16_t* get_red_channel_12bits() {
		return red_channel_12bits;
	}
	uint16_t* get_green1_channel_12bits() {
		return green1_channel_12bits;
	}
	uint16_t* get_green2_channel_12bits() {
		return green2_channel_12bits;
	}
	uint16_t* get_blue_channel_12bits() {
		return blue_channel_12bits;
	}
	uint8_t* get_buffer_8bits() {
		return buffer_8bits;
	}
	~RAW12();
};
