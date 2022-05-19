#pragma once

#include  "ofxAssimpModelLoader.h"
#include "ofMain.h"
#include "ParticleEmitter.h"
#include "ofxGui.h"

// This extends ofxAssimpModelLoader, so it should have all of the same methods to start
// this makes the implementation much easier (nothing to change in ofApp.cpp)
// All I'm adding is the physics integrator, which will allow us to give the lander some thrust and let gravity work on it

// Made by: Roy and Lauren
class Lander : public ofxAssimpModelLoader {
public:
	//Lander(){}; //no instance of lander, only used to do work on the lander

	ofVec3f position = ofVec3f(1, 2, 0);
	ofVec3f velocity = ofVec3f(0, 0, 0);
	ofVec3f acceleration = ofVec3f(0, 0, 0);
	ofVec3f forces = ofVec3f(0, 0, 0);

	ofVec3f gravity = ofVec3f(0, 0, 0);
	float	damping = 0.99;
	float   mass = 1;
	
	void    integrate();
	//void	setPosition(float x, float y, float z); 
	void	setGravity(ofVec3f g);
	void	applyGravity();
	void	applyThrust(ofVec3f t);
	void	createParticles();
	ParticleEmitter thrustEmitter;
	void	stopParticles(); 
	ParticleEmitter explodeEmitter;
	void explode();

	//ofLight light;

	ofPlanePrimitive plane;
	ofMaterial planeMaterial;


	bool bModelLoaded = false;
	bool bPlaneLoaded = false;
	bool bWireFrame = false;
	//ofxPanel gui;

	ofxToggle keyEnabled, fillEnabled, rimEnabled;

	ofxVec3Slider keyPosition, keyDiffuse, keySpecular;

	ofxVec3Slider fillPosition, fillDiffuse, fillSpecular;

	ofxVec3Slider rimPosition, rimDiffuse, rimSpecular;

	ofxFloatSlider planeShiningess;
};
