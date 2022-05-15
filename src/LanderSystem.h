#pragma once

#include "ofMain.h"
#include  "ofxAssimpModelLoader.h"


//  Pure Virtual Function Class - must be subclassed to create new forces.
//
class LanderForce {
protected:
public:
	bool applyOnce = false;
	bool applied = false;
	virtual void updateForce(Particle*) = 0;
};

class LanderSystem {
public:
	void add(const ofxAssimpModelLoader&);
	void addForce(LanderForce*);
	void remove(int);
	void update();
	void setLifespan(float);
	void reset();
	int removeNear(const ofVec3f& point, float dist);
	void draw();
	vector<ofxAssimpModelLoader> particles; //TODO: This is when there's multiple to do work on?
	vector<LanderForce*> forces;
};



// Some convenient built-in forces
//
class GravityForce : public LanderForce {
	ofVec3f gravity;
public:
	void set(const ofVec3f& g) { gravity = g; }
	GravityForce(const ofVec3f& gravity);
	GravityForce() { gravity.set(0, -10, 0); }
	void updateForce(ofxAssimpModelLoader*);
};

class TurbulenceForce : public LanderForce {
	ofVec3f tmin, tmax;
public:
	void set(const ofVec3f& min, const ofVec3f& max) { tmin = min; tmax = max; }
	TurbulenceForce(const ofVec3f& min, const ofVec3f& max);
	TurbulenceForce() { tmin.set(0, 0, 0); tmax.set(0, 0, 0); }
	void updateForce(ofxAssimpModelLoader*);
};


class ThrusterForce : public LanderForce {
	ofVec3f thrust = ofVec3f(0, 0, 0);
public:
	void set(ofVec3f t) { thrust = t; }
	void add(ofVec3f t) { thrust += t; }
	ThrusterForce(ofVec3f t) { thrust = t; }
	ThrusterForce() {}
	void updateForce(ofxAssimpModelLoader*);
};
