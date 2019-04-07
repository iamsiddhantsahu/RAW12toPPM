#include <iostream>
#include <fstream>

#include "RAW12PPM/RAW12.h"
#include "RAW12PPM/PPM.h"
#include "Debayer/Debayer.h"

int main() {

	RAW12 raw12("../data/input.raw12", 4096, 3072);
	std::cout << "Loaded RAW12 image into 8 bits buffer\n";

	raw12.buffer_8bits_to_12bits();
	std::cout << "Stored and alligned into a 12 bits buffer\n";

	raw12.extract_channels();
	std::cout << "Separated channels into red, green1, green2, blue\n";

	Debayer debayer(raw12);
	std::cout << "Debayered the RAW12 image\n";

	debayer.debayer_RAW12(NEARESTNEIGHBOUR);

	PPM ppm(debayer, raw12);
	ppm.print_5X5();
	ppm.write_pgm("../result/red.pgm", RED);
	ppm.write_pgm("../result/green1.pgm", GREEN1);
	ppm.write_pgm("../result/green2.pgm", GREEN2);
	ppm.write_pgm("../result/blue.pgm", BLUE);

	ppm.write_ppm("../result/colored_neigh_test.ppm");


	return 0;
}
