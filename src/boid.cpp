#include "boid.h"

boid::boid(ofColor color, ofVec2f position, float maxSpeed, float maxForce, bool groupTogether)
	: color(color)
	, acceleration(0)
	, directionVector(0)
	, position(position)
	, lastLoc(position)
    , eggPosition(0, 0)
	, radius(30)
	, maxSpeed(maxSpeed)
	, maxForce(maxForce)
	, amount(0)
	, count(0)
	, groupTogether(groupTogether)
    , followEgg(false)
{
	directionVector = ofVec2f(ofRandom(1.0f), ofRandom(1.0f)) * 2 - 1;
	auto strength = ofRandom(0.5f);
	this->maxSpeed = maxSpeed + strength;
	this->maxForce = maxForce + strength;
	this->amount = strength * 10 + 10;

	for (int i = 0; i < amount; ++i) {
		this->segments.push_back(ofVec2f(0));
		if (i < 5) {
			this->shortSegments.push_back(ofVec2f(0));
		}
	}

	head.setFillColor(color);
	tail.setFilled(false);
	tail.setStrokeWidth(2);
	tail.setColor(color);
	shortTail.setFilled(false);
	shortTail.setStrokeWidth(4);
	shortTail.setColor(color);

	calculateTail();
	createItems();
}

boid::~boid()
{
	otherBoids.clear();
}

bool boid::operator==(const boid& other) {
	if (acceleration == other.acceleration
		&& directionVector == other.directionVector
		&& position == other.position) {
		return true;
	}
	return false;
}

ofVec2f boid::getPosition() {
    return position;
}

void boid::addOtherBoid(boid* otherBoid) {
	otherBoids.push_back(otherBoid);
}

void boid::update() {
	lastLoc = position;
	if (!groupTogether) {
		flock();
	}
	else {
		align();
	}
	borders();

	directionVector += acceleration;
	directionVector.scale(MIN(maxSpeed, directionVector.length()));
	position += directionVector;
	acceleration = ofVec2f();

	calculateTail();
	createItems();
}

void boid::draw() {
	head.draw();
	tail.draw();
	shortTail.draw();
}

void boid::setGrouping(bool groupTogether) {
    this->groupTogether = groupTogether;
}

void boid::setFollowEgg(bool followEgg) {
    this->followEgg = followEgg;
}

void boid::setEggPosition(ofVec2f eggPosition) {
    this->eggPosition = eggPosition;
}

void boid::calculateTail() {
	auto speed = directionVector.length();
	auto pieceLength = 5 + speed / 3;
	segments.at(0) = position;
	shortSegments.at(0) = position;
	auto segmentPosition = position;
	// Chain goes the other way than the movement
	auto lastVector = -directionVector;
	for (int i = 1; i < amount; ++i) {
		auto segmentVector = segments.at(i) - segmentPosition;
		count += speed * 10;
		auto wave = sin((count + i * 3) / 300);
        auto sway = lastVector.getRotated(90).getNormalized().getScaled(wave);
		segmentPosition += lastVector.getNormalized().getScaled(pieceLength) + sway;
		segments.at(i) = segmentPosition;
		if (i < 5) {
			shortSegments.at(i) = segmentPosition;
		}
		lastVector = segmentVector;
	}
}

void boid::createItems() {
	head.clear();
	head.ellipse(ofVec2f(0, 0), 12, 8);
    head.setMode(ofPath::POLYLINES);
	head.rotate(directionVector.angle(ofVec2f(1, 0)), ofVec3f(0, 0, -1));
	head.translate(position);

	tail.clear();
	for (int i = 0; i < segments.size() - 1; ++i) {
		if (i == 0) {
			tail.moveTo(segments.at(i));
		}
		tail.curveTo(segments.at(i + 1));
	}

	shortTail.clear();
	for (int i = 0; i < shortSegments.size() - 1; ++i) {
		if (i == 0) {
			shortTail.moveTo(shortSegments.at(i));
		}
		shortTail.curveTo(shortSegments.at(i + 1));
	}
}

void boid::flock() {
	acceleration += separate() * 3 + align() + cohesion();
}

void boid::seek(ofVec2f target) {
    acceleration += steer(target, false);
}

void boid::seek(ofVec2f target, float eggRadius) {
    ofVec2f eggTarget = target + (position - target).getNormalized().getScaled(eggRadius);
    acceleration += steer(eggTarget, false);
}

void boid::arrive(ofVec2f target) {
	acceleration += steer(target, true);
}

void boid::borders() {
	auto vector = ofVec2f();
	ofRectangle size = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
	if (position.x < -radius) vector.x = size.width + radius;
	if (position.y < -radius) vector.y = size.height + radius;
	if (position.x > size.width + radius) vector.x = -size.width - radius;
	if (position.y > size.height + radius) vector.y = -size.height - radius;
	if (vector != ofVec2f::zero()) {
		position += vector;
		for (int i = 0; i < amount; i++) {
			segments.at(i) += vector;
		}
	}
}

ofVec2f boid::steer(ofVec2f target, bool slowdown) {
	auto steer = target - position;
	auto desired = steer;

	auto distance = desired.length();
	// Two options for desired vector magnitude
	// (1 -- based on distance, 2 -- maxSpeed)
	if (slowdown && distance < 100) {
		// This damping is somewhat arbitrary:
		desired.scale(maxSpeed * (distance / 100));
	}
	else {
		desired.scale(maxSpeed);
	}
	steer = desired - directionVector;
	steer.normalize().scale(MIN(maxForce, steer.length()));
	return steer;
}

ofVec2f boid::separate() {
	auto desiredSeperation = 60;
	auto steer = ofVec2f();
	auto count = 0;
	// For every boid in the system, check if it's too close
	for (int i = 0; i < otherBoids.size(); ++i) {
		auto other = *otherBoids.at(i);
		auto vector = position - other.position;
		auto distance = vector.length();
		if (distance > 0 && distance < desiredSeperation) {
			// Calculate vector pointing away from neighbor
			steer += vector.normalize().scale(1 / distance);
			count++;
		}
	}
    desiredSeperation = 170;
    if (followEgg) {
        auto eggVector = position - eggPosition;
        auto eggDistane = eggVector.length();
        if (eggDistane > 0 && eggDistane < desiredSeperation) {
            // Calculate vector pointing away from egg
            steer += eggVector.normalize().scale(1 / eggDistane);
            count++;
        }
    }
	// Average -- divide by how many
	if (count > 0) {
		steer /= count;
	}
	if (steer != ofVec2f::zero()) {
		// Implement Reynolds: Steering = Desired - Velocity
		steer.normalize().scale(maxSpeed);
		steer -= directionVector;
		steer.normalize().scale(MIN(steer.length(), maxForce));
	}
	return steer;
}

ofVec2f boid::align() {
	auto neighborDist = pow(25, 2);
	auto steer = ofVec2f();
	auto count = 0;
	for (int i = 0; i < otherBoids.size(); ++i) {
		auto other = *otherBoids.at(i);
		auto distance = position.squareDistance(other.position);
		if (distance > 0 && distance < neighborDist) {
			steer += other.directionVector;
			count++;
		}
	}

	if (count > 0) {
		steer /= count;
	}
	if (steer != ofVec2f::zero()) {
		// Implement Reynolds: Steering = Desired - Velocity
		steer.normalize().scale(maxSpeed);
		steer -= directionVector;
		steer.normalize().scale(MIN(steer.length(), maxForce));
	}
	return steer;
}

ofVec2f boid::cohesion() {
	auto neighborDist = pow(100, 2);
	auto sum = ofVec2f();
	auto count = 0;
	for (int i = 0; i < otherBoids.size(); ++i) {
		auto other = *otherBoids.at(i);
		auto distance = position.squareDistance(other.position);
		if (distance > 0 && distance < neighborDist) {
			sum += other.position; // Add location
			count++;
		}
	}
    if (followEgg) {
        auto eggDistance = position.squareDistance(eggPosition);
        if (eggDistance > 0 && eggDistance < neighborDist) {
            sum += eggPosition; // Add location
            count++;
        }
    }
	if (count > 0) {
		sum /= count;
		// Steer towards the location
		return steer(sum, false);
	}
	return sum;
}

