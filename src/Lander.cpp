#include "Lander.h"
//#include "ofApp.h"
//#include  "ofxAssimpModelLoader.h"
//#include "ofApp.cpp"


// This is the actual integrator 
// Made by: Roy and Lauren
void Lander::integrate() 
{
	float framerate = ofGetFrameRate();
	if (framerate < 1.0) return;

	float dt = 1.0 / framerate;

	// update position based on velocity
	//
	position += (velocity * dt);
	// cout << "position += velocity*dt: " << position << endl; 
	this->setPosition(position.x, position.y, position.z);

	// update acceleration with accumulated paritcles forces
	// remember :  (f = ma) OR (a = 1/m * f)
	//
	ofVec3f accel = acceleration;    // start with any acceleration already on the Lander
	accel += (forces * (1.0 / mass));
	// cout << "force on lander: " << forces << endl;
	velocity += accel * dt;

	// add a little damping for good measure
	//
	velocity *= damping;

	// clear forces on Lander (they get re-added each step)
	//
	//forces.set(0, 0, 0);
	forces.set(gravity.x, gravity.y, gravity.z); //	We always have gravity, so reset it by reapplying the gravity?
	thrustEmitter.setPosition(position);
	//light.setPosition(position); 
	explodeEmitter.setPosition(position);

	//angular motion
	/*setRotation(0, getRotationAngle(0) + angularVelocity * dt, 0, 1, 0);
	rotation += (angularVelocity * dt);
	float a = angularAcceleration;
	a += (angularForce * 1.0 / mass);
	angularVelocity += a * dt;
	angularVelocity *= damping;*/
}

//void Lander::setPosition(float x, float y, float z) {
//	position = ofVec3f(x, y, z);
//}

//	set the gravity that will work in the system
void Lander::setGravity(const ofVec3f g) {
	gravity = g;
}

//	apply gravity?
void Lander::applyGravity() {
	//
	// f = mg
	//

	forces += gravity * mass;
}

//	applying the thrust from any direction
// Made by: Roy and Lauren
void Lander::applyThrust(ofVec3f t) {
	forces += t; 
}

// Made by: Roy and Lauren
void Lander::createParticles() 
{
	//thrustEmitter.sys->reset(); 
	thrustEmitter.start();
}

// Made by: Roy and Lauren
void Lander::stopParticles()
{
	thrustEmitter.sys->reset();
	thrustEmitter.stop();
}