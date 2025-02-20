#include "Map.h"
#include "Window.h"

#include <stdexcept>
#include <algorithm>
#include <vector>
#include <unordered_set>

using std::swap, std::invalid_argument;
using std::unordered_set,std::vector;
using std::uniform_real_distribution;
using std::make_shared;

namespace TankTrouble {
    
    list<shared_ptr<Wall>> WallPool;

    Wall::Wall(point u, point v, int size) {
        if (u == v) {
            throw invalid_argument("u and v cannot be the same point");
        }

        if (size == 1) {
            this->HalfWidth = 8;
        }
        if (size == 2) {
            this->HalfWidth = 4;
        }
        if (size == 3) {
            this->HalfWidth = 2;
        }
        if (u.x == v.x) {
            if (u.y > v.y) swap(u, v);
            LeftUp = u + point{ -HalfWidth, -HalfWidth };
            RightUp = u + point{ HalfWidth, -HalfWidth };
            LeftDown = v + point{ -HalfWidth, HalfWidth };
            RightDown = v + point{ HalfWidth, HalfWidth };
        }
        else if (u.y == v.y) {
            if (u.x > v.x) swap(u, v);
            LeftUp = u + point{ -HalfWidth, -HalfWidth };
            RightUp = v + point{ HalfWidth, -HalfWidth };
            LeftDown = u + point{ -HalfWidth, HalfWidth };
            RightDown = v + point{ HalfWidth, HalfWidth };
        }
        else {
            throw invalid_argument("u or v must have the same coordinate in one dimension.");
        }

    }

    Wall::~Wall() {
    }

    void Wall::draw(HDC hdcMem) {
        Rectangle(hdcMem, (int)LeftUp.x, (int)LeftUp.y, (int)RightDown.x, (int)RightDown.y);
    }

    int PtoN(position pos){
        return (pos.row - 1) * Column + pos.column;
    }

    position NtoP(int num){
        return {(num - 1) / Column + 1, (num - 1) % Column + 1};
    }

    int square(int x){
        return x*x;
    }

    void addWall(position a, position b){
        point u, v;
        if (a.row == b.row) {
            if (a.column > b.column) {
                swap(a, b);
            }
            u.x = LeftWall + a.column * xGap;
            u.y = UpWall + (a.row - 1) * yGap;
            v.x = LeftWall + a.column * xGap;
            v.y = UpWall + a.row * yGap;
        }
        if (a.column == b.column) {
            if (a.row > b.row) {
                swap(a, b);
            }
            u.x = LeftWall + (a.column - 1)* xGap;
            v.x = LeftWall + a.column * xGap;
            u.y = UpWall + a.row * yGap;
            v.y = UpWall + a.row * yGap;
        }
        WallPool.emplace_back(make_shared<Wall>(u, v, MapSize));
    }

    void GenerateMap(int mapSize){
        if (mapSize == SMALL_MAP) {
            Row = SMALL_ROW;
            Column = SMALL_COLUMN;

            beginNum = 4;

        }
        else if (mapSize == MEDIUM_MAP) {
            Row = MEDIUM_ROW;
            Column = MEDIUM_COLUMN;

            beginNum = 4;

        }
        else if (mapSize == LARGE_MAP) {
            Row = LARGE_ROW;
            Column = LARGE_COLUMN;

            beginNum = 4;

        }

        xGap = (RightWall - LeftWall) / Column;
        yGap = (BottomWall - UpWall) / Row;

        /*这里采用了int类型，是因为从网上搜索的的信息来看，
        * int在内存中是对齐的，所以访存的速度更快
        */
        int* d = new int[Row * Column + 1];
        //时间戳，用于保证没有小于某个长度的环
        int* dfn = new int[Row * Column + 1];
        memset(dfn, 0, sizeof(int) * (Row * Column + 1));
        int* vis = new int[Row * Column + 1];
        memset(vis, 0, sizeof(int) * (Row * Column + 1));
        int tmp[4], cnt=0;
        
        /*邻接的节点
        * 指针数组，每个指针指向一个数组，这个数组中存储了当前节点的相邻节点
        */
        unordered_set<int> set;
        vector<vector<int>>g(Row * Column + 1);
        for (int i = 1;i <= Row * Column;i++) {
            position pos = NtoP(i);
            cnt = 0;
            //存储相邻的节点
            if(pos.row > 1)       tmp[cnt++] = PtoN({ pos.row - 1, pos.column });
            if(pos.row < Row)     tmp[cnt++] = PtoN({ pos.row + 1, pos.column });
            if(pos.column > 1)       tmp[cnt++] = PtoN({ pos.row, pos.column - 1 });
            if(pos.column < Column)  tmp[cnt++] = PtoN({ pos.row, pos.column + 1 });
            //将这些节点随机排序，使得能够以随机顺序访问
            mt19937 f(rd());
            shuffle(tmp, tmp + cnt, f);
            d[i] = cnt;
            for (int j = 0;j < cnt;j++) {
                g[i].push_back(tmp[j]);
            }
        }

        /*这里连边的概率计算了两个点度的平方
        * 如果两个点都没连边，那么二者之间必连边，保证了连通性
        * 二者连的边越少，就越有可能连边
        */
        auto dfs = [&](auto dfs, int now) -> void{
            if (dfn[now]) return ;
            dfn[now] = ++cnt;
            for (auto v:g[now]) {
                if (set.find((now << 5) | v) != set.end()) {
                    continue;
                }
                if (dfn[v] && dfn[now] - dfn[v] <= 6) {
                    continue;
                }
                //随机一个概率
                mt19937 p(rd());
                uniform_real_distribution<double> dis(0.0, 1.0);
                double probability = dis(p);
                if (square(g[now].size()) * square(g[v].size()) * probability <=
                    square(d[now]) * square(d[v])) {
                    /*这里要先删边再进行下一层的dfs
                    * 否则会重复搜边
                    */
                    set.insert((now << 5) | v);
                    set.insert((v << 5) | now);
                    d[now]--;d[v]--;
                    dfs(dfs, v);
                }
            }
        };

        
        /*检查图是否联通
        * 如果当前联通分量的点数不是Row*Column，
        * 则dfs过程后直接连另一个联通分量
        */
        auto check = [&](auto check, int now)->void {
            if (vis[now]) return;
            vis[now] = 1;
            cnt++;
            for (auto v : g[now]) {
                //走遍连通分量
                if (set.find((now << 5) | v) != set.end()) {
                    check(check,v);
                }
            }
            if (cnt < Row * Column) {
                
                for (auto v : g[now]) {
                    if (set.find((now << 5) | v) != set.end()) {
                        continue;
                    }
                    if (vis[v]) continue;
                    set.insert((now << 5) | v);
                    set.insert((v << 5) | now);
                    check(check,v);
                    break;
                }
            }
        };

        cnt = 0;
        for (int i = 1;i < Row * Column;i++) {
            dfs(dfs,i);
        }

        cnt = 0;
        check(check, 1);
        for (int i = 1;i <= Row * Column;i++) {
            for (auto v : g[i]) {
                if (set.find((i << 5) | v) != set.end()) {
                    continue;
                }
                addWall(NtoP(i), NtoP(v));
                set.insert( ( i << 5 ) | v);
                set.insert( ( v << 5 ) | i);
            }
        }
        
        delete[] dfn;
        delete[] d;
    }
}
