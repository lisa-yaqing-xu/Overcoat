#include "stdafx.h"
#include "data.h"
#ifndef OCT_H
#define OCT_H
int _X_Y_Z = 0;
int _X_YZ = 1;
int _XY_Z = 2;
int _XYZ = 3;
int X_Y_Z = 4;
int X_YZ = 5;
int XY_Z = 6;
int XYZ = 7;
class OctreeNode;

class OctreeNode{
private:
	OctreeNode* children[2][2][2];
	int haschildren = 0;
	vector<Vertex*> content;
	int maxitems;
	float width;
	int depth;
	float centerx;
	float centery;
	float centerz;
	int *rootd;
public:
	OctreeNode(){
	}
	OctreeNode(float x, float y, float z, float w, int limit, int d, int *rootdepth){
		centerx = x;
		centery = y;
		centerz = z;
		width = w;
		maxitems = limit;
		depth = d;
		if (depth > *rootdepth){
			*rootdepth = depth;
		}
		rootd = rootdepth;
		haschildren = 0;
	}
	~OctreeNode(){
	}
	void OctreeNode::subdivide(){
		float w = width / 2;
		for (int i = 0; i < 2; i++){			
			for (int j = 0; j < 2; j++){
				for (int k = 0; k < 2; k++){
					float x = (i == 1) ? w : -w;
					float y = (j == 1) ? w : -w;
					float z = (k == 1) ? w : -w;
					children[i][j][k] = new OctreeNode(centerx + x, centery + y, centerz + z, w, maxitems, depth + 1, rootd);
				}
			}
		}
		haschildren = 1;
		cout << "set" << haschildren << endl;
	}
	OctreeNode* OctreeNode::getChild(glm::vec3 v){
		float x = v.x;
		float y = v.y;
		float z = v.z;

		return children[int(x>centerx)][int(y>centery)][int(z>centerz)];

	}
	void OctreeNode::insert(Vertex *v){
		if (haschildren == 1){
			OctreeNode* o = getChild(v->pts);
			o->insert(v);
		}
		else {
			content.push_back(v);
			if (content.size() > maxitems){
				subdivide();
				while (!content.empty()){
					Vertex *vv = content.back();
					OctreeNode *o = getChild(vv->pts);
					o->insert(vv);
					content.pop_back();
				}
			}
		}
	}

	

};

class Octree{
private: 
	OctreeNode root;
	int totaldepth = 0;
	int maxitems = 16;
public:
	void Octree::insert(Vertex* v){
		root.insert(v);
	}
	Octree(float startx, float starty, float startz, float width, int limit){
		//maxitems = 16;//limit;
		root = OctreeNode(startx, starty, startz, width, limit, 0, &totaldepth);
		root.subdivide();
	}
	~Octree(){
		delete &root;
	}
};

#endif //OCT_H