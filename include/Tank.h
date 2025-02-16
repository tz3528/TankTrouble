#ifndef TANK_H
#define TANK_H

#include "Object.h"

#include <windows.h>
#include <list>
#include <memory>
#include <chrono>

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

        bool CollisionWall();

        int getId() const;
        int getController() const;
        void setColor(COLORREF color);
        point body[4], barrel[4];
        bool isForward = false, isBackward = false, isLeft = false, isRight = false;
        bool isAttack = false;

    private:
        int id, size, bullets = 15;
        int controller;
        double movingStep, length, width;
        point tmpPosition, tmpDirection;
        const int attackInterval = 100;
        std::chrono::steady_clock::time_point lastAttack;
    };

    extern std::list<std::shared_ptr<Tank>> TankPool;

    void TankControl();
    void PlayerControl(Tank* Player);
    void ComputerControl(Tank* Computer);
}

#endif // TANK_H
