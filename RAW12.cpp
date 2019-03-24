#include "RAW12.h"

RAW12::RAW12()
	: filename(nullptr), _width(0), _height(0) {
}

RAW12::RAW12(const char* RAW12_filename, int width, int height) 
	: filename(RAW12_filename), _width(width), _height(height) {

	std::cout << "Filename = " << filename << "\n";
	std::cout << "Width = " << _width << "\n";
	_total_pixels = _width * _height;
	std::cout << "Total Pixels = " << _total_pixels << "\n";
	_size_in_bytes = (_width * _height * 12) / 8;
	std::cout << "Total Size in Bytes = " << _size_in_bytes << "\n";

	RAW12_file.open(filename, std::ios::binary);
	if (!RAW12_file) {
		std::cout << "Error! Cannot open input file.\n";
	}

	buffer_8bits = new uint8_t[_size_in_bytes]; //create 8bits buffer
	RAW12_file.read(reinterpret_cast<char*>(buffer_8bits), _size_in_bytes);
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

void RAW12::seperate_channels() {
	std::cout << "Inside seperate_channels() function\n";
	red_channel = new uint16_t[_total_pixels / 4];
	green1_channel = new uint16_t[_total_pixels / 4];
	green2_channel = new uint16_t[_total_pixels / 4];
	blue_channel = new uint16_t[_total_pixels / 4];

	unsigned long long int counter_pixel, counter_row;
	unsigned long long int counter_pattern1 = 0, counter_pattern2 = 0; //pattern1 for RGRG..(odd rows) & pattern2 for GBGB..(even rows)
	for (counter_pixel = 0; counter_pixel < _total_pixels - 1; counter_pixel = counter_pixel + 2) {
		counter_row = (counter_pixel / _width);
		if (counter_row % 2 == 0) { //for odd rows
			red_channel[counter_pattern1] = buffer_16bits[counter_pixel];
			green1_channel[counter_pattern1] = buffer_16bits[counter_pixel + 1];
			counter_pattern1++;
		}
		else { //for even rows
			green2_channel[counter_pattern2] = buffer_16bits[counter_pixel];
			blue_channel[counter_pattern2] = buffer_16bits[counter_pixel + 1];
			counter_pattern2++;
		}
		//std::cout << "Inside for loop of seperate_channels() function\n";
	}
	std::cout << "Ended separate-channnels() function\n";
}

void RAW12::debayer_nearest_neighbour() {
	std::cout << "Inside Debayer function\n";
	result_12bits = new uint16_t[3 * _total_pixels];

	unsigned int indexrg = 0, indexgb = 0, clrindex = 0;

	for (unsigned int index = 0; index < _total_pixels / 2; index++, clrindex += 2) {
		if ((index / _width) % 2 == 0) {
			//get values of missing pixels at R and G1

			for (uint32_t j = 0; j < 6; j += 3) {
				result_12bits[clrindex * 3 + j] = red_channel[indexrg];
				result_12bits[clrindex * 3 + j + 1] = (green1_channel[indexrg] + green2_channel[indexrg]) / 2;
				result_12bits[clrindex * 3 + j + 2] = blue_channel[indexrg];
			}

			indexrg++;
		}
		else {
			//get values of missing pixels at G2 and B

			for (uint32_t k = 0; k < 6; k += 3) {
				result_12bits[clrindex * 3 + k] = red_channel[indexgb];
				result_12bits[clrindex * 3 + k + 1] = (green1_channel[indexgb] + green2_channel[indexgb]) / 2;
				result_12bits[clrindex * 3 + k + 2] = blue_channel[indexgb];
			}

			indexgb++;
		}
	}
}

void RAW12::debayer_bilinear() {
	std::cout << "Inside Debayer function\n";
	result_12bits = new uint16_t[3 * _total_pixels];
	uint32_t imOff, grOff;
	for (uint32_t i = 0; i < _height; i++) {
		for (uint32_t j = 0; j < _width; j++) {
			imOff = 3 * (i * _width + j);
			grOff = (i / 2) * _width / 2 + (j / 2);
			result_12bits[imOff + 0] = red_channel[grOff];
			result_12bits[imOff + 1] = (green1_channel[grOff] + green2_channel[grOff]) / 2;
			result_12bits[imOff + 2] = blue_channel[grOff];
		}
	}
}

void RAW12::write_ppm(const char* filename) {
	std::cout << "Inside write PPM function\n";
	if (_width == 0 || _height == 0) {
		fprintf(stderr, "Can't save an empty image\n");
	}

	std::ofstream ofs;
	try {
		ofs.open(filename, std::ios::binary);
		if (ofs.fail()) {
			throw("Can't open output file");
		}

		result_8bits = new uint8_t[(unsigned)_total_pixels * 3];
		//convert 12bpc to 8bpc
		std::cout << "Started Converting 12bpc to 8bpc\n";
		for (unsigned long int index = 0; index < (_total_pixels * 3); index++) {
			result_8bits[index] = (result_12bits[index]) >> 4;
			//std::cout << (unsigned)result_8bits[index] << " "; 
		}
		std::cout << "Converted 12bpc to 8bpc\n";

		ofs << "P3\n" << _width << " " << _height << "\n255\n";
		//loop over each pixel in the image, clamp and convert to byte format
		for (unsigned long int index = 0; index < (_total_pixels * 3); index++) {
			ofs << (unsigned)result_8bits[index] << " ";
		}
		ofs.close();
	}
	catch (const char* err) {
		fprintf(stderr, "%s\n", err);
		ofs.close();
	}
}

void RAW12::write_red_pgm(const char* filename) {
	std::cout << "Inside write PGM function\n";
	if (_width == 0 || _height == 0) {
		fprintf(stderr, "Can't save an empty image\n");
	}

	std::ofstream ofs;
	try {
		ofs.open(filename, std::ios::binary);
		if (ofs.fail()) {
			throw("Can't open output file");
		}

		red_channel_8bits = new uint8_t[(unsigned)(_total_pixels / 4)];
		//convert 12bpc to 8bpc
		std::cout << "Started Converting 12bpc to 8bpc\n";
		for (unsigned long int index = 0; index < (_total_pixels / 4); index++) {
			red_channel_8bits[index] = (red_channel[index]) >> 4;
			//std::cout << (unsigned)result_8bits[index] << " "; 
		}
		std::cout << "Converted 12bpc to 8bpc\n";

		ofs << "P2\n" << _width / 2 << " " << _height / 2 << "\n255\n";
		//loop over each pixel in the image, clamp and convert to byte format
		for (unsigned long int index = 0; index < (_total_pixels / 4); index++) {
			ofs << (unsigned)red_channel_8bits[index] << " ";
		}
		ofs.close();
	}
	catch (const char* err) {
		fprintf(stderr, "%s\n", err);
		ofs.close();
	}
}

void RAW12::write_green1_pgm(const char* filename) {
	std::cout << "Inside write PGM function\n";
	if (_width == 0 || _height == 0) {
		fprintf(stderr, "Can't save an empty image\n");
	}

	std::ofstream ofs;
	try {
		ofs.open(filename, std::ios::binary);
		if (ofs.fail()) {
			throw("Can't open output file");
		}

		green1_channel_8bits = new uint8_t[(unsigned)(_total_pixels / 4)];
		//convert 12bpc to 8bpc
		std::cout << "Started Converting 12bpc to 8bpc\n";
		for (unsigned long int index = 0; index < (_total_pixels / 4); index++) {
			green1_channel_8bits[index] = (green1_channel[index]) >> 4;
			//std::cout << (unsigned)result_8bits[index] << " "; 
		}
		std::cout << "Converted 12bpc to 8bpc\n";

		ofs << "P2\n" << _width / 2 << " " << _height / 2 << "\n255\n";
		//loop over each pixel in the image, clamp and convert to byte format
		for (unsigned long int index = 0; index < (_total_pixels / 4); index++) {
			ofs << (unsigned)green1_channel_8bits[index] << " ";
		}
		ofs.close();
	}
	catch (const char* err) {
		fprintf(stderr, "%s\n", err);
		ofs.close();
	}
}

void RAW12::write_green2_pgm(const char* filename) {
	std::cout << "Inside write PGM function\n";
	if (_width == 0 || _height == 0) {
		fprintf(stderr, "Can't save an empty image\n");
	}

	std::ofstream ofs;
	try {
		ofs.open(filename, std::ios::binary);
		if (ofs.fail()) {
			throw("Can't open output file");
		}

		green2_channel_8bits = new uint8_t[(unsigned)(_total_pixels / 4)];
		//convert 12bpc to 8bpc
		std::cout << "Started Converting 12bpc to 8bpc\n";
		for (unsigned long int index = 0; index < (_total_pixels / 4); index++) {
			green2_channel_8bits[index] = (green2_channel[index]) >> 4;
			//std::cout << (unsigned)result_8bits[index] << " "; 
		}
		std::cout << "Converted 12bpc to 8bpc\n";

		ofs << "P2\n" << _width / 2 << " " << _height / 2 << "\n255\n";
		//loop over each pixel in the image, clamp and convert to byte format
		for (unsigned long int index = 0; index < (_total_pixels / 4); index++) {
			ofs << (unsigned)green2_channel_8bits[index] << " ";
		}
		ofs.close();
	}
	catch (const char* err) {
		fprintf(stderr, "%s\n", err);
		ofs.close();
	}
}

void RAW12::write_blue_pgm(const char* filename) {
	std::cout << "Inside write PGM function\n";
	if (_width == 0 || _height == 0) {
		fprintf(stderr, "Can't save an empty image\n");
	}

	std::ofstream ofs;
	try {
		ofs.open(filename, std::ios::binary);
		if (ofs.fail()) {
			throw("Can't open output file");
		}

		blue_channel_8bits = new uint8_t[(unsigned)(_total_pixels / 4)];
		//convert 12bpc to 8bpc
		std::cout << "Started Converting 12bpc to 8bpc\n";
		for (unsigned long int index = 0; index < (_total_pixels / 4); index++) {
			blue_channel_8bits[index] = (blue_channel[index]) >> 4;
			//std::cout << (unsigned)result_8bits[index] << " "; 
		}
		std::cout << "Converted 12bpc to 8bpc\n";

		ofs << "P2\n" << _width / 2 << " " << _height / 2 << "\n255\n";
		//loop over each pixel in the image, clamp and convert to byte format
		for (unsigned long int index = 0; index < (_total_pixels / 4); index++) {
			ofs << (unsigned)blue_channel_8bits[index] << " ";
		}
		ofs.close();
	}
	catch (const char* err) {
		fprintf(stderr, "%s\n", err);
		ofs.close();
	}
}

RAW12::~RAW12() {
	delete[] buffer_8bits;
	delete[] buffer_16bits;
	delete[] red_channel;
	delete[] green1_channel;
	delete[] green2_channel;
	delete[] blue_channel;
}
