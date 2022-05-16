#include "Lander.h"


// This is the actual integrator 
//
void Lander::integrate() {
	float framerate = ofGetFrameRate();
	if (framerate < 1.0) return;

	float dt = 1.0 / framerate;

	// update position based on velocity
	//
	position += (velocity * dt);
	cout << "position += velocity*dt: " << position << endl; 
	this->setPosition(position.x, position.y, position.z);

	// update acceleration with accumulated paritcles forces
	// remember :  (f = ma) OR (a = 1/m * f)
	//
	ofVec3f accel = acceleration;    // start with any acceleration already on the Lander
	accel += (forces * (1.0 / mass));
	velocity += accel * dt;

	// add a little damping for good measure
	//
	velocity *= damping;

	// clear forces on Lander (they get re-added each step)
	//
	forces.set(0, 0, 0);
}

//	set the gravity that will work in the system
void Lander::setGravity(const ofVec3f g) {
	gravity = g;
}

//	apply gravity?
void Lander::applyGravity() {
	//
	// f = mg
	//

	cout << "applying gravity" << endl; 
	forces += gravity * mass;
}

//	applying the thrust from any direction
void Lander::applyThrust(ofVec3f t) {
	cout << "applying thrust" << endl;
	forces += t; 
}
