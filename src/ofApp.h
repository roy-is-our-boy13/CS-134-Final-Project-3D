#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include  "ofxAssimpModelLoader.h"
#include "Octree.h"
#include <glm/gtx/intersect.hpp>

//** Particle system and gravity start **//
#include "Particle.h"
#include "ParticleEmitter.h"

#include "Lander.h" 
//** Particle system and gravity end **//


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void checkCollisions(); 

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent2(ofDragInfo dragInfo);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(ofVec3f);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void toggleSelectTerrain();
		void setCameraTarget();
		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
		bool raySelectWithOctree(ofVec3f &pointRet);
		glm::vec3 getMousePointOnPlane(glm::vec3 p , glm::vec3 n);
		//void loadVbo(ParticleEmitter e);
		void loadVbo(); 
		void loadVbo2();
		void devMode(); 
		void fuelDraw(); 
		void landerMovement(ofVec3f m);
		void cameraSetup(); 
		void updateCameras(); 
		void setupLights(); 
		void aboveGroundLevel();
		void loseCondition(); //ran if the player lost in some way (no more fuel, hard landing, crash landing)
		void winGame(); //When player wins the game. 

		//for fuel 
		float startTime; // store when we start time timer
		float endTime; // when do want to stop the timer
		float activeStart = 0; // calculates current time when user applies fuel/thrusters
		float activeEnd = 0; // holds the value of the timer the last time it was set 
		float timer = 0; //for calculations with fuel 
		bool timerStarted = false; 
		
		bool  bTimerReached; // used as a trigger when we hit the timer


		//** Cameras **//
		ofEasyCam cam; // this is the main camera
		ofCamera cam1, cam2; // these are the other camera views
		ofCamera* theCam; // note that "theCam" is also easyCam so we can put it in the array
		int camToView; // which camera index are we looking through
		ofCamera camArray[3] = {cam, cam1, cam2}; //cam is a different kind of camera but seems to work in this array

		//ofxAssimpModelLoader mars, lander;
		ofxAssimpModelLoader lunar, model; 
		Lander lander; 

		//ofLight light;
		Box boundingBox, landerBounds; 
		Box testBox;
		vector<Box> colBoxList;
		bool bLanderSelected = false;
		Octree octree;
		TreeNode selectedNode; 
		glm::vec3 mouseDownPos, mouseLastPos;
		bool bInDrag = false;


		//** Particle system and gravity start **//
			ParticleEmitter landerParticle;

			GravityForce grav;
			ImpulseForce impulse;  // test for collisions;
		//** Particle system and gravity end **//

		// explosion
		ParticleEmitter emitter;

		bool bHide;
			ofxIntSlider numLevels;	// octree levels
			ofxFloatSlider gravity;	// strength of the gravity
			ofxFloatSlider damping;	// resistance force?
			ofxFloatSlider radius;	// radius of the particles
			ofxVec3Slider velocity;	// velocity of the particles TODO: change this to be the velocity of the ship? (no, change the thrust instead bc cant directly change velocity)
			ofxFloatSlider lifespan;	// lifespan of the particles
			ofxFloatSlider rate;	// particle spawn rate
			ofxFloatSlider restitution;	// bounceback
			ofxIntSlider thrustStr; // strength of the thrust, aka speed?


			ofxPanel gui;
		

		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		
		bool pointSelected = false;
		bool bDisplayLeafNodes = false;
		bool bDisplayOctree = false;
		bool bDisplayBBoxes = false;
		
		bool bLanderLoaded;
		bool bTerrainSelected;
	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;

		vector<Box> bboxList;

		const float selectionRange = 4.0;

		ParticleSystem* sys;


		ofLight keyLight; 
		ofLight fillLight; 
		ofLight rimLight; 
		ofLight light;

		ofPlanePrimitive plane;
		ofMaterial planeMaterial;


		bool bModelLoaded = false;
		bool bPlaneLoaded = false;
		bool bWireFrame = false;
		
		//ofxPanel gui;
		
		/*ofxToggle keyEnabled, fillEnabled, rimEnabled;

		ofxVec3Slider keyPosition, keyDiffuse, keySpecular;

		ofxVec3Slider fillPosition, fillDiffuse, fillSpecular;

		ofxVec3Slider rimPosition, rimDiffuse, rimSpecular;

		ofxFloatSlider planeShiningess;*/

		// textures
		//
		ofTexture  particleTex;

		// shaders
		//
		ofVbo vbo, vbo2; // vbo for booster, vbo2 for explosion
		ofShader shader;

		ofSoundPlayer   boosterSound;
		ofSoundPlayer   explosionSound;

		ofImage background;

		bool started = false; //game starts the first time that a player uses the up thruster 
		bool outOfBounds = true; //if it touches the ground while this is true, it's a fail condition. gets set to false if in lander. 
		bool crashLanding = false; //hit the ground too hard
		bool noMoreFuel = false; //ran out of fuel while playing 
		bool wonGame = false; 
		bool gameOver = false;

		string msg = string(""); 

		bool showInstructions = false; 
		string instructions = string("") + //TODO: ADD THE MOVEMENT KEYS
			"\n" +
			"i: toggle instructions\n" +
			"b: display boundary boxes\n" +
			"c: enable camera adjustments\n" +
			"f: toggle fullscreen\n" +
			"g: toggle gui\n" +
			"l: display leaf nodes\n" +
			"o: display octree\n" +
			"r: reset camera\n" +
			"f1: camera 1\n" +
			"f2: camera 2\n" +
			"f3: camera 3\n" +
			"backspace: reset lander position\n";

		string agl = string("") + "Current AGL: ";
};
