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

static int inW, inH;
static double *hsvPixmap;
static unsigned char *outPixmap;
static std::string input, output;
static double palette[] = {0, 0, 0, 0, 0, 0.5, 0, 0, 1};

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

void prepareDisplay()
{
	int pos;
	double r, g, b;
	
	setupOutPixmap(inW, inH);
	
	for (int i = 0; i < inH; ++i) {
		for (int j = 0; j < inW; ++j) {
			pos = i * inW + j;
			Utils::getInstance().HSVtoRGB(hsvPixmap[pos * HSV + 0], hsvPixmap[pos * HSV + 1], hsvPixmap[pos * HSV + 2], r, g, b);
			outPixmap[pos * RGBA + R] = (int)r * 255;
			outPixmap[pos * RGBA + G] = (int)g * 255;
			outPixmap[pos * RGBA + B] = (int)b * 255;
		}
	}
}

void process()
{
	double avgV = 0;
	for (int i = 0; i < inH; ++i) 
		for (int j = 0; j < inW; ++j) 
			avgV += hsvPixmap[(i * inW + j) * HSV + 2];
	avgV /= (inH * inW);
	
	std::cout << "avgV " << avgV << std::endl;

	int pos;
	double v;
	for (int i = 0; i < inH; ++i) {
		for (int j = 0; j < inW; ++j) {
			pos = i * inW + j;
			v = Utils::getInstance().round(hsvPixmap[pos * HSV + 2], 2);
			if (v < 0.5) {
				hsvPixmap[pos * HSV + 0] = palette[0 * HSV + 0];
				hsvPixmap[pos * HSV + 1] = palette[1 * HSV + 1];
				hsvPixmap[pos * HSV + 2] = palette[0 * HSV + 2];
			} else if (v >= 0.5 && v < avgV) {
				hsvPixmap[pos * HSV + 0] = palette[1 * HSV + 0];
				hsvPixmap[pos * HSV + 1] = palette[1 * HSV + 1];
				hsvPixmap[pos * HSV + 2] = palette[1 * HSV + 2];
			} else if (v >= avgV) {
				hsvPixmap[pos * HSV + 0] = palette[2 * HSV + 0];
				hsvPixmap[pos * HSV + 1] = palette[1 * HSV + 1];
				hsvPixmap[pos * HSV + 2] = palette[2 * HSV + 2];
			}
		}
	}

	prepareDisplay();
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
	process();
	ioMapped.setPixmap(inW, inH, outPixmap);

	// Origin image window
	glutInitWindowSize(inW, inH);
	glutCreateWindow("Original Image");
	glutDisplayFunc(displayOriginWindow);
	glutKeyboardFunc(handleKeyboard);
	glutReshapeFunc(handleReshape);

	// Warped image window
	glutInitWindowSize(inW, inH);
  glutInitWindowPosition(glutGet(GLUT_WINDOW_X) + inW, 0);
	glutCreateWindow("Warped Image");
	glutDisplayFunc(displayWarpedWindow);
	glutKeyboardFunc(handleKeyboard);
	glutReshapeFunc(handleReshape);

	glutMainLoop();
	
	return 0;
}
