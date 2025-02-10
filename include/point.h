#ifndef POINT_H
#define POINT_H

#include <windows.h>

namespace TankTrouble
{
    const double eps = 1e-8;

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

        point normalVector() const;

        double x, y;
    };

    double distPointPoint(const point u, const point v);
    double norm(const point u);

    POINT* toPOINT(const point* now, const int size);

    //点、线运算

    //计算点集中x坐标的最小值
    long getMinX(const point* points, int n);
    //计算点集中y坐标的最小值
    long getMinY(const point* points, int n);
    //计算点集中x坐标的最大值
    long getMaxX(const point* points, int n);
    //计算点集中y坐标的最大值
    long getMaxY(const point* points, int n);
    //计算点u到现段vw的距离
    double distPointSeg(const point u, const point v, const point w);
    //判断线段ab与线段cd是否相交
    bool IntersectSegSeg(const point a, const point b, const point c, const point d);

}

#endif

