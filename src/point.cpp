#include "point.h"

#include <stdexcept>

using std::min, std::max;

namespace TankTrouble {

	point point::operator+(const point& other) const {
		return point(this->x + other.x, this->y + other.y);
	}

	point point::operator-(const point& other) const {
		return point(this->x - other.x, this->y - other.y);
	}

	point point::operator*(const double& other) const {
		return point(this->x * other, this->y * other);
	}

	point point::operator/(const double& other) const {
		if (other == 0)
			throw std::invalid_argument("Division by zero");
		return point(this->x / other, this->y / other);
	}

	double point::operator*(const point& other) const {
		return this->x * other.x + this->y * other.y;
	}

	double point::operator^(const point& other) const {
		return this->x * other.y - this->y * other.x;
	}

	bool point::operator ==(const point& other) const {
		return abs(this->x - other.x) < eps && abs(this->y - other.y) < eps;
	}

	point point::normalVector() const {
		return point(-this->y / norm(*this), this->x / norm(*this));
	}

	point::~point() {}

	double distPointPoint(const point u, const point v) {
		return sqrt((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y));
	}

	double norm(const point u) {
		return sqrt(u.x * u.x + u.y * u.y);
	}

	POINT* toPOINT(const point* now, const int size) {
		POINT* tmp = new POINT[size];
		for (int i = 0;i < size;i++) {
			tmp[i].x = (long)now[i].x;
			tmp[i].y = (long)now[i].y;
		}
		return tmp;
	}

	double getMinX(const point* points, int n) {
		double minx = points[0].x;
		for (int i = 1;i < n;i++) {
			minx = min<double>(minx, points[i].x);
		}
		return minx;
	}
	double getMinY(const point* points, int n) {
		double miny = points[0].y;
		for (int i = 1;i < n;i++) {
			miny = min<double>(miny, points[i].y);
		}
		return miny;
	}

	double getMaxX(const point* points, int n) {
		double maxx = points[0].x;
		for (int i = 1;i < n;i++) {
			maxx = max<double>(maxx, points[i].x);
		}
		return maxx;
	}

	double getMaxY(const point* points, int n) {
		double maxy = points[0].y;
		for (int i = 1;i < n;i++) {
			maxy = max<double>(maxy, points[i].y);
		}
		return maxy;
	}

	//计算点u到现段vw的距离
	double distPointSeg(const point u, const point v, const point w) {
		double d1 = (u.x - v.x) * (w.x - v.x) + (u.y - v.y) * (w.y - v.y);
		if (d1 <= 0.0) {
			return sqrt((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y));
		}
		double d2 = (v.x - w.x) * (v.x - w.x) + (v.y - w.y) * (v.y - w.y);
		if (d1 >= d2) return sqrt((u.x - w.x) * (u.x - w.x) + (u.y - w.y) * (u.y - w.y));
		double r = 1.0 * d1 / d2;
		double px = v.x + (w.x - v.x) * r;
		double py = v.y + (w.y - v.y) * r;
		return sqrt((u.x - px) * (u.x - px) + (u.y - py) * (u.y - py));
	}
	//判断线段ab与线段cd是否相交
	bool IntersectSegSeg(const point a, const point b, const point c, const point d) {
		point u = b - a;
		point v = d - c;
		if (((c - a) ^ u) * ((d - a) ^ u) <= 0 && ((a - c) ^ v) * ((b - c) ^ v) <= 0) return true;
		return false;
	}

}