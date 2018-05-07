#pragma once

#include "ofMain.h"
#include "ofxRPlidar.h"
#include "ofxGui.h"
#include "ofxMidi.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	void usePosition(float _x,float _y);
	ofxMidiOut 	midiOut;
	unsigned int currentPgm;
	
	int 		channel;
	int 		note, velocity;
	int 		pan, bend, touch, polytouch;
	
	int			tracks;
	int			octave;
	ofxMidiMessage message;
	std::vector<std::shared_ptr<ofxRPlidar>> sensors_;
	//	std::shared_ptr<ofxRPlidar>		lidar;
	
	
	ofxPanel			gui;
	ofParameter<bool>	bShowScanData;
	ofParameter<int>	scanRadius;
	ofParameter<int>	moveUp;
	ofParameter<int>	moveDown;
	ofParameter<int>	moveRight;
	ofParameter<int>	moveLeft;
	
	ofParameter<int>	rotate;
	ofParameter<int>	filterLeft;
	ofParameter<int>	filterRight;
	ofParameter<int>	filterTop;
	ofParameter<int>	filterBottom;
	
	float 				centroidX;
	float				centroidY;
	int					accuOfPosData;
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
