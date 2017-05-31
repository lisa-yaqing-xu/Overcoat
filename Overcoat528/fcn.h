#pragma once
#include "stdafx.h"
#include "data.h"
#include "colors.h"
#include "octree.h"
#ifndef CALC_H
#define CALC_H
#include<windows.h>
vector<SculptStroke> sculpts;

/*bool readPNG(char* filename, int &outW, int&outH, GLubyte **imgdata){
	//didn't work out entirely so im just going to remove it
}*/

/*
	Followed this tutorial: https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
	to load an OBJ into OpenGL
*/
void loadFile(char* filename, ProxyMesh *m){//, Octree *oct){//, vector<glm::vec4> &vertices, vector<glm::vec3> &normals, vector<GLushort> &faces){
	cout << "loading " << filename << endl;
	DWORD t1 = GetTickCount();
	
	ifstream in(filename, ios::in);
	while (!in){
		cout << filename << " is invalid input. Please try again." << endl;
		cin >> filename;
		in = ifstream(filename, ios::in);
	}
	string str;
	while (getline(in, str)){
		if (str.substr(0, 2) == "v "){
			istringstream s(str.substr(2));
			glm::vec3 vv;
			s >> vv.x;
			s >> vv.y;
			s >> vv.z;
			Vertex v;
			v.pts = vv;
			m->vertices.push_back(v);
			//oct->insert(&v);
			//cout << "X" << v.pts.x << " Y" << v.pts.y << " Z" << v.pts.z << endl;
		}
		else if (str.substr(0, 2) == "f "){
			istringstream s(str.substr(2));
			string aa, bb, cc;
			s >> aa;
			s >> bb;
			s >> cc;

			GLushort a, b, c;

			istringstream conv(aa.substr(0, aa.find("//")));
			conv >> a;
			conv = istringstream(bb.substr(0, bb.find("//")));
			conv >> b;
			conv = istringstream(cc.substr(0, cc.find("//")));
			conv >> c;

			a--; b--; c--;
			//cout << a << " " << b << " " << " " << c << endl;
			Face f;

			f.verts.push_back(a);
			f.verts.push_back(b);
			f.verts.push_back(c);
			m->vertices[a].faceindices.push_back(&f);
			m->vertices[b].faceindices.push_back(&f);
			m->vertices[c].faceindices.push_back(&f);
			m->faces.push_back(f);
		}
	}
	for (int i = 0; i < m->faces.size(); i++){
		GLushort a = m->faces[i].verts[0];
		GLushort b = m->faces[i].verts[1];
		GLushort c = m->faces[i].verts[2];

		glm::vec3 normal = glm::cross(
			glm::vec3(m->vertices[b].pts) - glm::vec3(m->vertices[a].pts),
			glm::vec3(m->vertices[c].pts) - glm::vec3(m->vertices[a].pts)
			);
		m->faces[i].normal = normal;
	}
	cout << "num of vertices: " << m->vertices.size() << endl;
	cout << "num of faces: " << m->faces.size() << endl;
	DWORD t2 = GetTickCount();
	cout << "load time: " << t2-t1 << endl;

}

void displayLoadedMesh(ProxyMesh *m, float magn, Colorf color){//vector<glm::vec4> &vertices, vector<glm::vec3> &normals, vector<GLushort> &faces, float magn){
	color.callGlColor3f();
	//glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < m->faces.size(); i++){
		GLushort a = m->faces[i].verts[0];
		GLushort b = m->faces[i].verts[1];
		GLushort c = m->faces[i].verts[2];

		//cout << a << " " << b << " " << c << endl;
		if (!m->faces[i].touched){
			glm::vec3 v = m->vertices[a].pts;
			glVertex3f(v[0], v[1], v[2]);
			v = m->vertices[b].pts;
			glVertex3f(v[0], v[1], v[2]);
			v = m->vertices[c].pts;
			glVertex3f(v[0], v[1], v[2]);
		}

	}
	for (int i = 0; i < sculpts.size(); i++){
		int num = sculpts[i].faces.size();
		glm::vec3 lastv;
		for (int j = 0; j < num ; j++){
			Face *f = sculpts[i].faces[j];
			GLushort a = f->verts[0];
			GLushort b = f->verts[1];
			GLushort c = f->verts[2];
			
			float scaler = (j==0||j==num-1)?.3:.5;
			glm::vec3 v1 = m->vertices[a].pts;
			glm::vec3 v2 = m->vertices[b].pts;
			glm::vec3 v3 = m->vertices[c].pts;

			glm::vec3 _cp((v1[0] + v2[0] + v3[0]) / 3, (v1[1] + v2[1] + v3[1]) / 3, (v1[2] + v2[2] + v3[2]) / 3);
			glm::vec3 nn = normalize(f->normal);
			glm::vec3 cp(_cp[0] + _levelmod*scaler*nn[0], _cp[1] + _levelmod*scaler*nn[1], _cp[2] + _levelmod*scaler*nn[2]);
			
			

			
			
			if (j != 0){
				if (j % 2 == 1){
					glVertex3f(v1[0], v1[1], v1[2]);
					glVertex3f(cp[0], cp[1], cp[2]);
					glVertex3f(lastv[0], lastv[1], lastv[2]);
					
					glVertex3f(v2[0], v2[1], v2[2]);
					glVertex3f(lastv[0], lastv[1], lastv[2]);
					glVertex3f(cp[0], cp[1], cp[2]);
					
				}
				else{

					glVertex3f(v2[0], v2[1], v2[2]);
					glVertex3f(cp[0], cp[1], cp[2]);
					glVertex3f(lastv[0], lastv[1], lastv[2]);
					
					glVertex3f(v3[0], v3[1], v3[2]);
					glVertex3f(lastv[0], lastv[1], lastv[2]);
					glVertex3f(cp[0], cp[1], cp[2]);
					
				}

			}


			
				glVertex3f(v1[0], v1[1], v1[2]);
				glVertex3f(v2[0], v2[1], v2[2]);
				glVertex3f(cp[0], cp[1], cp[2]);

				glVertex3f(v1[0], v1[1], v1[2]);
				glVertex3f(cp[0], cp[1], cp[2]);
				glVertex3f(v3[0], v3[1], v3[2]);

				glVertex3f(cp[0], cp[1], cp[2]);
				glVertex3f(v2[0], v2[1], v2[2]);
				glVertex3f(v3[0], v3[1], v3[2]);






			lastv = cp;

		}
			
	}
	glEnd();
}

glm::vec3 getCurrCamPos(){
	GLdouble mv[16], pm[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	glGetDoublev(GL_PROJECTION_MATRIX, pm);
	GLdouble a = 0.0, b = 0.0, c = 0.0;
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluUnProject((viewport[2] - viewport[0]) / 2, (viewport[3] - viewport[1]) / 2, 0.0, mv, pm, viewport, &a, &b, &c);

	return glm::vec3(a, b, c);
}

glm::vec3 getViewVector(double x, double y){
	//cout << x << "\t" << y << endl;
	GLdouble mv[16], pm[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	glGetDoublev(GL_PROJECTION_MATRIX, pm);
	GLdouble a = 0.0, b = 0.0, c = 0.0, d = 0.0, e = 0.0, f = 0.0;

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluUnProject(x, viewport[3] - y, 0.0, mv, pm, viewport, &a, &b, &c);
	gluUnProject(x, viewport[3] - y, 0.1, mv, pm, viewport, &d, &e, &f);

	glm::vec3 v(d - a, e - b, f - c);
	v = glm::normalize(v);
	//cout << v.x << "\t" << v.y << "\t" << v.z << endl;	
	return v;

}
faceInters intersectFace(glm::vec3 p, glm::vec3 d, ProxyMesh * m,Face * face){
	DWORD t1 = GetTickCount();
	faceInters fi;
	fi.t = -1;
	glm::vec3 nn = face->normal;
	float nnorm = sqrt(fabs(nn.x * nn.x + nn.y * nn.y + nn.z * nn.z));
	glm::vec3 n = nn / nnorm;
	glm::vec3 vp = m->vertices[face->verts[0]].pts - p;
	float vpn = glm::dot(vp, n);
	
	if (vpn >= 0) return fi;

	//2. compute the intersection between the ray and the plane determined by the triangle	

	float dn = glm::dot(d, n);
	float t = vpn / dn;

	if (t < 0) return fi;

	glm::vec3 intP = p + t*d;

	double area = glm::dot(nn,n);
	area *= .5;

	double a[3];
	for (int i = 0; i < 3; i++)
	{
		glm::vec3 a1 = m->vertices[face->verts[(i + 1) % 3]].pts - intP;
		glm::vec3 a2 = m->vertices[face->verts[(i + 2) % 3]].pts - intP;
		glm::vec3 crossa = glm::cross(a1, a2);
		a[i] = (glm::dot(crossa,n))/ (2 * area);
		if (a[i] < 0) return fi;
	}

	
	fi.t = vpn / dn;
	fi.norm = &(nn);
	fi.face = face;
	//cout << "T " << t << endl;
	return fi; //t;

};

faceInters getT(glm::vec3 CameraLoc, glm::vec3 Dir, ProxyMesh *m){
	//DWORD t1 = GetTickCount();
	float t = FLT_MAX;
	bool found = false;
	faceInters f;
	f.t = 0;
	for (int i = 0; i < m->faces.size(); i++){
		faceInters fi = intersectFace(CameraLoc, Dir, m, &(m->faces[i]));
		float x = fi.t;
		//cout << "t? " << t << endl;
		if (x > 0 && x < t){
			found = true;
			f = fi;
			t = x;
		}
	}
	//DWORD t2 = GetTickCount();
	//cout << "load time: " << t2 - t1 << endl;
	return f;//(found)?t:0;
}
void optimize(Stroke *s, ProxyMesh *m, int mode, float x, float y){
	//params: stroke, screen coordinates, weights for optimizing
	glm::vec3 camv = getCurrCamPos();
	glm::vec3 dir = getViewVector(x, y);
	float t_p, t = 0;
	faceInters f;
	if (mode == 0 || s->vertices.size()==0){
		f = getT(camv, dir, m);
		t_p = f.t; //modify this to return face normal
		if (t_p <= 0) return;
		float leveladd = 0;
		if (f.face->touched){		
			for (int i = 0; i < sculpts.size();i++){// f.face->sculpts.size(); i++){
				leveladd += sculpts[i].getSculptLevel(glm::vec3(camv.x + dir.x*t_p, camv.y + dir.y*t_p, camv.z + dir.z*t_p));
			}
		}		
		t = t_p - ((level + leveladd)*_levelmod); //init value
		if (s->vertices.size() == 0){
			s->initT = t;
			s->facenormdir = *f.norm;
		}
	}
	else if (mode == 1){
		float size = s->vertices.size();
		s->initT -= size*(size < 5) ? (0.01*(6 - size)) : (0.01);
		t = s->initT;
		//temp point
		//face normal direction
	}
	else if(mode == 2){
		float size = s->vertices.size();
		s->initT -= size*(size < 5) ? (0.01*(size/3)) : (0.015);
		t = s->initT;
		//temp point
		//tangent direction
	}	
	//if (t > 0) s->addVertex(glm::vec3(camv.x + v.x*t, camv.y + v.y*t, camv.z + v.z*t));
	int _k = 20;
	int _m = 10;

	float w_al, w_ang, w_ls;
	switch (mode){
	case 0:
		w_ls = 1;
		w_ang = 0.1;
		w_al = 0;
		break;
	case 1:
	case 2:
		w_ls = (s->vertices.size() == 0)?1:0;
		w_ang = 1;
		w_al = 0.05;
		break;
	default:
		w_ls = 1;
		w_ang = 0.1;
		w_al = 0;
		break;
	}



//at the end, insert the optimized value into the stroke
	s->addVertex(glm::vec3(camv.x + dir.x*t, camv.y + dir.y*t, camv.z + dir.z*t));
}

void sculpt(SculptStroke *s, ProxyMesh *m, int mode, float x, float y){
	//params: stroke, screen coordinates, weights for optimizing
	glm::vec3 camv = getCurrCamPos();
	glm::vec3 dir = getViewVector(x, y);

	faceInters f;
	f = getT(camv, dir, m);
	float t = f.t; //modify this to return face normal
	if (t <= 0) return;
	f.face->touched = true;

	s->addVertex(glm::vec3(camv.x + dir.x*t, camv.y + dir.y*t, camv.z + dir.z*t));

	bool hasface = false;
	for (int i = 0; i < s->faces.size(); i++){
		if (s->faces[i] == f.face){
			hasface = true;
			break;
		}
	}
	if (!hasface){
		s->faces.push_back(f.face);
		f.face->sculpts.push_back(s);
	}

}

float gradient_arcl(Stroke *s, glm::vec3 camv, glm::vec3 dir, float t){
	int i = s->vertices.size() - 1;
	return pow(camv.x + t*dir.x - s->vertices[i].x, 2) +
		pow(camv.y + t*dir.y - s->vertices[i].y, 2) +
		pow(camv.z + t*dir.z - s->vertices[i].z, 2);
}
float ArcLength(Stroke *s, glm::vec3 camv, glm::vec3 dir, float t){
	float sum = 0;
	int i = 0;
	for (i = 1; i < s->vertices.size(); i++){
		sum += pow(s->vertices[i].x - s->vertices[i - 1].x, 2) +
			pow(s->vertices[i].y - s->vertices[i - 1].y, 2) +
			pow(s->vertices[i].z - s->vertices[i - 1].z, 2);
	}
	sum += pow(camv.x+t*dir.x - s->vertices[i].x, 2) +
		pow(camv.y + t*dir.y - s->vertices[i].y, 2) +
		pow(camv.z + t*dir.z - s->vertices[i].z, 2);
	return sum;
}



float getAng(glm::vec3 a, glm::vec3 b, glm::vec3 c){
	glm::vec3 ab = a - b;
	glm::vec3 bc = b - c;
	return glm::dot(normalize(ab), normalize(bc));
}

float gradient_Ang(Stroke *s, glm::vec3 camv, glm::vec3 dir, float t){
	int i = s->vertices.size() - 3;
	float diff = pow(1 - getAng(s->vertices[i + 2], s->vertices[i + 1], s->vertices[i]), 2);
	diff = pow(1 - getAng(camv + t*dir, s->vertices[i + 2], s->vertices[i + 1]), 2) - diff;
	return diff;
}

float Angle(Stroke *s, glm::vec3 camv, glm::vec3 dir, float t){
	float sum = 0;
	int ncap = s->vertices.size();
	for (int i = 0; i < ncap - 2; i++){
		if (i < ncap - 3){
			sum += pow(1 - getAng(s->vertices[i + 2], s->vertices[i + 1], s->vertices[i]), 2);
		}
		else{
			sum += pow(1 - getAng(camv + t*dir, s->vertices[i + 1], s->vertices[i]), 2);
		}
	}
	return sum;
}

float f_x(float t_p, float t){
	return (t_p - t) / _levelmod;
}

float grad_LD(Stroke *s, float t_p, float t){
	float diff = pow(s->fxvals[s->fxvals.size()-1]-level, 2);
	return pow(f_x(t_p, t) - level, 2) - diff;
	//return (-2/_levelmod)*(f_x(t_p, t) - level);
}

float LevelDistance(Stroke *s, glm::vec3 camv, glm::vec3 dir,float t_p, float t){
	float sum = 0;
	for (int i = 0; i < s->fxvals.size(); i++){
		sum += pow(s->fxvals[i]-level,2);
	}
	return sum + pow(f_x(t_p, t) - level, 2);
}

#endif // CALC_H