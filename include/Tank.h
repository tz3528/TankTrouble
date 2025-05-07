#ifndef TANK_H
#define TANK_H

#include "Object.h"
#include "Map.h"
#include "Window.h"
#include "bullet.h"
#include "pch.h"

#include <thread>
#include <chrono>

constexpr int PLAYER = 0;
constexpr int COMPUTER = 1;

namespace TankTrouble
{
    class Tank :public Object
    {
    public:
        Tank() = default;
        Tank(int id, int controller, point position, point direction, int size, const COLORREF& color);
        ~Tank();
		/**
         * @brief 用于更新坦克本体及炮管四角的坐标
         */
        void getTank();
		/**
         * @brief 坦克向前移动
         */
        void forward();
		/**
         * @brief 坦克向后移动
         */
        void backwards();
		/**
         * @brief 坦克逆时针旋转
         */
        void left();
		/**
         * @brief 坦克顺时针旋转
         */
        void right();
        /**
         * @brief 坦克发射子弹
         */
        void attack();
        /**
         * @brief 坦克回收子弹
         */
        void addbullet();
        /**
         * @brief           绘制坦克
         * @param hdcMem    画布
         */
        void draw(HDC hdcMem) override;
        /**
         * @brief   用于判断坦克是否与地图或其它坦克发生碰撞
         * @return  若存在碰撞则返回true，否则返回false
         */
        bool Collision() const;

        int getId() const;
        /**
         * @brief   用于获取坦克的操控者
         * @return  坦克的操控者
         */
        int getController() const;
        /**
         * @brief   用于获取当前坦克所在坐标
         * @return  网格坐标
         */
        point getposition() const;
        /**
         * @brief 更新坦克所在的网格坐标
         */
        void getGridPosition();
        void setColor(COLORREF color);

        point body[4], barrel[4];
        bool isForward = false, isBackward = false, isLeft = false, isRight = false;
        bool isAttack = false;
        GridPosition pos;
        bool isLife = true;

    private:
        int id, size;
        std::atomic<int> bullets = 10;
        int controller;
        double movingStep, length, width;
        point tmpPosition, tmpDirection;
        // 攻击间隔
        const int attackInterval = 200;

        std::chrono::steady_clock::time_point lastAttack;
    };

    static int RotationRatio = 20;
    static point directions[4] = {
        point(1, 0), point(0, 1), point(-1, 0), point(0, -1)
    };
	/**
     * @brief 坦克池，存储所有坦克对象的指针
     */
    extern list<std::shared_ptr<Tank>> TankPool;
	/**
     * @brief 坦克池的互斥锁
     */
    extern std::mutex  tpMutex;

    int TankControl();
}

#endif // TANK_H
