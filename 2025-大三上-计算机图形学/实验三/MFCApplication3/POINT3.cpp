#include "pch.h"
#include "POINT3.h"


POINT3::POINT3()
{
}


POINT3::~POINT3()
{
}


POINT3::POINT3(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

POINT3 POINT3::translate(POINT3 p, double dx, double dy, double dz)
{
	double op[1][4] = { { p.x, p.y, p.z, 1 } };
	double T[4][4] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ dx, dy, dz, 1 }
	};
	double rp[1][4];
	POINT3::mul(op, T, rp);
	return POINT3(rp[0][0], rp[0][1], rp[0][2]);
}

void POINT3::mul(const double op[1][4], const double trans[4][4], double rp[1][4])
{
	double ** a = (double **)op;
	double **b = (double **)trans;
	double **target = (double **)rp;
	size_t tar_cols = 4;
	size_t tar_rows = 1;
	size_t a_cols = 4;

	size_t m = a_cols, n = tar_rows, p = tar_cols;
	for (int i = 0; i < tar_rows; ++i) {
		for (int j = 0; j < tar_cols; ++j) {
			double sum = 0;
			for (int k = 0; k < m; ++k) {
				double aik = *(((double*)a) + i * m + k);
				double bkj = *(((double*)b) + k * p + j);
				sum += aik * bkj;
			}
			*(((double*)target) + i * tar_cols + j) = sum;
		}
	}

}

POINT3 POINT3::rotateX(POINT3 p, double radians)
{
	double cosr = cos(radians);
	double sinr = sin(radians);
	double op[1][4] = { { p.x, p.y, p.z, 1 } };
	double T[4][4] = {
		{ 1, 0, 0, 0 },
		{ 0, cosr, sinr, 0 },
		{ 0, -sinr, cosr, 0 },
		{ 0, 0, 0, 1 }
	};
	double rp[1][4];
	POINT3::mul(op, T, rp);
	return POINT3(rp[0][0], rp[0][1], rp[0][2]);
}

POINT3 POINT3::rotateY(POINT3 p, double radians)
{
	double cosr = cos(radians);
	double sinr = sin(radians);
	double op[1][4] = { { p.x, p.y, p.z, 1 } };
	double T[4][4] = {
		{ cosr, 0, -sinr, 0 },
		{ 0, 1, 0, 0 },
		{ sinr, 0, cosr, 0 },
		{ 0, 0, 0, 1 }
	};
	double rp[1][4];
	POINT3::mul(op, T, rp);
	return POINT3(rp[0][0], rp[0][1], rp[0][2]);
}

POINT3 POINT3::rotateZ(POINT3 p, double radians)
{
	double cosr = cos(radians);
	double sinr = sin(radians);
	double op[1][4] = { { p.x, p.y, p.z, 1 } };
	double T[4][4] = {
		{ cosr, sinr, 0, 0 },
		{ -sinr, cosr, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	double rp[1][4];
	POINT3::mul(op, T, rp);
	return POINT3(rp[0][0], rp[0][1], rp[0][2]);
}

POINT3 POINT3::perspectiveProjection1P(POINT3 p, POINT3 centre)
{
	double op[1][4] = { { p.x, p.y, p.z, 1 } };
	double T[4][4] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ -(centre.x) / (centre.z), -(centre.y) / (centre.z), 0, -1 / (centre.z) },
		{ 0, 0, 0, 1 }
	};
	double rp[1][4];
	POINT3::mul(op, T, rp);
	return POINT3(rp[0][0] / rp[0][3], rp[0][1] / rp[0][3], rp[0][2] / rp[0][3]);
}

POINT POINT3::perspectiveProjection1P2D(POINT3 p, POINT3 centre)
{
	POINT3 rp = POINT3::perspectiveProjection1P(p, centre);
	POINT res = { (long)rp.x, (long)rp.y };
	return res;
}

POINT POINT3::obliqueProjection2D(POINT3 p)
{
	POINT pt;
	double k = 0.5;  // 斜二测系数
	double angle = 3.1415926 / 4.0;  // 45度

	// 斜二测投影公式
	pt.x = (LONG)(p.x + p.z * cos(angle) * k);
	pt.y = (LONG)(p.y + p.z * sin(angle) * k);

	return pt;
}

// 比例变换
POINT3 POINT3::scale(POINT3 p, double sx, double sy, double sz)
{
	POINT3 result;
	result.x = p.x * sx;
	result.y = p.y * sy;
	result.z = p.z * sz;
	return result;
}