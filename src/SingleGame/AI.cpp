#include "SingleGame/AI.h"
#include <cstdio>
typedef pair<int, int> PII;

using std::priority_queue,std::greater;

namespace TankTrouble
{

    void AIControl(shared_ptr<Tank> Computer){
        while (Running) {
            {
                unique_lock<shared_mutex> lock(tpMutex);
                if (Computer->isLife == false) {
                    break;
                }
                vector<int> path = JudgmentMove(Computer);
                JudgmentAttack(Computer, path);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    }

    vector<int> JudgmentMove(shared_ptr<Tank> Computer) {
        //方向初始化
        Computer->isBackward = false;
        Computer->isForward = false;
        Computer->isLeft = false;
        Computer->isRight = false;

        point dir(0, 0);
        //躲避的加权和
        for (auto& bullet : bulletPool) {
            double length = distPointSeg(Computer->getposition(),
                bullet->TrackSegment.u, bullet->TrackSegment.v);

            //如果距离为0，说明已经被击中，则线程应当退出
            if (length == 0) {
                return vector<int>();
            }
            dir = dir + bullet->direction.normalVector() * 100 / length;
        }

        //寻径加权和
        vector<int> dist(Row * Column + 1, 0x3f3f3f3f);
        vector<int> vis(Row * Column + 1), last(Row * Column + 1);
        //单源最短路
        auto dijsktra = [&](int s) {
            priority_queue<PII, vector<PII>, greater<PII>> q;
            dist[s] = 0;
            q.push({ 0, s });
            while (q.size()) {
                PII tmp = q.top();
                q.pop();
                int now = tmp.second;
                if (vis[now]) continue;
                vis[now] = 1;
                for (auto v : edge[now]) {
                    if (dist[v] > dist[now] + 1) {
                        last[v] = now;
                        dist[v] = dist[now] + 1;
                        q.push({ dist[v], v });
                    }
                }
            }
        };

        if (PtoN(Computer->pos)<= 0 || PtoN(Computer->pos) >= Row * Column) {
            return vector<int>();
        }

        dijsktra(PtoN(Computer->pos));

        GridPosition PlayerPosition = { 0, 0 };
        for (auto& Tank : TankPool) {
            if (Tank->getController() == PLAYER) {
                PlayerPosition = Tank->pos;
                break;
            }
        }
        //如果玩家已经死亡
        int now = PtoN(PlayerPosition);
        if (now <= 0 || now > Row * Column) {
            return vector<int>();
        }
        
        vector<int> path;
        while (now != PtoN(Computer->pos)) {
            path.emplace_back(now);
            now = last[now];
        }
        //求加权和
        point tmp = getCentrePoint(NtoP(path.back())) - Computer->getposition();
        dir = dir + tmp * path.size() / norm(tmp);
        
        /*通过点乘和叉积判断最终加权方向，
        * 分别得到前后和左右方向上的选择
        */
        if ((dir * Computer->direction) > eps) {
            Computer->isForward = true;
        }
        if ((dir * Computer->direction) < -eps) {
            Computer->isBackward = true;
        }
        if ((dir ^ Computer->direction) < -eps ) {
            Computer->isRight = true;
        }
        if ((dir ^ Computer->direction) > eps) {
            Computer->isLeft = true;
        }

        if (Computer->isBackward && !Computer->isLeft && !Computer->isRight) {
            Computer->isBackward = false;
            if (rand() % 2) {
                Computer->isLeft = true;
            }
            else {
                Computer->isRight = true;
            }
        }

        if (Computer->isBackward && Computer->isLeft) {
            Computer->isBackward = false;
        }
        if (Computer->isBackward && Computer->isRight) {
            Computer->isBackward = false;
        }

        return path;
    }

    void JudgmentAttack(shared_ptr<Tank> Computer, vector<int>path) {
        Computer->isAttack = false;
        for (auto& Tank : TankPool) {
            if (Tank->getController() == PLAYER) {
                if (path.size() <= 2) {
                    point tmp = Tank->position - Computer->position;
                    if (sqrt(2) * (Computer->direction * tmp)
                        >= norm(Computer->direction) * norm(tmp)) {
                        Computer->isAttack = true;
                    }
                }
                break;
            }
        }
    }

}
