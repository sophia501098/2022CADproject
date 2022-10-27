#pragma once
#include<iostream>
#include<vector>
#include "include/GL/glew.h"
#include "include/glm/glm.hpp"
class Point;
class Vertex;
class HalfEdge;
class Edge;
class Loop;
class Face;
class Solid;

class Point {
private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat r;
	GLfloat g;
	GLfloat b;
public:
	Vertex* v;
	Point(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
		v = NULL;
	}
	void SetColor(float _r,float _g,float _b) {
		r = _r;
		g = _g;
		b = _b;
	}
	void SetPos(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	glm::vec3 GetPos() {
		 glm::vec3 pos(x, y, z);
		 return pos;
	}
	glm::vec3 GetColor() {
		glm::vec3 color(r, g, b);
		return color;
	}
};

class Vertex {
public:
	Point* pointPtr;
	Vertex* pre_v;
	Vertex* nxt_v;
	HalfEdge* vhe;
public:
	Vertex(Point* _pointPtr) {
		pointPtr = _pointPtr;
		pointPtr->v = this;
		pre_v = NULL;
		nxt_v = NULL;
		vhe = NULL;
	}
};

class HalfEdge {
public:
	Vertex* startv;
	Vertex* endv;
	HalfEdge* pre_he;
	HalfEdge* nxt_he;
	Edge* e;
	Loop* wloop;
};
class Edge {
public:
	HalfEdge* he1;
	HalfEdge* he2;
};
class Loop
{
public:
	Loop* pre_l;
	Loop* nxt_l;
	HalfEdge* ledg;
	Face* lface;
	bool isInnerLoop;
	int GetLength()
	{
		if (ledg == NULL)
		{
			return 0;
		}
		else
		{
			int count = 0;
			HalfEdge* startHe = ledg;
			HalfEdge* he = ledg;
			do
			{
				count++;
				he = he->nxt_he;
			} while (he != NULL && he != startHe);
			return count;
		}
	}
};

class Face {
public:
	Face* pre_f;
	Face* nxt_f;
	Loop* floop;
	Solid* fsolid;
	bool hasInnerLoop = false;
};

class Solid {
public:
	Solid* pre_s;
	Solid* nxt_s;
	Face* sface;
	Edge* e;
	Solid()
	{
		pre_s = NULL;
		nxt_s = NULL;
		sface = NULL;
		e = NULL;
	}
};