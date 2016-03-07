#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(82, 37, 131);
    
    mousePosition.set(ofGetWidth() / 2, ofGetHeight() / 2);
    
    boidColor.set(175, 203, 39);
    eggRadius = 100;
    eggCoreRadius = 20;
    eggRadiusMargin = 250;
    circlingSpeed = 0.03;
    egg.ellipse(mousePosition, eggRadius, eggRadius);
    egg.setCircleResolution(40);
    egg.setColor(ofColor(255, 204, 0));
    eggCore.ellipse(mousePosition, eggCoreRadius, eggCoreRadius);
    eggCore.setCircleResolution(30);
    eggCore.setColor(boidColor);
    followEgg = false;
    amount = 30;
	for (int i = 0; i < amount; ++i) {
		auto position = ofPoint(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
		this->boids.push_back(boid(boidColor, position, 10, 0.05, followEgg));
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
    for (int i = 0; i < boids.size(); ++i) {
        boid& boid = boids.at(i);
        if (followEgg) {
            boid.setEggPosition(mousePosition);
            int sign = i < amount / 2 ? 1 : -1;
            ofVec2f boidPosition = ofVec2f(sin((ofGetFrameNum() * sign + i * 300) * circlingSpeed), cos((ofGetFrameNum() * sign + i * 300) * circlingSpeed));
            boidPosition = mousePosition + boidPosition.getScaled(eggRadius + eggRadiusMargin + i * 5 + (i / amount) * 20);
            boid.seek(boidPosition);
        }
        boid.update();
    }
    
    if (followEgg) {
        egg.clear();
        egg.ellipse(mousePosition, eggRadius, eggRadius);
        eggCore.clear();
        eggCore.ellipse(mousePosition, eggCoreRadius, eggCoreRadius);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(boidColor);
	for (auto boid : boids) {
		boid.draw();
	}
    if (followEgg) {
        egg.draw();
        eggCore.draw();
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
    mousePosition.set(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    followEgg = !followEgg;
    for (auto& boid : boids) {
        boid.setFollowEgg(followEgg);
    }
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