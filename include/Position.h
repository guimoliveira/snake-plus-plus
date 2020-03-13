#ifndef POSITION_H
#define POSITION_H

class Position
{
    public:
        Position(float _x, float _y):
            x(_x), y(_y) {}

        Position(int _x, int _y):
            x((float)_x), y((float)_y) {}

        float x;
        float y;

        bool operator == (const Position& p) const{
            return std::round(x) == std::round(p.x) &&
                   std::round(y) == std::round(p.y);
        }
        bool operator != (const Position& p) const{
            return std::round(x) != std::round(p.x) ||
                   std::round(y) != std::round(p.y);
        }

        float dist(const Position& pos) const{
            return std::hypot(pos.x - x, pos.y - y);
        }

        void round(){
            x = std::round(x);
            y = std::round(y);
        }

};

#endif // POSITION_H
