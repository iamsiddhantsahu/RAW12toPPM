#include "RAW12PPM/PPM.h"
#include "RAW12PPM/RAW12.h"
#include "Debayer/Debayer.h"

PPM::PPM(Debayer& DebayeredImage, RAW12& RAW12Image)
	: _width(RAW12Image.get_width()),
	_height(RAW12Image.get_height()),
	_total_pixels(RAW12Image.get_total_pixels()),
	red_channel_12bits(RAW12Image.get_red_channel_12bits()),
	green1_channel_12bits(RAW12Image.get_green1_channel_12bits()),
	green2_channel_12bits(RAW12Image.get_green2_channel_12bits()),
	blue_channel_12bits(RAW12Image.get_blue_channel_12bits()),
	buffer_8bits(RAW12Image.get_buffer_8bits()),
	result_12bits(DebayeredImage.get_result_12bits()) {

}

void PPM::print_5X5() {
	for (unsigned long int row_ptr = 0; row_ptr < _width*5; row_ptr += _width) {
		for (unsigned long int col_ptr = row_ptr; col_ptr < row_ptr + 5; col_ptr++) {
			std::cout << (unsigned)buffer_8bits[col_ptr] << " ";
		}
		std::cout << std::endl;
	}
}

//for writing grayscale red, green1, green2 and bue channels
void PPM::write_pgm(const char* filename, Channel channel) {
	switch (channel) {
		case RED:
			if (_width == 0 || _height == 0) {
				fprintf(stderr, "Can't save an empty image\n");
			}

			try {
				ofs.open(filename, std::ios::binary);
				if (ofs.fail()) {
					throw("Can't open output file");
				}

				red_channel_8bits = new uint8_t[(unsigned)(_total_pixels / 4)];
				//convert 12bpc to 8bpc
				std::cout << "Started Converting 12bpc to 8bpc\n";
				for (unsigned long int index = 0; index < (_total_pixels / 4); index++) {
					red_channel_8bits[index] = (red_channel_12bits[index]) >> 4;
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
			break;

		case GREEN1:
			if (_width == 0 || _height == 0) {
				fprintf(stderr, "Can't save an empty image\n");
			}

			try {
				ofs.open(filename, std::ios::binary);
				if (ofs.fail()) {
					throw("Can't open output file");
				}

				green1_channel_8bits = new uint8_t[(unsigned)(_total_pixels / 4)];
				//convert 12bpc to 8bpc
				std::cout << "Started Converting 12bpc to 8bpc\n";
				for (unsigned long int index = 0; index < (_total_pixels / 4); index++) {
					green1_channel_8bits[index] = (green1_channel_12bits[index]) >> 4;
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
			break;

		case GREEN2:
			if (_width == 0 || _height == 0) {
				fprintf(stderr, "Can't save an empty image\n");
			}

			try {
				ofs.open(filename, std::ios::binary);
				if (ofs.fail()) {
					throw("Can't open output file");
				}

				green2_channel_8bits = new uint8_t[(unsigned)(_total_pixels / 4)];
				//convert 12bpc to 8bpc
				std::cout << "Started Converting 12bpc to 8bpc\n";
				for (unsigned long int index = 0; index < (_total_pixels / 4); index++) {
					green2_channel_8bits[index] = (green2_channel_12bits[index]) >> 4;
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
			break;

		case BLUE:
			if (_width == 0 || _height == 0) {
				fprintf(stderr, "Can't save an empty image\n");
			}

			try {
				ofs.open(filename, std::ios::binary);
				if (ofs.fail()) {
					throw("Can't open output file");
				}

				green2_channel_8bits = new uint8_t[(unsigned)(_total_pixels / 4)];
				//convert 12bpc to 8bpc
				std::cout << "Started Converting 12bpc to 8bpc\n";
				for (unsigned long int index = 0; index < (_total_pixels / 4); index++) {
					green2_channel_8bits[index] = (blue_channel_12bits[index]) >> 4;
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
			break;
		default:
			break;
	} //end of switch
}

//for writing debayered colored image
void PPM::write_ppm(const char* filename) {
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
