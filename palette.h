#define HSV 3

struct HSVPixel
{
	double h, s, v;

	bool operator<(const HSVPixel &rhs) const {
		return v < rhs.v;
	}
};

class Palette
{
	public:
		Palette(const std::string name);
		~Palette();

		int size() const;	// total number of colors
		std::vector<HSVPixel> getPalette() const;

		void sort();
		void print() const;

	private:
		std::string filename;	// palette file name
		std::vector<HSVPixel> palette;

		// for display on screen
		int width, height;	
		unsigned char *pixmap;	

		void loadPalette();
};