#ifndef TANK_H
#define TANK_H

#include "Object.h"
#include "Map.h"
#include "bullet.h"
#include "Window.h"
#include "geometry.h"
#include "pch.h"

#include <thread>
#include <chrono>
#include <windows.h>

constexpr int PLAYER = 0;
constexpr int COMPUTER = 1;

namespace TankTrouble
{
    class Tank :public Object
    {
    public:
        Tank(int id, int controller, point position, point direction, int size, const COLORREF& color);
        ~Tank();

        void getTank();

        void forward();
        void backwards();
        void left();
        void right();
        void attack();
        void addbullet();
        void draw(HDC hdcMem) override;

        bool Collision() const;

        int getId() const;
        int getController() const;
        point getposition() const;
        void getGridPosition();
        void setColor(COLORREF color);

        point body[4], barrel[4];
        bool isForward = false, isBackward = false, isLeft = false, isRight = false;
        bool isAttack = false;
        GridPosition pos;
        bool isLife = true;

    private:
        int id, size, bullets = 10;
        int controller;
        double movingStep, length, width;
        point tmpPosition, tmpDirection;
        const int attackInterval = 200;
       
        std::chrono::steady_clock::time_point lastAttack;
    };

    static int RotationRatio = 20;
    static point directions[4] = {
        point(1, 0), point(0, 1), point(-1, 0), point(0, -1)
    };

    extern list<std::shared_ptr<Tank>> TankPool;
    extern shared_mutex tpMutex;

    void TankControl();
}

#endif // TANK_H
