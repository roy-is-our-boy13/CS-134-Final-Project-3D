#ifndef _BOX_H_
#define _BOX_H_

#include <assert.h>
#include "vector3.h"
#include "ray.h"
#include <glm/gtx/intersect.hpp>

/*
 * Axis-aligned bounding box class, for use with the optimized ray-box
 * intersection test described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */

class Box {
  public:
    Box() 
	{

	}
    Box(const Vector3 &min, const Vector3 &max) 
	{
 //     assert(min < max);
      parameters[0] = min;
      parameters[1] = max;
    }
    // (t0, t1) is the interval for valid hits
    bool intersect(const Ray &, float t0, float t1) const;

    // corners
    Vector3 parameters[2];

	Vector3 min()
	{
		return parameters[0]; 
	}

	Vector3 max() 
	{ 
		return parameters[1]; 
	}

	const bool inside(const Vector3 &p) 
	{
		return ((p.x() >= parameters[0].x() && p.x() <= parameters[1].x()) &&
		     		(p.y() >= parameters[0].y() && p.y() <= parameters[1].y()) &&
						(p.z() >= parameters[0].z() && p.z() <= parameters[1].z()));
	}
	const bool inside(Vector3 *points, int size) 
	{
		bool allInside = true;
		for (int i = 0; i < size; i++) 
		{
			if (!inside(points[i])) 
			{ 
				allInside = false; 
			}
			break;
		}
		return allInside;
	}

	// implement for Homework Project
	//
	 bool overlap(const Box &box) {
		 // 1 refers to the box being passed via params, 2 refers to the box this is called on 

		 float minX1 = box.parameters[0].x();
		 float maxX1 = box.parameters[1].x();
		 float minX2 = parameters[0].x();
		 float maxX2 = parameters[1].x();

		 float minY1 = box.parameters[0].y();
		 float maxY1 = box.parameters[1].y();
		 float minY2 = parameters[0].y();
		 float maxY2 = parameters[1].y();

		 float minZ1 = box.parameters[0].z();
		 float maxZ1 = box.parameters[1].z();
		 float minZ2 = parameters[0].z();
		 float maxZ2 = parameters[1].z();

		 bool overlappingX = (maxX1 >= minX2) && (maxX2 >= minX1); 
		 bool overlappingY = (maxY1 >= minY2) && (maxY2 >= minY1);
		 bool overlappingZ = (maxZ1 >= minZ2) && (maxZ2 >= minZ1);

		 bool isOverlapping = overlappingX && overlappingY && overlappingZ; 

		 return isOverlapping;
	}

	 //This centers the box
	Vector3 center() 
	{
		return ((max() - min()) / 2 + min());
	}

	float length()//Roy Perlman
	{
		return ((max() - min()).x());
	}

	float height()//Roy Perlman
	{
		return ((max() - min()).y());
	}

	float width()//Roy Perlman
	{
		return((max() - min()).z());
	}


};

#endif // _BOX_H_
