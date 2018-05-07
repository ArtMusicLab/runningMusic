#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// we don't want to be running to fast
	ofSetVerticalSync(true);
	//	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	// print the available output ports to the console
	midiOut.listPorts(); // via instance
	//ofxMidiOut::listPorts(); // via static too
	
	// connect
	midiOut.openPort(0); // by number
	//midiOut.openPort("IAC Driver Pure Data In"); // by name
	//midiOut.openVirtualPort("ofxMidiOut"); // open a virtual port
	
	
	
	
	channel = 1;
	currentPgm = 0;
	note = 0;
	velocity = 0;
	pan = 0;
	bend = 0;
	touch = 0;
	polytouch = 0;
	
	tracks = ofGetHeight() / 4;
	octave = 2;
	
	gui.setup("settings");
	gui.add(bShowScanData.set("show scan data",true));
	gui.add(scanRadius.set("scan radius",1,1,100));
	gui.add(rotate.set("rotate",0,1,360));
	gui.add(filterLeft.set("filter left",0,1,5000));
	gui.add(filterRight.set("filter right",0,1,5000));
	gui.add(filterTop.set("filter top",0,1,5000));
	gui.add(filterBottom.set("filter bottom",0,1,5000));
	gui.add(moveUp.set("up",0,1,1000));
	gui.add(moveDown.set("down",0,1,1000));
	gui.add(moveLeft.set("left",0,1,1000));
	gui.add(moveRight.set("right",0,1,1000));
	
	// seva setting with give name
	if (!ofFile("settings.xml"))
		gui.saveToFile("settings.xml");
	
	gui.loadFromFile("settings.xml");
	
	centroidX = 0.0;
	centroidY = 0.0;
	accuOfPosData = 0;
	
	auto sensor_list = ofxRPlidar::getDeviceList();
	for(auto &sensor_info : sensor_list) {
		auto sensor = make_shared<ofxRPlidar>();
		if(sensor->connect(sensor_info.getDevicePath())) {
			sensor->start();
			sensors_.push_back(sensor);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	
	gui.draw();
	
	
	for(auto &s : sensors_) {
		
		// for get centroid position of each lidar
		accuOfPosData = 0;
		centroidX = 0.0;
		centroidY = 0.0;
		
		
		auto data = s->scan();
		ofPushMatrix();
		ofSetColor(255,0,0);
		//		ofTranslate(ofVec2f(ofGetWidth(), ofGetHeight())/2.f);

		for(auto &d : data) {
			if(d.quality > 0) {
				ofVec2f pos = ofVec2f(d.distance / scanRadius.get(), 0).getRotated(d.angle + rotate.get());
				pos.x += moveRight.get();
				pos.x -= moveLeft.get();
				pos.y += moveUp.get();
				pos.y -= moveDown.get();
				if(pos.x > filterLeft.get()
				   & pos.x < filterRight.get()
				   & pos.y > filterTop.get() * -1
				   &pos.y < filterBottom.get()
				   )
					
					
				{
					
					
					if(bShowScanData.get()){
						if(pos.x > 0 & pos.x < ofGetWidth() & pos.y > 0 & pos.y < ofGetHeight()){
							// draw all pos of dots ------
//							ofDrawCircle(pos, 5);
//							usePosition(pos.x,pos.y);
							
							accuOfPosData += 1;
							centroidX += pos.x;
							centroidY += pos.y;

						}
						
						
						
						string message = "[" + ofToString(int(pos.x)) + "," + ofToString(int(pos.y)) + "]";
						
						ofDrawBitmapString(message, 100, 100);
						
						
					}
					
				}
				
			}
		}
		
		// draw centroid dot
		centroidX = centroidX / accuOfPosData;
		centroidY = centroidY / accuOfPosData;
		ofDrawCircle(ofVec2f(centroidX,centroidY), 5);
		usePosition(centroidX,centroidY);

//		channel = int(centroidY / 192);
//		note = int(centroidX * 127 / ofGetWidth());
//		velocity = 1;
//		if(channel >= 0 & note >= 0){
//			midiOut.sendNoteOn(channel + 1, note,  velocity);
//
//			cout << centroidX << " " << centroidY << " "  << channel + 1 << " " << note << endl;
//
//
//		}
//
		ofPopMatrix();
		
	}
}
//--------------------------------------------------------------
void ofApp::exit(){
	
	midiOut.closePort();

	
	for(auto &s : sensors_) {
		s->stop();
		s->disconnect();
	}
}

void ofApp::usePosition(float _x,float _y){
	// debug -------------
//	cout << _x << " " << _y << endl;
	
	// make midi message to send =====================
	// send a note on if the key is a letter or a number
	
//	channel = int(_y / 192);
	channel = 1;
	note = int(_x * 127 / ofGetWidth());
	velocity = 120;
	if(channel >= 0 & note >= 0){
		midiOut.sendNoteOn(channel, note,  velocity);
		
//		cout << _x << " " << _y << " "  << channel << " " << note << endl;
		midiOut.sendNoteOff(channel, note);

	}
	
	
	
	// print out both the midi note and the frequency
//	ofLogNotice() << "note: " << note;
//	<< " freq: " << ofxMidi::mtof(note) << " Hz";
	
	
//
//	if(false) {
//
//		// scale the ascii values to midi velocity range 0-127
//		// see an ascii table: http://www.asciitable.com/
//		int posX = 0;
//		int posY = 0;
//		note = ofMap(posX, 48, 122, 0, 127);
//		velocity = 10;
//		midiOut.sendNoteOn(channel, note,  velocity);
//
//		// print out both the midi note and the frequency
//		ofLogNotice() << "note: " << note
//		<< " freq: " << ofxMidi::mtof(note) << " Hz";
//	}
//
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case OF_KEY_LEFT:
			rotate.set(rotate.get() - 1);
			break;
		case OF_KEY_RIGHT:
			rotate.set(rotate.get() + 1);
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	
}
