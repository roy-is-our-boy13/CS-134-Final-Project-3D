#pragma once
#include "ofMain.h"

//  Kevin M. Smith - CS 134 SJSU
//

//  Base class for any object that needs a transform.
//
class TransformObject 
{
public:
	ofVec3f position;

protected:
	TransformObject();
	ofVec3f scale;
	float	rotation;
	bool	bSelected;
	void setPosition(const ofVec3f &);
};