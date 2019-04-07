#pragma once

#include <iostream>
#include "Debayer/Debayer.h"
#include "RAW12PPM/RAW12.h"

enum Channel {
	RED,
	GREEN1,
	GREEN2,
	BLUE
};

class PPM {
private:
	unsigned _width, _height, _total_pixels;

	uint16_t* buffer_12bits;
	uint8_t* buffer_8bits;

	uint16_t* result_12bits;
	uint8_t* result_8bits;

	uint16_t* red_channel_12bits, *green1_channel_12bits, *green2_channel_12bits, *blue_channel_12bits;
	uint8_t* red_channel_8bits, *green1_channel_8bits, *green2_channel_8bits, *blue_channel_8bits;

	std::ofstream ofs;

public:
	PPM(Debayer& DebayeredImage, RAW12& RAW12Image);
	void print_5X5();
	void write_pgm(const char* filename, Channel channel); //for grayscale (red, green1, green2, blue)
	void write_ppm(const char* filename);
};
