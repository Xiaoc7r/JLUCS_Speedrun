#pragma once
#include "POINT3.h"
typedef int Dir;
typedef Dir Axis;

class Cube
{
public:
	const static POINT3 DEF;
	const static size_t DEFsurfaces[6][4];
	const static POINT3 DEFvertices[8];
	const static Dir X = 1;
	const static Dir Y = 2;
	const static Dir Z = 4;

public:
	void reset();
	void init(const POINT3 vertices[8] = Cube::DEFvertices, const POINT3 viewPoint = Cube::DEF, const size_t surfaces[6][4] = Cube::DEFsurfaces);
	void setViewPoint(const POINT3 dir);
	void setSurfaces(const size_t surfaces[6][4]);
	void setVertices(const POINT3 verts[]);
	void translate(Dir dir, double steps);
	void rotate(Axis axis, double radians);
	Cube();
	~Cube();

public:
	bool canSeeVec[12];
	bool canSee[6];
	POINT3 viewPoint;
	vec3 viewDirs[6];

	size_t surfaces[6][4];
	size_t edges[12][2];
	POINT3 vertices[8];
};

