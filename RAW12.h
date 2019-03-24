#pragma once

#include <iostream>
#include <fstream>

class RAW12 {
private:
	const char* filename;
	unsigned int _width, _height;
	unsigned int _total_pixels, _size_in_bytes;

	uint16_t* red_channel, *green1_channel, *green2_channel, *blue_channel;
	uint8_t* red_channel_8bits, *green1_channel_8bits, *green2_channel_8bits, *blue_channel_8bits;

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
	void debayer_nearest_neighbour();
	void debayer_bilinear();
	void write_ppm(const char* PPM_filename);
	void write_red_pgm(const char* filename);
	void write_green1_pgm(const char* filename);
	void write_green2_pgm(const char* filename);
	void write_blue_pgm(const char* filename);
	~RAW12();
};