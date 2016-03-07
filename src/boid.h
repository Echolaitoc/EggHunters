#pragma once

// Based on paper.js Tadpole example: http://paperjs.org/examples/tadpoles/

#include "ofMain.h"

class boid
{
public:
	boid(ofColor color, ofVec2f position, float maxSpeed, float maxForce, bool groupTogether);
	~boid();

	bool operator==(const boid& other);

    ofVec2f getPosition();
	void addOtherBoid(boid* otherBoid);
	void update();
	void draw();
    void setGrouping(bool groupTogether);
    void setFollowEgg(bool followEgg);
    void setEggPosition(ofVec2f eggPosition);
    void seek(ofVec2f target);
    void seek(ofVec2f target, float eggRadius);
    void arrive(ofVec2f target);

private:
	void calculateTail();
	void createItems();
	void flock();
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
	ofVec2f acceleration;
	ofVec2f directionVector;
	ofVec2f position;
	ofVec2f lastLoc;
    ofVec2f eggPosition;
	float radius;
	float maxSpeed;
	float maxForce;
	float amount;
	float count;
	bool groupTogether;
    bool followEgg;
};

