#ifndef POINT_H
#define POINT_H

#include <math.h>

namespace TankTrouble
{
    const double eps = 1e-2;
    const double pi = acos(-1);

    class point
    {
    public:
        point() : x(0), y(0) {}

        template<typename T, typename U>
        point(T x, U y) : x(static_cast<double>(x)), y(static_cast<double>(y)) {}

        ~point(); // 添加析构函数声明

        //向量的四则运算
        point operator+(const point& other) const;
        point operator-(const point& other) const;
        point operator*(const double& other) const;
        point operator/(const double& other) const;

        bool operator==(const point& other) const;

        //向量的点积和叉积
        double operator*(const point& other) const;
        double operator^(const point& other) const;

        int quad();
        point normalVector() const;

        double x, y;
    };

    //有向线段，其中u为起点，v为终点
    struct Segment {
        point u, v;
    };
    //射线，其中origin为起点，direction为方向向量
    struct Ray {
        point origin, direction;
    };

    double distPointPoint(const point u, const point v);
    double norm(const point u);

    POINT* toPOINT(const point* now, const int size);

    //点、线运算

    //计算点集中x坐标的最小值
    double getMinX(const point* points, int n);
    //计算点集中y坐标的最小值
    double getMinY(const point* points, int n);
    //计算点集中x坐标的最大值
    double getMaxX(const point* points, int n);
    //计算点集中y坐标的最大值
    double getMaxY(const point* points, int n);
    //计算点u到现段vw的距离
    double distPointSeg(const point u, const point v, const point w);
    //判断线段ab与线段cd是否相交
    bool itsSegSeg(const point a, const point b, const point c, const point d);
    //判断两个凸包是否相交
    bool itsPolPol(const point* a, const int na, const point* b, const int nb);

    class line
    {
    public:
        line(point p,point q);
        //两点式
        point u, v;
        //起点和方向向量决定的直线
        point origin, direction;
        //直线的一般方程
        double A, B, C;
    };

    //直线的计算

    //求两直线的交点
    point itsLineLine(line l1, line l2);
}

#endif
