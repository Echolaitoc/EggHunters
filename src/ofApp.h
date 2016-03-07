#pragma once

#include "ofMain.h"
#include "boid.h"

class ofApp : public ofBaseApp {
private:
	vector<boid> boids;
    
    ofPath egg;
    ofColor boidColor;
    ofVec2f mousePosition;
    float eggRadius;
    float eggRadiusMargin;
    float circlingSpeed;
    int amount;
    bool followEgg;

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
};
