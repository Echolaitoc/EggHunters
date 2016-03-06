#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	for (int i = 0; i < 20; ++i) {
		auto position = ofPoint(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
		this->boids.push_back(boid(ofColor(255), position, 10, 0.05, false));
	}

	for (auto& boid : boids) {
		for (auto& otherBoid : boids) {
			if (boid == otherBoid) {
				continue;
			}
			boid.addOtherBoid(&otherBoid);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	for (auto& boid : boids) {
		boid.update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	for (auto boid : boids) {
		boid.draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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