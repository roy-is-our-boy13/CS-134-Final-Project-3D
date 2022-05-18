
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Octree Test - startup scene
// 
//
//  Student Name:   < Your Name goes Here >
//  Date: <date of last version>


#include "ofApp.h"
#include "Util.h"

//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
	bTimerReached = false;
    startTime = ofGetElapsedTimeMillis();  // get the start time
    endTime = 120000 + startTime; // two minutes
		// endTime = 10000 + startTime;

	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
	bTerrainSelected = true;
//	ofSetWindowShape(1024, 768);
// 
	//TODO: What do we want the intial camera distance to be?
	cam.setDistance(75);

	//TODO: this has a weird reaction to having the different cameras
	//cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();

	theCam = &cam; // referencing the main camera

	cam1.setGlobalPosition(glm::vec3(100, 0, 0));
	cam1.lookAt(glm::vec3(0, 0, 0));

	cam2.setGlobalPosition(glm::vec3(0, 150, 0));
	cam2.lookAt(glm::vec3(0, 0, 0));

	boosterSound.load("sounds/booster.mp3");
	explosionSound.load("sounds/explosion.mp3");

	// texture loading
	//
	ofDisableArbTex();     // disable rectangular textures

	// load textures
	//
	if (!ofLoadImage(particleTex, "images/dot.png")) {
		cout << "Particle Texture File: images/dot.png not found" << endl;
		ofExit();
	}

	background.load("images/background.jpg");

	// load the shader
	//
	#ifdef TARGET_OPENGLES
		shader.load("shaders_gles/shader");
	#else
		shader.load("shaders/shader");
	#endif

	// setup rudimentary lighting 
	//
	initLightingAndMaterials();

	lunar.loadModel("geo/mars-low-5x-v2.obj");
	lunar.setScaleNormalization(false);


	//Loading lander without drag+drop
	if (lander.loadModel("geo/lander.obj")){
		lander.setScaleNormalization(false);
		lander.setPosition(1, 2, 0);

		bLanderLoaded = true;
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		//cout << "Mesh Count: " << lander.getMeshCount() << endl;
	}

	// create sliders for testing
	//
	gui.setup();
	gui.add(numLevels.setup("Number of Octree Levels", 1, 1, 10));
	gui.add(velocity.setup("Initial Velocity", ofVec3f(25, 35, 0), ofVec3f(0, 0, 0), ofVec3f(100, 100, 100)));	// high on default, change to thruster?
	gui.add(thrustStr.setup("Thrust", 5, 5, 100));
	gui.add(lifespan.setup("Lifespan", 2.0, .1, 10.0));
	gui.add(rate.setup("Rate", 1.0, .5, 60.0));
	gui.add(damping.setup("Damping", .99, .1, 1.0));
    gui.add(gravity.setup("Gravity", 0, 0, 20));
	gui.add(radius.setup("Radius", .01, .01, 1.0));
	gui.add(restitution.setup("Restitution", .85, 0, 1.0));
	bHide = false;

	//  Create Octree for testing.
	//
	
	// startTime = ofGetElapsedTimeMillis(); 
	octree.create(lunar.getMesh(0), 20);
	// endTime = ofGetElapsedTimeMillis() - startTime;
	//cout << "Time to run Octree:: create: " << ofToString(endTime) << " milliseconds" << endl;
	
	//cout << "Number of Verts: " << mars.getMesh(0).getNumVertices() << endl;

	testBox = Box(Vector3(3, 3, 0), Vector3(5, 5, 2));


	// Emitters
	// 

	landerParticle.setRate(rate); //?? landerParticle is the particle we'll be using to control the lander
								// we can test this by treating this as a normal particle emitter and connecting lander position to it?
	landerParticle.setOneShot(true);

	//landerParticle.start();
	landerParticle.setLifespan(20000);

	lander.setGravity(ofVec3f(0, -1.0, 0)); //set gravity for the rest of the program

	sys = landerParticle.sys;
	grav.set(ofVec3f(0, -1.0, 0));
	sys->addForce(&grav);
	//sys->addForce(new TurbulenceForce(ofVec3f(-3, -1, -1), ofVec3f(3, 1, 1)));
	
	//Lander Thrust Particles Setup.
	lander.thrustEmitter.init();
	lander.thrustEmitter.setLifespan(1);
	lander.thrustEmitter.setRate(50);
	lander.thrustEmitter.setVelocity(glm::vec3(0, -1, 0));
	lander.thrustEmitter.setParticleRadius(radius);

	//added by lauren, reference from radialEmitterExample
	lander.thrustEmitter.setGroupSize(10);
	//lander.thrustEmitter.setOneShot(true);
	//lander.thrustEmitter.setRandomLife(true);
	//lander.thrustEmitter.setLifespanRange(ofVec2f(1, 2));

	//adding forces for booster particles 
	lander.thrustEmitter.sys->addForce(new TurbulenceForce(ofVec3f(-5, -5, -5), ofVec3f(5, 5, 5)));
	lander.thrustEmitter.sys->addForce(new GravityForce(ofVec3f(0, -gravity, 0))); 
	/*lander.thrustEmitter.sys->addForce(new ImpulseRadialForce(1000));
	lander.thrustEmitter.sys->addForce(new CyclicForce(0));*/

	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
//	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableDepthTest();
	ofEnableLighting();

	// Setup 3 - Light System
	// 
	/*
	keyLight.setup();
	keyLight.enable();
	keyLight.setAreaLight(1, 1);
	keyLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	keyLight.setDiffuseColor(ofFloatColor(1, 1, 1));
	keyLight.setSpecularColor(ofFloatColor(1, 1, 1));

	keyLight.rotate(45, ofVec3f(0, 1, 0));
	keyLight.rotate(-45, ofVec3f(1, 0, 0));
	keyLight.setPosition(5, 5, 5);

	fillLight.setup();
	fillLight.enable();
	fillLight.setSpotlight();
	fillLight.setScale(.05);
	fillLight.setSpotlightCutOff(15);
	fillLight.setAttenuation(2, .001, .001);
	fillLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	fillLight.setDiffuseColor(ofFloatColor(1, 1, 1));
	fillLight.setSpecularColor(ofFloatColor(1, 1, 1));
	fillLight.rotate(-10, ofVec3f(1, 0, 0));
	fillLight.rotate(-45, ofVec3f(0, 1, 0));
	fillLight.setPosition(-5, 5, 5);

	rimLight.setup();
	rimLight.enable();
	rimLight.setSpotlight();
	rimLight.setScale(.05);
	rimLight.setSpotlightCutOff(30);
	rimLight.setAttenuation(.2, .001, .001);
	rimLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	rimLight.setDiffuseColor(ofFloatColor(1, 1, 1));
	rimLight.setSpecularColor(ofFloatColor(1, 1, 1));
	rimLight.rotate(180, ofVec3f(0, 1, 0));
	rimLight.setPosition(0, 5, -7);*/

}
//--------------------------------------------------------------
//
// load vertex buffer in preparation for rendering
//
void ofApp::loadVbo() {
	if (lander.thrustEmitter.sys->particles.size() < 1) return;

	vector<ofVec3f> sizes;
	vector<ofVec3f> points;
	for (int i = 0; i < lander.thrustEmitter.sys->particles.size(); i++) {
		points.push_back(lander.thrustEmitter.sys->particles[i].position);
		sizes.push_back(ofVec3f(radius));
	}
	// upload the data to the vbo
	//
	int total = (int)points.size();
	vbo.clear();
	vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
}
//--------------------------------------------------------------
//
//	FROM: examples/particleBouncingBall, written by prof. smith
//  This a very simple function to check for collision on the ground plane at (0,0,0)
//  If the partical position.y value is smaller than it's radius, we will assume
//  it's has gone through the plane and we apply a simple impulse function
//  resolve it..
//
void ofApp::checkCollisions() { //TODO: currently this is off of particle position. We need to change it to OCTREE collisions

	//Collision Detection(final proj)
	//	* Take normal of each vertex from leaf nodes, and calculate average of them
	//	- No rotations with spacecraft / lander, only apply impulse force
	//	* Collision steps
	//	1.) Detect Collision
	//	2.) Get contact info
	//		> Normal (normalize?)
	//	3.) Resolve Interpenetrations
	//		p' = p + v * dt
	//		p = p' - v * dt
	//	4.) Apply Impulse
	//> Same formula for 2D applies for 3D(from Midterm test ? )
	//> No friction or micro - collisions



	// for each particle, determine if we hit the groud plane.
	//

	//TODO: It looks like the collisions are actually being detected, but are they being normalized? How do I resolve interpenetrations?
	ofVec3f min = lander.getSceneMin() + lander.getPosition();
	ofVec3f max = lander.getSceneMax() + lander.getPosition();

	Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
	colBoxList.clear();

	octree.intersect(bounds, octree.root, colBoxList); //colBoxList describes the octree boxes that the lander is colliding with

	//for (int j = 0 ; j < colBoxList.size() ; j++) {
	if(colBoxList.size() > 0) { //We want this restiition force once if there is any collision
		lander.setGravity(ofVec3f(0, 0, 0)); //set gravity to ZERO
		ofVec3f vel = lander.velocity; //access the velocity of the lander for the restitution bounce
		//cout << "lander velocity: " << vel << endl;

		// apply impulse function
		//
		ofVec3f norm = ofVec3f(0, 1, 0);  // TODO: this should be normalizing the mesh/vertices(?)
		ofVec3f f = (restitution + 1.0) * ((-vel.dot(norm)) * norm);
		//cout << "f: " << f << endl; //the force we're applying to the lander
		//cout << "lander.forces: " << lander.forces << endl;
		//cout << "ofGetFrameRate() * f: " << ofGetFrameRate() * f << endl;
		//lander.forces += ofGetFrameRate() * f
		lander.forces += f; //doing it without frame rate to see what happens?
		//lander.integrate();//Let the update cycle take care of intagrating forces

		//cout << "AFTER ADDITION: " << lander.forces << endl;
		//cout << "==============================================================================" << endl;
	}
	else
	{
		lander.setGravity(ofVec3f(0, -1.0, 0)); //set gravity for the rest of the program

	}
	// cout << "lander velocity: " << lander.velocity <<
	// 	"\nlander.forces: " << lander.forces <<
	// 	"\nAFTER ADDITION: " << lander.forces <<
	// 	"\n==============================================================================" << endl;

	//for (int i = 0; i < landerParticle.sys->particles.size(); i++) {

 //      // only bother to check for descending particles.
 //      //
	//	ofVec3f vel = landerParticle.sys->particles[i].velocity; // velocity of particle
	//	if (vel.y >= 0) break;                             // ascending;

	//	ofVec3f pos = landerParticle.sys->particles[i].position;
	//	//cout << "pos.y =" << pos.y << endl;
	//	if (pos.y < landerParticle.sys->particles[i].radius) {
	//		cout << "impulse function" << endl; 
	//		// apply impulse function
	//		//
	//		ofVec3f norm = ofVec3f(0, 1, 0);  // just use vertical for normal for now
	//		ofVec3f f = (restitution + 1.0)*((-vel.dot(norm))*norm);
	//		landerParticle.sys->particles[i].forces += ofGetFrameRate() * f;
	//	}
	//}

}
 
//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() 
{
	checkCollisions();

	// for the lander movement
	grav.set(ofVec3f(0, -gravity, 0));
	landerParticle.setParticleRadius(radius);
	//lander.setGravity(ofVec3f(0, -gravity, 0)); //Don't need to update gravity on every update:-)
	lander.integrate();

	//TODO: should I be adding the particle integrator here?
	// get velocity from slider
	glm::vec3 v = velocity;
	landerParticle.setVelocity(v);
	//
	// update objects you've created here
	landerParticle.update();

	//if (landerParticle.sys->particles.size() >= 1) { 
	//	ofVec3f vel = landerParticle.sys->particles[0].position; // checking location of the first particle in the system... 

	//	lander.setPosition(vel.x, vel.y, vel.z);
	//}

	lander.thrustEmitter.update();
}
//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(ofColor::white); 
	background.draw(0, 0);

	//ofBackground(ofColor::black);

	glDepthMask(false);

	// this makes everything look glowy :)
	//
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnablePointSprites();

	if (!bHide) gui.draw();
	glDepthMask(true);

	 //cam.begin();
	theCam->begin(); 
	shader.begin();

	// draw particle emitter here..
	//
	//	emitter.draw();
	particleTex.bind();
	vbo.draw(GL_POINTS, 0, (int)lander.thrustEmitter.sys->particles.size());
	particleTex.unbind();

	ofPushMatrix();
	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		lunar.drawWireframe();
		if (bLanderLoaded) {
			lander.drawWireframe();
			if (!bTerrainSelected) drawAxis(lander.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		lunar.drawFaces();
		ofMesh mesh;
		if (bLanderLoaded) {
			lander.drawFaces();
			if (!bTerrainSelected) drawAxis(lander.getPosition());
			if (bDisplayBBoxes) {
				ofNoFill();
				ofSetColor(ofColor::white);
				for (int i = 0; i < lander.getNumMeshes(); i++) {
					ofPushMatrix();
					ofMultMatrix(lander.getModelMatrix());
					ofRotate(-90, 1, 0, 0);
					Octree::drawBox(bboxList[i]);
					ofPopMatrix();
				}
			}
			ofVec3f min = lander.getSceneMin() + lander.getPosition();
			ofVec3f max = lander.getSceneMax() + lander.getPosition();

			Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
			ofNoFill(); // prevents holding box from filling white
			ofSetColor(ofColor::white);
			Octree::drawBox(bounds);

			// draw colliding boxes
			//
			ofSetColor(ofColor::red);
			for (int i = 0; i < colBoxList.size(); i++) {
				Octree::drawBox(colBoxList[i]);
			}

			if (bLanderSelected) {

				ofVec3f min = lander.getSceneMin() + lander.getPosition();
				ofVec3f max = lander.getSceneMax() + lander.getPosition();

				Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
				ofNoFill(); // prevents holding box from filling white
				ofSetColor(ofColor::white);
				Octree::drawBox(bounds);

				// draw colliding boxes
				//
				ofSetColor(ofColor::red);
				for (int i = 0; i < colBoxList.size(); i++) {
					Octree::drawBox(colBoxList[i]);
				}
			}
			lander.thrustEmitter.draw();
		}
	}
	if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));



	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		lunar.drawVertices();
	}

	// highlight selected point (draw sphere around selected point)
	//
	if (bPointSelected) {
		ofSetColor(ofColor::blue);
		ofDrawSphere(selectedPoint, .1);
	}


	// recursively draw octree
	//
	ofDisableLighting();
	int level = 0;
	//	ofNoFill();

	if (bDisplayLeafNodes) {
		octree.drawLeafNodes(octree.root);
		//cout << "num leaf: " << octree.numLeaf << endl;
    }
	else if (bDisplayOctree) {
		ofNoFill();
		ofSetColor(ofColor::white);
		octree.draw(numLevels, 0);
	}

	// if point selected, draw a sphere
	//
	if (pointSelected) {
		ofVec3f p = octree.mesh.getVertex(selectedNode.points[0]);
		ofVec3f d = p - cam.getPosition();
		ofSetColor(ofColor::lightGreen);
		ofDrawSphere(p, .02 * d.length());
	}

	//
	// draw any other objects here...
	//
	landerParticle.draw(); //particle(?) for the lander movement

	ofPopMatrix();
	
	//TODO: draw these?
	/*cam.draw();
	cam1.draw();
	cam2.draw();*/

	shader.end();
	theCam->end();
	 //cam.end();

	ofDisablePointSprites();
	ofDisableBlendMode();
	ofEnableAlphaBlending();

	fuelDraw(); //to draw the fuel bar


	//--
	// Draw annotations (text, gui, etc)

	// ofPushStyle();
	// // ofDisableDepthTest();

	// // draw some labels
	// ofSetColor(255, 255, 255);
	// ofDrawBitmapString("Press keys 1-4 to select a camera for main view", ofGetWidth()/2, 30);
	
	// ofPopStyle();
	
	//
	//--
}


// 
// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
	

	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {

	switch (key) { //TODO: thrust strength might be too strong at the moment, or the gravity is too weak
	case ' ': //apply up booster
		landerMovement(ofVec3f(0, 1*thrustStr, 0)); 
		//Tell lander to create particles with its thrust emitter
		//lander needs a new method that handles creating particles.
		lander.createParticles();
		break;
	case OF_KEY_UP: // forward
		cout << "forward" << endl; 
		landerMovement(ofVec3f(0, 0, -1 * thrustStr)); 
		break; 
	case OF_KEY_LEFT: // left
		cout << "left" << endl;
		landerMovement(ofVec3f(-1 * thrustStr, 0, 0)); 
		break;
	case OF_KEY_DOWN: // back
		cout << "back" << endl;
		landerMovement(ofVec3f(0, 0, 1 * thrustStr));
		break;
	case OF_KEY_RIGHT: // right
		cout << "right" << endl;
		landerMovement(ofVec3f(1 * thrustStr, 0, 0)); 
		break;
	case 'B':
	case 'b':
		bDisplayBBoxes = !bDisplayBBoxes;
		break;
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'D':
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		break;
	case 'L':
	case 'l':
		bDisplayLeafNodes = !bDisplayLeafNodes;
		break;
	case 'O':
	case 'o':
		bDisplayOctree = !bDisplayOctree;
		break;
	case 'r':
		cam.reset();
		break;
	case 's':
		savePicture();
		break;
	case 't':
		setCameraTarget();
		break;
	case 'u': //** RESET THE POSITION OF THE STARTING PARTICLE
		//ofVec3f vel = landerParticle.sys->particles[0].position;

		landerParticle.sys->particles[0].position = ofVec3f(1, 1, 0);
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'w':
		toggleWireframeMode();
		break;
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_DEL:
		break;
	case OF_KEY_F1:
		theCam = &cam;
		break;
	case OF_KEY_F2:
		theCam = &cam1;
		break;
	case OF_KEY_F3:
		theCam = &cam2;
		break;
	case OF_KEY_SHIFT:				// release a particle
		//landerParticle.start(); //TODO: How do we use this to relaunch the same particle more than once?
		//if (landerParticle.sys->particles.size() <= 0) { landerParticle.start(); } //TODO: uncomment to start the game?
		break;
	case OF_KEY_BACKSPACE:				// return particle to original pos
		//lander.position = ofVec3f(1, 1, 0); 
		break;
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {

	switch (key) {
	case ' ': 
		lander.stopParticles();
		timerStarted = false; // we stopped using the thruster
		activeEnd = timer; // remember the amount of fuel we spent
		break;
	case OF_KEY_UP: // forward
		timerStarted = false; // we stopped using the thruster
		activeEnd = timer; // remember the amount of fuel we spent
		break; 
	case OF_KEY_LEFT: // left
		timerStarted = false; // we stopped using the thruster
		activeEnd = timer; // remember the amount of fuel we spent
		break;
	case OF_KEY_DOWN: // back
		timerStarted = false; // we stopped using the thruster
		activeEnd = timer; // remember the amount of fuel we spent
		break;
	case OF_KEY_RIGHT: // right
		timerStarted = false; // we stopped using the thruster
		activeEnd = timer; // remember the amount of fuel we spent
		break;
	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
	default:
		break;

	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	// if moving camera, don't allow mouse interaction
//
	if (cam.getMouseInputEnabled()) return;

	// if rover is loaded, test for selection
	//
	if (bLanderLoaded) {
		glm::vec3 origin = cam.getPosition();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

		// startTime = ofGetElapsedTimeMillis(); 
		bool hit = bounds.intersect(Ray(Vector3(origin.x, origin.y, origin.z), Vector3(mouseDir.x, mouseDir.y, mouseDir.z)), 0, 10000);
		// endTime = ofGetElapsedTimeMillis() - startTime;
		//cout << "Time to run bounds.intersect: " << ofToString(endTime) << " milliseconds" << endl;

		if (hit) {
			bLanderSelected = true;
			mouseDownPos = getMousePointOnPlane(lander.getPosition(), cam.getZAxis());
			mouseLastPos = mouseDownPos;
			bInDrag = true;
		}
		else {
			bLanderSelected = false;
		}
	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}

bool ofApp::raySelectWithOctree(ofVec3f &pointRet) {
	ofVec3f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
		Vector3(rayDir.x, rayDir.y, rayDir.z));

	// startTime = ofGetElapsedTimeMillis(); 
	pointSelected = octree.intersect(ray, octree.root, selectedNode);
	// endTime = ofGetElapsedTimeMillis() - startTime;
	//cout << "Time to run octree.intersect: " << ofToString(endTime) << " milliseconds" << endl;

	if (pointSelected) {
		pointRet = octree.mesh.getVertex(selectedNode.points[0]);
	}
	
	cout << "touching the lander:" << pointSelected << endl; 
	return pointSelected;
}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	if (bInDrag) { //if dragging the lander? 

		glm::vec3 landerPos = lander.getPosition();

		glm::vec3 mousePos = getMousePointOnPlane(landerPos, cam.getZAxis());
		glm::vec3 delta = mousePos - mouseLastPos;
	
		landerPos += delta;
		lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
		mouseLastPos = mousePos;

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

		colBoxList.clear();

		// startTime = ofGetElapsedTimeMillis(); 
		octree.intersect(bounds, octree.root, colBoxList);
		// endTime = ofGetElapsedTimeMillis() - startTime;
		//cout << "Time to run octree.intersect: " << ofToString(endTime) << " milliseconds" << endl;

		/*if (bounds.overlap(testBox)) {
			cout << "overlap" << endl;
		}
		else {
			cout << "OK" << endl;
		}*/


	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bInDrag = false;
}



// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {

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
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent2(ofDragInfo dragInfo) {

	ofVec3f point;
	mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);
	if (lander.loadModel(dragInfo.files[0])) {
		lander.setScaleNormalization(false);
//		lander.setScale(.1, .1, .1);
	//	lander.setPosition(point.x, point.y, point.z);
		lander.setPosition(1, 1, 0);

		bLanderLoaded = true;
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		//cout << "Mesh Count: " << lander.getMeshCount() << endl;
	}
	else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (lander.loadModel(dragInfo.files[0])) {
		bLanderLoaded = true;
		lander.setScaleNormalization(false);
		lander.setPosition(0, 0, 0);
		//cout << "number of meshes: " << lander.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		//		lander.setRotation(1, 180, 1, 0, 0);

				// We want to drag and drop a 3D object in space so that the model appears 
				// under the mouse pointer where you drop it !
				//
				// Our strategy: intersect a plane parallel to the camera plane where the mouse drops the model
				// once we find the point of intersection, we can position the lander/lander
				// at that location.
				//

				// Setup our rays
				//
		glm::vec3 origin = cam.getPosition();
		glm::vec3 camAxis = cam.getZAxis();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPoint = origin + distance * mouseDir;

			// Now position the lander's origin at that intersection point
			//
			glm::vec3 min = lander.getSceneMin();
			glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);

			// set up bounding box for lander while we are at it
			//
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
	}


}

//  intersect the mouse ray with the plane normal to the camera 
//  return intersection point.   (package code above into function)
//
glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
	// Setup our rays
	//
	glm::vec3 origin = cam.getPosition();
	glm::vec3 camAxis = cam.getZAxis();
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}
//--------------------------------------------------------------
//	a state mode that enables the following user actions: 
//	- pause gravity (TODO:)
//	- drag lander (TODO:)
//	- reset timer/gas (TODO:)
//	- turn off collisions? (TODO:)
//	- move camera to any angle (TODO:)
//	- ??? (More?)
void ofApp::devMode(){ 

}
//--------------------------------------------------------------
void ofApp::fuelDraw() {
		// "Turns off depth testing so rendering happens in draw order rather than by z-depth. Turning off depth test is useful for combining 3d scenes with 2d overlays such as a control panel."
		ofDisableDepthTest(); 
		
    float barWidth = 500;

		if (timerStarted){ //currently holding down the thrusters
			timer = startTime + activeStart + activeEnd; 
			//if thrusters being applied, see how much is being drained
		}
    
    if(timer >= endTime && !bTimerReached) {
        bTimerReached = true;        
        ofMessage msg("Timer Reached");
        ofSendMessage(msg);
				cout << "timer reached" << endl; 
    }
    
    // the background to the progress bar
    ofSetColor(105,105,105);
		ofFill(); //without this, the program only draws an unfilled rectangle 
    ofDrawRectangle((ofGetWidth()-barWidth)/2, ofGetHeight()-100, barWidth, 30);
    
    // draw the percent of the bar completed
    float pct = ofMap(timer, 0.0, endTime, 0.0, 1.0, true);
    ofSetHexColor(0xf02589);
		ofFill(); //without this, the program only draws an unfilled rectangle 
		ofDrawRectangle((ofGetWidth()-barWidth)/2, ofGetHeight()-100, barWidth-barWidth*pct, 30);

    // write the percentage next to the bar
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ofToString((1-pct)*100, 0)+"%", (ofGetWidth()/2), (ofGetHeight()-80)); //should we change this to a timer instead of a percent?
    
    // the timer was reached :)
    if(bTimerReached) {
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("Out of fuel!", (ofGetWidth()-100)/2, (ofGetHeight()-50));
				//TODO: this is the lose condition, need to code the loss
    }
}
//--------------------------------------------------------------
//take the movement input dictated by the key pressed, then apply the timer
void ofApp::landerMovement(ofVec3f m){
	if (!timerStarted){ // only reset when we're applying thrust for the first time (not repeatedly)
		ofResetElapsedTimeCounter();  
		timerStarted = true; 
	}
	activeStart = ofGetElapsedTimeMillis(); // see how long we're holding the thrust down
	lander.applyThrust(m);
}
//--------------------------------------------------------------
