#pragma once

#include  "ofxAssimpModelLoader.h"
#include "ofMain.h"

// This extends ofxAssimpModelLoader, so it should have all of the same methods to start
// this makes the implementation much easier (nothing to change in ofApp.cpp)
// All I'm adding is the physics integrator, which will allow us to give the lander some thrust and let gravity work on it

class Lander : public ofxAssimpModelLoader {
public:
	//Lander(){}; //no instance of lander, only used to do work on the lander

	ofVec3f position = ofVec3f(1, 1, 0);
	ofVec3f velocity = ofVec3f(0, 0, 0);
	ofVec3f acceleration = ofVec3f(0, 0, 0);
	ofVec3f forces = ofVec3f(0, 0, 0);

	ofVec3f gravity = ofVec3f(0, 0, 0);
	float	damping = 0.99;
	float   mass = 1;
	
	void    integrate();
	void	setGravity(ofVec3f g);
	void	applyGravity();
	void	applyThrust(ofVec3f t);
};
