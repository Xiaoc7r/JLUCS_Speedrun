#pragma once
class POINT3
{

public:
	static POINT perspectiveProjection1P2D(POINT3 p, POINT3 centre);
	static POINT3 perspectiveProjection1P(POINT3 p, POINT3 centre);
	static POINT3 rotateZ(POINT3 p, double radians);
	static POINT3 rotateY(POINT3 p, double radians);
	static POINT3 rotateX(POINT3 p, double radians);
	static POINT3 translate(POINT3 p, double dx, double dy, double dz);

public:
	POINT3();
	POINT3(double x, double y, double z);
	~POINT3();


private:
	static void mul(const double op[1][4], const double trans[4][4], double rp[1][4]);

public:
	double z;
	double y;
	double x;

};

typedef POINT3 vec3;