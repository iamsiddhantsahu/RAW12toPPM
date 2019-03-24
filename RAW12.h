#pragma once

#include <iostream>
#include <fstream>

class RAW12 {
private:
	const char* filename;
	uint32_t _width, _height;
	uint32_t _total_pixels, _size_in_bytes;

	uint16_t* red_channel, *green1_channel, *green2_channel, *blue_channel;

	std::ifstream RAW12_file;

	uint16_t* buffer_16bits; 
	uint8_t* buffer_8bits;

	uint16_t* result_12bits;
	uint8_t* result_8bits;

public:
	RAW12();
	RAW12(const char* RAW12_filename, int width, int height);
	void buffer_8bits_to_12bits();
	void seperate_channels();
	void debayer();
	void write_ppm(const char* PPM_filename);
	~RAW12();
};