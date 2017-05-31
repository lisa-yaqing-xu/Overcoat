#include "stdafx.h"

#ifndef DATA_H
#define DATA_H

float RotZ = 0.0;
float RotX = 0.0;

float magnification = 1.0f;

float level = 0.3;
float _levelmod = 0.1;

struct Brush{

};

struct faceInters{
	float t;
	glm::vec3 *norm;
	Face* face;
};

struct Face {
	ProxyMesh *parent;
	vector<GLshort> verts;
	bool touched = false;
	glm::vec3 normal;
	vector<SculptStroke*> sculpts;
};

struct Vertex {
	ProxyMesh *parent;
	glm::vec3 pts;
	vector<Face*> faceindices;
};

struct ProxyMesh{
public:
	vector<Vertex> vertices;
	vector<Face> faces;
};

class Colorf {
private:
	float r;
	float g;
	float b;

public:
	void Colorf::setColor256(int ir, int ig, int ib){
		r = float(ir) / 255.0f;
		g = float(ig) / 255.0f;
		b = float(ib) / 255.0f;
	}

	void Colorf::setColorf(float fr, float fg, float fb){
		r = fr;
		g = fg;
		b = fb;
	}

	void Colorf::callGlColor3f(){
		glColor3f(r, g, b);
	}

	Colorf(int ir, int ig, int ib){
		setColor256(ir, ig, ib);
	}
	Colorf(float fr, float fg, float fb){
		setColorf(fr, fg, fb);
	}
	~Colorf(){};
};


class Stroke {
public:
	vector<glm::vec3> vertices;
	vector<float> fxvals;
	glm::vec3 facenormdir;
	float initT = 0;
	Brush brush;
	int type;
	
	void Stroke::addVertex(float x, float y, float z){
		glm::vec3 v;
		v.x = x;
		v.y = y;
		v.z = x;
		vertices.push_back(v);
	}
	void Stroke::addVertex(glm::vec3 &v){
		vertices.push_back(v);
	}
	void Stroke::drawRect(glm::vec3 camv,glm::vec3 normal, glm::vec3 midpt, float width, float height ){
		glm::vec3 nn = midpt-camv;
		float _a = normal.x;
		float _b = normal.y;
		float _c = normal.z;
		float _d = _a*midpt.x + _b*midpt.y + _c*midpt.z;
		glm::vec3 u = glm::cross(normal, normalize(nn));
		glm::vec3 w = glm::cross(normalize(nn),u);

		glm::vec3 c1, c2, c3, c4;
		c1 = midpt + (width / 2)*u + (height / 2)*w;
		c2 = midpt + (width / 2)*u - (height / 2)*w;
		c3 = midpt - (width / 2)*u - (height / 2)*w;
		c4 = midpt - (width / 2)*u + (height / 2)*w;
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_FLAT);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(c1.x, c1.y, c1.z);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(c2.x, c2.y, c2.z);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(c3.x, c3.y, c3.z);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(c4.x, c4.y, c4.z);

		//glVertex3f(midpt.x, midpt.y, midpt.z);
		//glVertex3f(0, 0, 5);

	}

	void Stroke::drawVertices(glm::vec3 camv, const GLdouble *mv, const GLdouble *pm, const GLint *viewport){

		switch (type){
		case 0:
			/*glBegin(GL_QUADS);

			for (int i = 1; i <vertices.size(); i++){
				glm::vec3 v = vertices[i];
				drawRect(camv, facenormdir,v,2,2);
			}
			glEnd();*/
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			
			break;
		}
		glBegin(GL_LINES);
		for (int i = 1; i <vertices.size(); i++){
			glm::vec3 v = vertices[i - 1];
			glVertex3f(v[0], v[1], v[2]);
			//cout << a << " " << b << " " << c << endl;
			glm::vec3 w = vertices[i];
			glVertex3f(w[0], w[1], w[2]);
		}
		glEnd();


	}

		Stroke(){}
	~Stroke(){}
};

class SculptStroke{
public:
	vector<glm::vec3> vertices;
	vector<Face*> faces;
	vector<float> fxvals;
	glm::vec3 facenormdir;
	float radius = 1;
	int type;

	void SculptStroke::addVertex(glm::vec3 &v){
		this->vertices.push_back(v);
	}

	float SculptStroke::getSculptLevel(glm::vec3 pt){
		float shortestdist = FLT_MIN;
		int ind = 0;
		for (int i = 0; i < this->vertices.size(); i++){
			float d = glm::distance(this->vertices[i], pt);
			if (shortestdist > d){
				shortestdist = d;
				ind = i;
			}
		}
		float dist = glm::distance(this->vertices[ind], pt);
		if (dist > radius){
			return 0;
		}
		else{
			float a = (2 * pow(dist, 3) / pow(radius, 3)) - (3 * pow(dist, 2) / pow(radius, 2)) + 1;
			//cout << a << endl;
			return a;
		}
	
	}

	SculptStroke(){}
	~SculptStroke(){}
};

class Camera{
public:
	float x;
	float y;
	float z;

	Camera(float xx, float yy, float zz){
		x = xx;
		y = yy;
		z = zz;
	}
	~Camera(){}
	
};
#endif // DATA_H
