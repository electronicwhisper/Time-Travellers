#include "testApp.h"




//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(60);
	//ofSetVerticalSync(true);
	ofHideCursor();
	
	ofBackground(0, 0, 0);
	
	width = 1280;
	height = 720;
	numPixels = width * height;
	
	currentMovie = 0;
	
	status = 0;
	
	showInfo = true;
	
	refreshInterval = 30 * 60 * 1000;
	nextRefresh = refreshInterval;
	
	alphaBlend = 0.7;
	blur = 4;
	
	
	//kinect.init(true);  //shows infrared image
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();
	
	clippingNear = 60;
	clippingFar = 230;
	camTilt = 0;
	
	kinect.setCameraTiltAngle(camTilt);
	kinect.getCalibration().setClippingInCentimeters(clippingNear, clippingFar);
	
	
	kinectImage.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayImage.setROI(16, 60, 640, 360); // Should be 0 but weird black pixels on left side of kinect depth image...
	kinectImage.setROI(16, 60, 640, 360);
	grayImageScaled.allocate(width, height);
	
	dest.allocate(width, height, OF_IMAGE_COLOR);
	
	DIR.setVerbose(false);
    numMovies = DIR.listDir("movies");
	
	
	frames = new unsigned char[256*numPixels*3];
		
	

}



//--------------------------------------------------------------
void testApp::update() {
	
	kinect.update();
	if(kinect.isFrameNew())	// there is a new frame and we are connected
	{
		
		// use this instead of the following for temporal blending
		// kinectImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		// cvAddWeighted(kinectImage.getCvImage(), alphaBlend, grayImage.getCvImage(), (1-alphaBlend), 0, grayImage.getCvImage());
		// grayImage.flagImageChanged();
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		
		grayImageScaled.scaleIntoMe(grayImage, CV_INTER_NN);
		
		// uncomment for blur control
		// grayImageScaled.blurGaussian(blur);
		
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofSetColor(255, 255, 255);
	
	if (status == 0) {
		dest.draw(0, 0);
		
		ofEnableAlphaBlending();
		ofSetColor(0, 0, 0, 200);
		ofRect(0, 0, width, height);
		ofDisableAlphaBlending();
		
		ofSetColor(255, 255, 255);
		stringstream reportStream;
		reportStream << "Loading\n" << DIR.getName(currentMovie);
		ofDrawBitmapString(reportStream.str(), width / 2, height / 2);
		status++;
	} else if (status == 1) {
		ofVideoPlayer video;
		video.loadMovie(DIR.getPath(currentMovie));
		
		float nFrames = video.nFrames;
		unsigned char* pixels;
		for (int i = 0; i < 256; i++) {
			video.setFrame(nFrames * (float) i / 256.0 + 3);
			pixels = video.getPixels();
			memcpy(frames + i*numPixels*3, pixels, numPixels*3);
		}
		
		video.closeMovie();
		
		status++;
	} else if (status == 2) {
		unsigned char* destPixels = dest.getPixels();
		unsigned char* grayPixels = grayImageScaled.getPixels();
		
		for (int i = 0; i < numPixels; i++) {
			int depth = grayPixels[i];
			int destOffset = i*3;
			int framesOffset = depth*numPixels*3 + destOffset;
			
			destPixels[destOffset] = frames[framesOffset];
			destPixels[destOffset+1] = frames[framesOffset+1];
			destPixels[destOffset+2] = frames[framesOffset+2];
		}
		
		dest.update();
		
		dest.draw(0, 0);
		
		// refresh if elapsed time long enough
		if (ofGetElapsedTimeMillis() > nextRefresh) {
			nextRefresh = ofGetElapsedTimeMillis() + refreshInterval;
			currentMovie = (currentMovie + 1) % numMovies;
			status = 0;
		}
		
	}
	
	if (showInfo) {
		stringstream reportStream;
		reportStream
		<< "toggle this information display (press: ?)" << endl
		<< endl
		<< "next video (press: spacebar)" << endl
		<< "toggle fullscreen (press: f)" << endl
		<< "save a snapshot (press: s)" << endl
		<< endl
		<< "set near threshold " << clippingNear << " (press: + -)" << endl
		<< "set far threshold " << clippingFar << " (press: < >)" << endl
		<< "set camera tilt " << camTilt << " (press: DOWN UP)" << endl
		<< endl
		// << "set alpha blend " << alphaBlend << " (press: a A)" << endl
		// << "set blur " << blur << " (press: b B)" << endl
		<< endl
		<< "next auto-refresh in: " << (nextRefresh-ofGetElapsedTimeMillis())/1000 << endl
		<< "fps: " << ofGetFrameRate() << endl;
		ofDrawBitmapString(reportStream.str(),20,500);
	}
}


//--------------------------------------------------------------
void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	switch (key) {
		case ' ':
			nextRefresh = ofGetElapsedTimeMillis() + refreshInterval;
			currentMovie = (currentMovie + 1) % numMovies;
			status = 0;
			break;
			
			
		case '/':
		case '?':
			showInfo = !showInfo;
			break;
		
		case 'f':
			ofToggleFullscreen();
			break;
			
		case 's':
			static char fileName[255];
			sprintf(fileName, "snapshots/%i-%i-%i-%i-%i-%i.png", ofGetYear(), ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds());
			dest.saveImage(fileName);
			break;
		
		case 'a':
			alphaBlend -= 0.05;
			break;
		case 'A':
			alphaBlend += 0.05;
			break;
		
		case 'b':
			blur -= 1;
			break;
		case 'B':
			blur += 1;
			break;
			
		case '>':
		case '.':
			clippingFar++;
			kinect.getCalibration().setClippingInCentimeters(clippingNear, clippingFar);
			break;
		case '<':		
		case ',':		
			clippingFar--;
			kinect.getCalibration().setClippingInCentimeters(clippingNear, clippingFar);
			break;
			
		case '+':
		case '=':
			clippingNear++;
			kinect.getCalibration().setClippingInCentimeters(clippingNear, clippingFar);
			break;
		case '-':		
			clippingNear--;
			kinect.getCalibration().setClippingInCentimeters(clippingNear, clippingFar);
			break;
		
		case OF_KEY_DOWN:
			camTilt--;
			kinect.setCameraTiltAngle(camTilt);
			break;
		case OF_KEY_UP:
			camTilt++;
			kinect.setCameraTiltAngle(camTilt);
			break;
			
		default:
			break;
	}	
	
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

