#ifndef BULLET_H
#define BULLET_H

#include "Object.h"
#include "pch.h"

#include <mutex>
#include <list>
#include <memory>

constexpr double SMALL_MAP_STEP = 4;
constexpr double MID_MAP_STEP = 2;
constexpr double BIG_MAP_STEP = 1;

using std::list;

namespace TankTrouble
{

    class bullet : public Object, public std::enable_shared_from_this<bullet>
    {
    public:

        bullet(int id, int controller, point position, point direction, int size, const COLORREF& color);
        ~bullet();

        int getLife() const;
        int getController() const;
        int getId() const;
        void getTrack();

        void draw(HDC hdcMem) override;
        void move();

        Segment TrackSegment;

    private:
        int id, controller, life;
        double radius, movingStep;
    };

    int bulletPoolUpdate();

    extern list<std::shared_ptr<bullet>> bulletPool;
    extern shared_mutex bpMutex;
};

#endif // BULLET_H
