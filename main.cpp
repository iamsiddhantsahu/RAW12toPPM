#include <iostream>
#include <fstream>

#include "RAW12.h"

int main() {

	RAW12 raw12("portrait-gainx2-offset2047-20ms-02.raw12", 4096, 3072);
	std::cout << "Loaded RAW12 image into 8 bits buffer\n";

	raw12.buffer_8bits_to_12bits();
	std::cout << "Stored and alligned into a 12 bits buffer\n";

	raw12.seperate_channels();
	std::cout << "Separated channels into red, green1, green2, blue\n";

	raw12.debayer();
	std::cout << "Debayered the RAW12 image\n";

	raw12.write_ppm("debayered.ppm");
	std::cout << "Wrote debayerred PPM image to disk\n";

	std::cin.get();
	return 0;
}