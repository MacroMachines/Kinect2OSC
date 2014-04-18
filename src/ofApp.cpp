#include "ofApp.h"

void ofApp::setup() {
	kinect.setRegistration(true);
	kinect.init();
	kinect.open();
	
	if (kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
	else {
		ofLogError() << "kinect not connected!";
	}
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
	
	nearThreshold = 255;
	farThreshold = 240;
	
	ofSetFrameRate(60);
	
	angle = 0;
	kinect.setCameraTiltAngle(angle);
}

void ofApp::update() {
	kinect.update();

	if (kinect.isFrameNew()) {
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		grayThreshNear = grayImage;
		grayThreshFar = grayImage;
		
		grayThreshNear.threshold(nearThreshold, true);
		grayThreshFar.threshold(farThreshold);
		
		cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		
		// update the cv images
		grayImage.flagImageChanged();
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayImage, 10, (kinect.width * kinect.height) / 2, 20, false);
		
		// get data from contours
		
		vector<ofxCvBlob> blobs = contourFinder.blobs;
		vector<ofxCvBlob>::iterator blobIt;
		for (blobIt = blobs.begin(); blobIt != blobs.end(); blobIt++) {
			ofLogNotice() << "centr: " << blobIt->centroid << " boundingrect: " << blobIt->boundingRect;
		}
	}
}

void ofApp::draw() {
	ofBackground(70, 70, 70);
	ofSetColor(255, 255, 255);

	kinect.draw(10, 10, 400, 300);
	
	kinect.drawDepth(420, 10, 400, 300);
	
	grayImage.draw(830, 10, 400, 300);
	contourFinder.draw(830, 10, 400, 300);
}

void ofApp::exit() {
	kinect.setCameraTiltAngle(0);
	kinect.close();
}

void ofApp::keyPressed(int key) {

}

void ofApp::mousePressed(int x, int y, int button) {

}