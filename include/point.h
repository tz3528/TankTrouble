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

        ~point(); // ���������������

        //��������������
        point operator+(const point& other) const;
        point operator-(const point& other) const;
        point operator*(const double& other) const;
        point operator/(const double& other) const;

        bool operator==(const point& other) const;

        //�����ĵ���Ͳ��
        double operator*(const point& other) const;
        double operator^(const point& other) const;

        point normalVector() const;

        double x, y;
    };

    double distPointPoint(const point u, const point v);
    double norm(const point u);

    POINT* toPOINT(const point* now, const int size);

    //�㡢������

    //����㼯��x�������Сֵ
    long getMinX(const point* points, int n);
    //����㼯��y�������Сֵ
    long getMinY(const point* points, int n);
    //����㼯��x��������ֵ
    long getMaxX(const point* points, int n);
    //����㼯��y��������ֵ
    long getMaxY(const point* points, int n);
    //�����u���ֶ�vw�ľ���
    double distPointSeg(const point u, const point v, const point w);
    //�ж��߶�ab���߶�cd�Ƿ��ཻ
    bool IntersectSegSeg(const point a, const point b, const point c, const point d);

}

#endif

