#include "pch.h"
#include "Cube.h"


const vec3 Cube::DEF = vec3(0, 0, -1200);
const size_t Cube::DEFsurfaces[6][4] = { { 0, 1, 2, 3 }, { 4, 5, 6, 7 }, { 0, 3, 7, 4 }, { 1, 2, 6, 5 }, { 0, 1, 5, 4 }, { 2, 3, 7, 6 } };

const POINT3 Cube::DEFvertices[8] = {
	POINT3(1, 0, 0), POINT3(1, 1, 0), POINT3(0, 1, 0), POINT3(0, 0, 0),
	POINT3(1, 0, 1), POINT3(1, 1, 1), POINT3(0, 1, 1), POINT3(0, 0, 1)
};
/*const POINT3 Cube::DEFvertices[8] = {
POINT3(0, 0, 0),POINT3(1, 0, 0),POINT3(1, 0, -1),POINT3(0, 0, -1),
POINT3(0, 1, 0),POINT3(1, 1, 0),POINT3(1, 1, -1),POINT3(0, 1, -1)
};*/

Cube::Cube()
{
	POINT3 np[8];
	int i;
	for (i = 0; i < 8; ++i) {
		np[i] = POINT3(Cube::DEFvertices[i].x * 200, Cube::DEFvertices[i].y * 200, (Cube::DEFvertices[i].z - 1) * 200);
	}
	for (i = 0; i < 8; ++i) {
		np[i] = POINT3(np[i].x + 100, np[i].y + 100, np[i].z);
	}
	this->init(np);
	//this->init();
}

Cube::~Cube()
{

}

void Cube::setVertices(const POINT3 verts[])
{
	for (int i = 0; i < 8; ++i) {
		this->vertices[i] = verts[i];
	}
}

void Cube::translate(Dir dir, double steps)
{
	for (size_t i = 0; i < 8; ++i)
	{
		this->vertices[i] =
			POINT3::translate(this->vertices[i], ((dir & Cube::X) == 0 ? 0 : steps), ((dir & Cube::Y) == 0 ? 0 : steps), ((dir & Cube::Z) == 0 ? 0 : steps));
	}
}

void Cube::rotate(Axis axis, double radians)
{
	size_t i;
	switch (axis)
	{
	case Cube::X:
		for (i = 0; i < 8; ++i)
		{
			this->vertices[i] = POINT3::rotateX(this->vertices[i], radians);
		}
		break;
	case Cube::Y:
		for (i = 0; i < 8; ++i)
		{
			this->vertices[i] = POINT3::rotateY(this->vertices[i], radians);
		}
		break;
	case Cube::Z:
		for (i = 0; i < 8; ++i)
		{
			this->vertices[i] = POINT3::rotateZ(this->vertices[i], radians);
		}
		break;
	default:
		break;
	}
}

void Cube::setSurfaces(const size_t surfaces[][4])
{
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 4; ++j) {
			this->surfaces[i][j] = surfaces[i][j];
		}
	}
}

void Cube::reset()
{
	POINT3 np[8];
	int i;
	for (i = 0; i < 8; ++i) {
		np[i] = POINT3(Cube::DEFvertices[i].x * 200, Cube::DEFvertices[i].y * 200, (Cube::DEFvertices[i].z - 1) * 200);
	}
	for (i = 0; i < 8; ++i) {
		np[i] = POINT3(np[i].x + 100, np[i].y + 100, np[i].z);
	}
	this->init(np);
}

void Cube::setViewPoint(const POINT3 dir)
{
	this->viewPoint = dir;
}

void Cube::init(const POINT3 vertices[], const POINT3 viewPoint, const size_t surfaces[][4])
{
	memset(this->canSee, 0, sizeof(bool) * 6);
	memset(this->canSeeVec, 0, sizeof(bool) * 12);
	this->setVertices(vertices);
	this->setSurfaces(surfaces);
	this->setViewPoint(viewPoint);
}


// 比例变换
void Cube::scale(double factor)
{
	// 先计算立方体中心
	double cx = 0, cy = 0, cz = 0;
	for (int i = 0; i < 8; i++) {
		cx += vertices[i].x;
		cy += vertices[i].y;
		cz += vertices[i].z;
	}
	cx /= 8; cy /= 8; cz /= 8;

	// 以中心为基准进行缩放
	for (int i = 0; i < 8; i++) {
		vertices[i].x = cx + (vertices[i].x - cx) * factor;
		vertices[i].y = cy + (vertices[i].y - cy) * factor;
		vertices[i].z = cz + (vertices[i].z - cz) * factor;
	}
}