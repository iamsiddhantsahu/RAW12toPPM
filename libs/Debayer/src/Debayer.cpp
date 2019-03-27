#include "Debayer.h"

Debayer::Debayer(RAW12& RAW12_Image)
	: _width(RAW12_Image.get_width()), _height(RAW12_Image.get_height()),
	red_channel_16bits(RAW12_Image.get_red_channel_16bits()), green1_channel_16bits(RAW12_Image.get_green1_channel_16bits()),
	green2_channel_16bits(RAW12_Image.get_green2_channel_16bits()), blue_channel_16bits(RAW12_Image.get_blue_channel_16bits()),
	_total_pixels(RAW12_Image.get_total_pixels()) {

}

void Debayer::debayer_RAW12(DebayerAlgorithm algo) {

	result_16bits = new uint16_t[3 * _total_pixels];

	switch (algo) {
		case NEARESTNEIGHBOUR:
			unsigned int indexrg = 0, indexgb = 0, clrindex = 0;

			for (unsigned int index = 0; index < _total_pixels / 2; index++, clrindex += 2) {
				if ((index / _width) % 2 == 0) {
					//get values of missing pixels at R and G1

					for (uint32_t j = 0; j < 6; j += 3) {
						result_16bits[clrindex * 3 + j] = red_channel_16bits[indexrg];
						result_16bits[clrindex * 3 + j + 1] = (green1_channel_16bits[indexrg] + green2_channel_16bits[indexrg]) / 2;
						result_16bits[clrindex * 3 + j + 2] = blue_channel_16bits[indexrg];
					}

					indexrg++;
				}
				else {
					//get values of missing pixels at G2 and B

					for (uint32_t k = 0; k < 6; k += 3) {
						result_16bits[clrindex * 3 + k] = red_channel_16bits[indexgb];
						result_16bits[clrindex * 3 + k + 1] = (green1_channel_16bits[indexgb] + green2_channel_16bits[indexgb]) / 2;
						result_16bits[clrindex * 3 + k + 2] = blue_channel_16bits[indexgb];
					}

					indexgb++;
				}
			}
			break;
		case LINEAR:
			uint32_t imOff, grOff;
			for (uint32_t i = 0; i < _height; i++) {
				for (uint32_t j = 0; j < _width; j++) {
					imOff = 3 * (i * _width + j);
					grOff = (i / 2) * _width / 2 + (j / 2);
					result_16bits[imOff + 0] = red_channel_16bits[grOff];
					result_16bits[imOff + 1] = (green1_channel_16bits[grOff] + green2_channel_16bits[grOff]) / 2;
					result_16bits[imOff + 2] = blue_channel_16bits[grOff];
				}
			}
			break;
		default:
			break;
	}
}