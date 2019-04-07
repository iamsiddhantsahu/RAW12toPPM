#include "RAW12PPM/RAW12.h"
#include <fstream>

RAW12::RAW12()
	: filename(nullptr), _width(0), _height(0) {
}

RAW12::RAW12(const char* RAW12_filename, int width, int height)
	: filename(RAW12_filename), _width(width), _height(height) {

	std::cout << "Filename = " << filename << "\n";
	std::cout << "----------------------------\n";
	std::cout << "Width = " << _width << "\n";
	std::cout << "Height = " << _height << "\n";
	_total_pixels = _width * _height;
	std::cout << "Total Pixels = " << _total_pixels << "\n";
	_size_in_bytes = (_width * _height * 12) / 8;
	std::cout << "Total Size in Bytes = " << _size_in_bytes << "\n";

	//std::ifstream RAW12_file;
	RAW12_file.open(filename, std::ios::binary);
	if (RAW12_file.is_open()==false) {
		std::cout << "Error! Cannot open input file.\n";
	}

	buffer_8bits = new uint8_t[_size_in_bytes]; //create 8bits buffer
	RAW12_file.read(reinterpret_cast<char*>(buffer_8bits), _size_in_bytes); //store the RAW12 image in 8bits buffer
}

/* Data in buffer_8bits
|               |               |               |
|R|R|R|R|R|R|R|R|R|R|R|R|G|G|G|G|G|G|G|G|G|G|G|G|
|<----8bits---->|<4bits>|<4bits>|<----8bits---->|
*/
void RAW12::buffer_8bits_to_12bits() {
	std::cout << "Started buffer_8bits_to_12bits() function\n";

	unsigned long int counter_8bits, counter_16bits = 0;
	buffer_16bits = new uint16_t[_total_pixels]; //create 16bits buffer

	std::cout << "Before for loop\n";
	for (counter_8bits = 0; counter_8bits < _size_in_bytes; counter_8bits++) {
		//std::cout << "Inside for loop\n";
		if (counter_16bits % 2 == 0) { //for red_channel and green2_channnel
			buffer_16bits[counter_16bits] = buffer_8bits[counter_8bits];
			/* Data in buffer_12bits
			|               |               |
			|-|-|-|-|-|-|-|-|R|R|R|R|R|R|R|R|
			*/
			buffer_16bits[counter_16bits] = buffer_16bits[counter_16bits] << 4;
			/* Data in buffer_12bits after 4bits left shift
			|               |               |
			|-|-|-|-|R|R|R|R|R|R|R|R|-|-|-|-|
			*/
			buffer_16bits[counter_16bits] = (buffer_16bits[counter_16bits] | ((buffer_8bits[counter_8bits + 1] >> 4) & 0x0F));
			/* Data in buffer_12bits after bitwiseOR with (right shifted 4bits next byte in buffer_8bits and bitwiseAND of 0x0F)
			|               |               |
			|-|-|-|-|R|R|R|R|R|R|R|R|R|R|R|R|
			*/
			counter_16bits++;
		}
		else { //for green1_channel and blue_channel
			buffer_16bits[counter_16bits] = (buffer_8bits[counter_8bits] & 0x0F);
			/* Data in buffer_12bits
			|               |               |
			|-|-|-|-|-|-|-|-|-|-|-|-|G|G|G|G|
			*/
			buffer_16bits[counter_16bits] = buffer_16bits[counter_16bits] << 8;
			/* Data in buffer_12bits after 8bits left shift
			|               |               |
			|-|-|-|-|G|G|G|G|-|-|-|-|-|-|-|-|
			*/
			buffer_16bits[counter_16bits] = (buffer_16bits[counter_16bits] | buffer_8bits[counter_8bits + 1]);
			/* Data in buffer_12bits after bitwiseOR with (right shifted 4bits and bitwiseAND of 0x0F)
			|               |               |
			|-|-|-|-|G|G|G|G|G|G|G|G|G|G|G|G|
			*/
			counter_8bits++;
			counter_16bits++;
		}
	}

	if (counter_8bits == _size_in_bytes) {
		std::cout << "Came out of for loop\n";
	}

	std::cout << "End of buffer_8bits_to_12bits() function\n";
}

void RAW12::extract_channels() {
	std::cout << "Inside seperate_channels() function\n";
	red_channel_12bits = new uint16_t[_total_pixels / 4];
	green1_channel_12bits = new uint16_t[_total_pixels / 4];
	green2_channel_12bits = new uint16_t[_total_pixels / 4];
	blue_channel_12bits = new uint16_t[_total_pixels / 4];

	unsigned long long int counter_pixel, counter_row;
	unsigned long long int counter_pattern1 = 0, counter_pattern2 = 0; //pattern1 for RGRG..(odd rows) & pattern2 for GBGB..(even rows)
	for (counter_pixel = 0; counter_pixel < _total_pixels - 1; counter_pixel = counter_pixel + 2) {
		counter_row = (counter_pixel / _width);
		if (counter_row % 2 == 0) { //for odd rows
			red_channel_12bits[counter_pattern1] = buffer_16bits[counter_pixel];
			green1_channel_12bits[counter_pattern1] = buffer_16bits[counter_pixel + 1];
			counter_pattern1++;
		}
		else { //for even rows
			green2_channel_12bits[counter_pattern2] = buffer_16bits[counter_pixel];
			blue_channel_12bits[counter_pattern2] = buffer_16bits[counter_pixel + 1];
			counter_pattern2++;
		}
		//std::cout << "Inside for loop of seperate_channels() function\n";
	}
	std::cout << "Ended separate-channnels() function\n";
}

RAW12::~RAW12() {
	delete[] buffer_8bits;
	delete[] buffer_16bits;
	delete[] red_channel_12bits;
	delete[] green1_channel_12bits;
	delete[] green2_channel_12bits;
	delete[] blue_channel_12bits;
}
