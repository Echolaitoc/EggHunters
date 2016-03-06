#pragma once

// Based on paper.js Tadpole example: http://paperjs.org/examples/tadpoles/

#include "ofMain.h"

class boid
{
public:
	boid(ofColor color, ofVec2f position, float maxSpeed, float maxForce, bool groupTogether);
	~boid();

	bool operator==(const boid& other);

	void addOtherBoid(boid* otherBoid);
	void update();
	void draw();

private:
	void calculateTail();
	void createItems();
	void flock();
	void seek(ofVec2f target);
	void arrive(ofVec2f target);
	void borders();
	ofVec2f steer(ofVec2f target, bool slowdown);
	ofVec2f separate();
	ofVec2f align();
	ofVec2f cohesion();

	vector<boid*> otherBoids;
	vector<ofVec2f> segments;
	vector<ofVec2f> shortSegments;
	ofPath head;
	ofPath tail;
	ofPath shortTail;
	ofColor color;
	ofPoint acceleration;
	ofPoint directionVector;
	ofPoint position;
	ofPoint lastLoc;
	float radius;
	float maxSpeed;
	float maxForce;
	float amount;
	float count;
	bool groupTogether;
};

