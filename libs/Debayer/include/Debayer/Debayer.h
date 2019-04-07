#pragma once

#include <iostream>

#include "RAW12PPM/RAW12.h"

enum DebayerAlgorithm {
	NEARESTNEIGHBOUR,
	LINEAR
};

class Debayer {
private:
	unsigned _width, _height, _total_pixels;
	uint16_t* red_channel_12bits, *green1_channel_12bits, *green2_channel_12bits, *blue_channel_12bits;
	uint16_t* result_12bits;
public:
	Debayer(RAW12& RAW12_Image);
	void debayer_RAW12(DebayerAlgorithm algo);
	uint16_t* get_result_12bits() {
		return result_12bits;
	}
};
