#define HSV 3

typedef struct 
{
	double h, s, v;
} HSVPixel;

class Palette
{
	public:
		Palette(const std::string name);
		~Palette();

		int size() const;	// total number of colors
		std::vector<HSVPixel> getPalette() const;

		void print() const;
	
	private:
		std::string filename;	// palette file name
		std::vector<HSVPixel> palette;

		// for display on screen
		int width, height;	
		unsigned char *pixmap;	

		void loadPalette();
};