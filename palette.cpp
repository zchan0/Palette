#include <fstream>
#include <vector>
#include <iostream>

#include "palette.h"
#include "ImageIO/Image.h"

static const std::string path = "palettes/";

Palette::~Palette()
{
		if (pixmap)
			delete []pixmap;
}

Palette::Palette(const std::string name): filename(path + name)
{
	palette.reserve(3);
	loadPalette();

	width  = 50;
	height = 30;
	pixmap = new unsigned char[RGBA * width * height];
}

void Palette::loadPalette()
{
	std::ifstream file(filename);
	std::string str;
	std::vector<double> v;

	if (file.is_open()) {
		while(!file.eof()) {
			str = " ";
			file >> str;
			if (str != " ") {
				v.push_back(stod(str));
			}
		} 
	} else {
		std::cerr << "Cannot open " << filename << std::endl;
		exit(0);
	}

	file.close();

	if (v.size() % HSV != 0) {
		std::cerr << "Palette seems to be wrong" << std::endl;
		exit(0);
	}

	for (int i = 0; i < v.size() / HSV; ++i) {
		HSVPixel hsv;
		hsv.h = v[i * HSV + 0];
		hsv.s = v[i * HSV + 1];
		hsv.v = v[i * HSV + 2];

		palette.push_back(hsv);
	}
}

std::vector<HSVPixel> Palette::getPalette() const
{
	return palette;
}

int Palette::size() const 
{
	return palette.size();
}

void Palette::print() const
{
	std::cout << "total color: " << size() << std::endl;
	std::vector<HSVPixel>::const_iterator ptr = palette.begin();
	while(ptr != palette.end()) {
		std::cout << (*ptr).h << " " << (*ptr).s << " " << (*ptr).v << std::endl;
		++ptr;
	}
}
