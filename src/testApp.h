#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxDirList.h"

class testApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		void exit();
	
		void drawPointCloud();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		
	int width;
	int height;
	int numPixels;
	
	ofxKinect kinect;
	
	int clippingNear;
	int clippingFar;
	int camTilt;
	
	ofxCvGrayscaleImage kinectImage;
	ofxCvGrayscaleImage grayImage;
	ofxCvGrayscaleImage grayImageScaled;
	
	ofxDirList   DIR;
	int numMovies;
	
	int currentMovie;
	
	unsigned char * frames;
	
	unsigned char * * movies;
	
	ofImage dest;
	
	int status;
	// 0 - loading started
	// 1 - load in progress
	// 2 - active
	
	bool showInfo;
	
	float alphaBlend;
	int blur;
	
	int refreshInterval;
	int nextRefresh;
};
