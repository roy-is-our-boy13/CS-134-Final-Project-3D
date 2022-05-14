
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Simple Octree Implementation 11/10/2020
// 
//  Copyright (c) by Kevin M. Smith
//  Copying or use without permission is prohibited by law. 
//

#include "Octree.h"
 
//draw a box from a "Box" class  
//
void Octree::drawBox(const Box &box) {
	Vector3 min = box.parameters[0];
	Vector3 max = box.parameters[1];
	Vector3 size = max - min;
	Vector3 center = size / 2 + min;
	ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
	float w = size.x();
	float h = size.y();
	float d = size.z();
	//ofSetColor(colors[colorIndex]);
	ofDrawBox(p, w, h, d);
}

// return a Mesh Bounding Box for the entire Mesh
//
Box Octree::meshBounds(const ofMesh & mesh) {
	int n = mesh.getNumVertices();
	ofVec3f v = mesh.getVertex(0);
	ofVec3f max = v;
	ofVec3f min = v;
	for (int i = 1; i < n; i++) {
		ofVec3f v = mesh.getVertex(i);

		if (v.x > max.x) max.x = v.x;
		else if (v.x < min.x) min.x = v.x;

		if (v.y > max.y) max.y = v.y;
		else if (v.y < min.y) min.y = v.y;

		if (v.z > max.z) max.z = v.z;
		else if (v.z < min.z) min.z = v.z;
	}
	cout << "vertices: " << n << endl;
//	cout << "min: " << min << "max: " << max << endl;
	return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

// getMeshPointsInBox:  return an array of indices to points in mesh that are contained 
//                      inside the Box.  Return count of points found;
//
int Octree::getMeshPointsInBox(const ofMesh & mesh, const vector<int>& points,
	Box & box, vector<int> & pointsRtn)
{
	int count = 0;
	for (int i = 0; i < points.size(); i++) {
		ofVec3f v = mesh.getVertex(points[i]);
		if (box.inside(Vector3(v.x, v.y, v.z))) {
			count++;
			pointsRtn.push_back(points[i]);
		}
	}
	return count;
}

// getMeshFacesInBox:  return an array of indices to Faces in mesh that are contained 
//                      inside the Box.  Return count of faces found;
//
int Octree::getMeshFacesInBox(const ofMesh & mesh, const vector<int>& faces,
	Box & box, vector<int> & facesRtn)
{
	int count = 0;
	for (int i = 0; i < faces.size(); i++) {
		ofMeshFace face = mesh.getFace(faces[i]);
		ofVec3f v[3];
		v[0] = face.getVertex(0);
		v[1] = face.getVertex(1);
		v[2] = face.getVertex(2);
		Vector3 p[3];
		p[0] = Vector3(v[0].x, v[0].y, v[0].z);
		p[1] = Vector3(v[1].x, v[1].y, v[1].z);
		p[2] = Vector3(v[2].x, v[2].y, v[2].z);
		if (box.inside(p,3)) {
			count++;
			facesRtn.push_back(faces[i]);
		}
	}
	return count;
}

//  Subdivide a Box into eight(8) equal size boxes, return them in boxList;
//
void Octree::subDivideBox8(const Box &box, vector<Box> & boxList) {
	Vector3 min = box.parameters[0];
	Vector3 max = box.parameters[1];
	Vector3 size = max - min;
	Vector3 center = size / 2 + min;
	float xdist = (max.x() - min.x()) / 2;
	float ydist = (max.y() - min.y()) / 2;
	float zdist = (max.z() - min.z()) / 2;
	Vector3 h = Vector3(0, ydist, 0);

	//  generate ground floor
	//
	Box b[8];
	b[0] = Box(min, center);
	b[1] = Box(b[0].min() + Vector3(xdist, 0, 0), b[0].max() + Vector3(xdist, 0, 0));
	b[2] = Box(b[1].min() + Vector3(0, 0, zdist), b[1].max() + Vector3(0, 0, zdist));
	b[3] = Box(b[2].min() + Vector3(-xdist, 0, 0), b[2].max() + Vector3(-xdist, 0, 0));

	boxList.clear();
	for (int i = 0; i < 4; i++)
		boxList.push_back(b[i]);

	// generate second story
	//
	for (int i = 4; i < 8; i++) {
		b[i] = Box(b[i - 4].min() + h, b[i - 4].max() + h);
		boxList.push_back(b[i]);
	}

	// note that it doesn't return in the traditional sense, but it alters 'boxList'
}

void Octree::create(const ofMesh & geo, int numLevels) {
	// initialize octree structure
	//
	mesh = geo;
	int level = 0;
	root.box = meshBounds(mesh);
	if (!bUseFaces) {
		for (int i = 0; i < mesh.getNumVertices(); i++) {
			root.points.push_back(i);
		}
	}
	else {
		// need to load face vertices here
		//
	}

	// recursively build octree
	//
	level++;
    subdivide(mesh, root, numLevels, level);
}
             
void Octree::subdivide(const ofMesh & mesh, TreeNode & node, int numLevels, int level) {
	if (level >= numLevels) return;

	// presumably when we get to this algo, we have the node for the box containing the list of point indices & list of child boxes
	// this node is passed to subdivide, called 'node' 

	vector<Box> boxList; // we create a vector of boxes called 'boxList' to receive the list of boxes from subDivideBox8
	subDivideBox8(node.box, boxList); //use helper function 

	// for each child box... 
	for (int i = 0; i < boxList.size(); i++) {
		TreeNode child = TreeNode(); // use this to set data into child nodes 
		child.box = boxList[i]; 

		vector<int> pointsRtn; 

		 getMeshPointsInBox(mesh, node.points, child.box, pointsRtn);

		child.points = pointsRtn; // set child points based on the points inside this child?

		if (pointsRtn.size() > 0) {
			node.children.push_back(child);
			if (pointsRtn.size() > 1) {
				// if the child has more than one child, it isn't a leaf node. you need to call subdivide on that condition (recursively)
				// cout << "call subdivide recursively #" << i << endl;
				subdivide(mesh, node.children[node.children.size() - 1], numLevels, level++);
			}
		}
	}
}

// Implement functions below for Homework project
//

bool Octree::intersect(const Ray &ray, const TreeNode & node, TreeNode & nodeRtn) {
	bool intersects = false;
	intersects = node.box.intersect(ray, -1000, 1000); //returns true if in the valid hit range of the ray 

	if (intersects){ //if true, check if children size > 0
		if (node.children.size() > 0){ //has children, we can recurse
			for (int j = 0; j < node.children.size(); j++) { // if our ray intersects with the box in question, we need to check the children
				intersects = intersect(ray, node.children[j], nodeRtn);
				if (intersects) { return intersects; } //if true, we break out 
			}
		}
		else { //no children, this is a leaf node 
			nodeRtn = node;
		}
	}

	return intersects;
}

bool Octree::intersect(const Box &box, TreeNode & node, vector<Box> & boxListRtn) {
	bool intersects = false;

	// the given "box" is the lander box 
	// "node" is octree.root, it is possible to access this box 
	// i think that 'boxListRtn' is the list of boxes that intersect with the lander 
			//boxListRtn.push_back() can be used to add the boxes 

	intersects = node.box.overlap(box); //i tried to do it the other way around, but it doesn't seem like its compatible
	
	if (intersects)
	{
		if (node.children.size() > 0) 
		{ //has children, we can recurse
			for (int k = 0; k < node.children.size(); k++)
			{ // if our ray intersects with the box in question, we need to check the children
				intersects = intersect(box, node.children[k], boxListRtn);
				if (intersects)
				{ 
					return intersects; 
				} //if true, we break out 
			}
		}
		else { //no children, this is a leaf node 
			boxListRtn.push_back(node.box);
		}
	}

	//TODO: uncomment and remove the other return statement 
	//return intersects;
	return false; 
}

	// implemented based on Octree Reference document
	// draw Octree (recursively)
	// 
void Octree::draw(TreeNode & node, int numLevels, int level)
{
	if (level >= numLevels)
	{ 
		return; 
	}

	drawBox(node.box);
	level++;
	for (int i = 0; i < node.children.size(); i++) 
	{
		draw(node.children[i], numLevels, level);
	}
}

// Optional
//
void Octree::drawLeafNodes(TreeNode & node) 
{
	if (node.children.size() <= 0) 
	{
		drawBox(node.box);
		return;
	}
	else 
	{
		for (int i = 0; i < node.children.size(); i++) 
		{
			drawLeafNodes(node.children[i]);
		}
	}
}


bool Octree::intersetingPoint(ofVec3f&, TreeNode& node, TreeNode& noder) 
{
	//if (node.box) 
	
}


