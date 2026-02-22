#pragma once

class POINT3
{
public:
	// 透视投影（一点透视）
	static POINT perspectiveProjection1P2D(POINT3 p, POINT3 centre);
	static POINT3 perspectiveProjection1P(POINT3 p, POINT3 centre);

	// 斜二测投影（新增）
	static POINT obliqueProjection2D(POINT3 p);

	// 旋转变换
	static POINT3 rotateZ(POINT3 p, double radians);
	static POINT3 rotateY(POINT3 p, double radians);
	static POINT3 rotateX(POINT3 p, double radians);

	// 平移变换
	static POINT3 translate(POINT3 p, double dx, double dy, double dz);

	// 比例变换（新增）
	static POINT3 scale(POINT3 p, double sx, double sy, double sz);

public:
	POINT3();
	POINT3(double x, double y, double z);
	~POINT3();

	static void mul(const double op[1][4], const double trans[4][4], double rp[1][4]);

public:
	double x;
	double y;
	double z;
};

typedef POINT3 vec3;

