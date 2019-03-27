#pragma once

#include <iostream>

#include "RAW12.h"

enum DebayerAlgorithm {
	NEARESTNEIGHBOUR,
	LINEAR
};

class Debayer {
private:
	unsigned _width, _height, _total_pixels;
	uint16_t* red_channel_16bits, *green1_channel_16bits, *green2_channel_16bits, *blue_channel_16bits;
	uint16_t* result_16bits;
public:
	Debayer(RAW12& RAW12_Image);
	void debayer_RAW12(DebayerAlgorithm algo);
	uint16_t* get_result_16bits() {
		return result_16bits;
	}
};