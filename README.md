
# The Physics

#### PARTICLE EMITTER
* Takes a **PARTICLE SYSTEM** and emitts the particles inside it
* **PARTICLE SYSTEM** is a collection filled with **PARTICLE**
* The particle emitter is a *TRANSFORMABLE OBJECT* (why?)

## Changing the Particle System to work on the lander

#### Logic 
* Used the program example **particleBouncingBall** provided by the professor
* We may be using the particle system to work on both the lander movement and the particles (although particles will be rendered *with a shader* for speed) so I don't want to edit it to accomodate only one
* When creating the Particle Emitter I'll have the initialization take a group size and a lifespan (group size = how many are created, lifespan = how long they last) because these will vary between the two uses
* Alternatively: have one default and just set these values with the other (i.e. emitter.setLifespan)
* Changing the group size seems to change the gravity or make it behave strangely 
* Different forces will act on the explosion particles and the lander particle


* The system uses particle velocity right now but you need to employ thrust instead (what's a good lab example of this?) 

#### Writing my Own Integrator
* Decided to switch to writing my own integrator (too much needed to change to make the particle system work for this)
* Clicking the directional keys (arrows, or maybe wasd if I edit later) moves the lander in that direction
* *Do I need to change how that movement works (use upward thrust and rotation, no directional?)*
* Need to apply gravity (how often do I apply it? it works every update, correct?); 
* Play with the thruster and gravity values (and figure out what to do to make going down easier/faster)

#### Collisions and Restitution
* box intersect, using the bounding box of the lander
* according to class: take normal of each vertex from leaf nodes, and calculate avg of them

Steps: 
1. Detect Collision
2. Get contact info
    * > Normal
3. Resolve Interpenetrations
Get the normal vectors from the mech ("vertices normal, not faces") 
        p' = p + v * dt
        p = p' - v * dt

From the slides: 
        P = (e + 1) * ( -v .dot. n ) * n;
* simplified because mass of moving object is 1
* object2 is zero (stationary) and infinite mass (like terrain
* v is the velocity of the moving object
* n is the normal at the contact point
* e is the material restitution in [0,1]
* impulse force p is going to act on object to move it in reflective direction from the collision surface, based on relative velocities of the objects and surface normal
* Epsilon e is the "resitution" of the material, with 1 being the most bouncy and 0 being no bounce

4. Apply Impulse
  *   > Same formula for 2D applies for 3D (from Midterm test?)
  *   > No friction or micro-collisions
* Use lunar landing surface for final, not Mars
Character (hierarchy)
* Mostly covered slides