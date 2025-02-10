#ifndef BULLET_H
#define BULLET_H

#include "Object.h"

#include <mutex>
#include <list>
#include <memory>

constexpr double SMALL_MAP_STEP = 2;
constexpr double MID_MAP_STEP = 1.5;
constexpr double BIG_MAP_STEP = 1;

namespace TankTrouble
{
    extern std::mutex bpMutex;
    extern std::condition_variable bpCv;

    class bullet : public Object, public std::enable_shared_from_this<bullet>
    {
    public:

        bullet(int id, int controller, point position, point direction, int size, const COLORREF& color);
        ~bullet();

        int getLife() const;
        int getController() const;
        int getId() const;

        void draw(HDC hdcMem) override;
        void move();


    private:
        int id, controller, life;
        double radius, movingStep;
    };

    void bulletPoolUpdate();

    extern std::list<std::shared_ptr<bullet>> bulletPool;

};

#endif // BULLET_H
