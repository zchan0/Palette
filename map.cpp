#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <iostream>
#include <vector>
#include <cmath>

#include "ImageIO/ImageIO.h"
#include "utils.h"

/** Special vars */

static const int HSV = 3;
static const double PI = 3.14159265;
static const unsigned char ESC = 27;

/** ImageIO handlers */

static int inW, inH, outW, outH;
static double *hsvPixmap;
static unsigned char *outPixmap;
static std::string input, output;

static ImageIO ioOrigin = ImageIO();
static ImageIO ioMapped = ImageIO();

void initHSVPixmap(int w, int h, const unsigned char *pixmap)
{
	hsvPixmap = new double[HSV * w * h];
	double hh, s, v;
	int pos;
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			pos = i * w + j;
			Utils::getInstance().RGBtoHSV(pixmap[pos * RGBA + R], pixmap[pos * RGBA + G], pixmap[pos * RGBA + B], hh, s, v);
			hsvPixmap[pos * HSV + 0] = std::ceil(hh);
			hsvPixmap[pos * HSV + 1] = s;
			hsvPixmap[pos * HSV + 2] = v;	
		}
	}
}

/** To draw on screen, outpixmap ALWAYS uses RGBA */

void setupOutPixmap(int w, int h) 
{
	outPixmap = new unsigned char[RGBA * w * h];
	for (int i = 0; i < h; ++i) 
		for (int j = 0; j < w; ++j) 
			for (int channel = 0; channel < RGBA; ++channel)
				/** Init alpha channel to 0, make no color value pixel to be transparent */
				outPixmap[(i * w + j) * RGBA + channel] = 0;
}

/** Calculate output image size */

void setOutputSize(int &w, int &h)
{
	int maxX, maxY = 0;
	int *xs = new int[2]; // x coordinates
	int *ys = new int[2]; // y coordinates

	xs[0] = 0;	xs[1] = inW; 
	ys[0] = 0;	ys[1] = inH;

	for (int i = 0; i < 2; ++i) {
	 	for (int j = 0; j < 2; ++j) {
	 		// maxX = fmax(maxX, U(xs[i], ys[j]));
	 		// maxY = fmax(maxY, V(xs[i], ys[j]));
	 	}
	}

	w = maxX;
	h = maxY;
	setupOutPixmap(w, h);
}

bool parseCommandLine(int argc, char* argv[]) 
{
  switch (argc) {
  case 2: case 3:
  	input  = argv[1];
    output = argv[2] != NULL ? argv[2] : "output.png";
    return true; break;

  default:
  	std::cerr << "Usage: warp inimage.png [outimage.png]" << std:: endl;
    exit(1);
  	return false; break;
  }
}

void loadImage()
{
	ioOrigin.loadFile(input);

	inW = ioOrigin.getWidth();
	inH = ioOrigin.getHeight();

	// inverseMap(inW, inH, ioOrigin.pixmap);
	ioMapped.setPixmap(outW, outH, outPixmap);
}

void displayOriginWindow() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ioOrigin.draw();
}

void displayWarpedWindow()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ioMapped.draw();
}

void handleReshape(int width, int height) 
{
	glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);	
}

void handleKeyboard(unsigned char key, int x, int y) 
{
	switch(key) {
		case 'w': case 'W': 
			ioMapped.saveImage(output); break;
  	case 'q': case 'Q': case ESC: 
  		exit(0); break;
  }	
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

  if (parseCommandLine(argc, argv)) {
		loadImage();	
	}

	initHSVPixmap(inW, inH, ioOrigin.pixmap);

	// Origin image window
	glutInitWindowSize(inW, inH);
	glutCreateWindow("Original Image");
	glutDisplayFunc(displayOriginWindow);
	glutKeyboardFunc(handleKeyboard);
	glutReshapeFunc(handleReshape);

	// Warped image window
	glutInitWindowSize(outW, outH);
	glutCreateWindow("Warped Image");
	glutDisplayFunc(displayWarpedWindow);
	glutKeyboardFunc(handleKeyboard);
	glutReshapeFunc(handleReshape);

	glutMainLoop();
	
	return 0;
}
