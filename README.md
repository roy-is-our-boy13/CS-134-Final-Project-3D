
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

* testing a change