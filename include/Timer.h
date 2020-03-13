#ifndef TIMER_H
#define TIMER_H

class Timer
{
    public:
        Timer(){
            lastTime = std::clock();
        }

        int tick(){
            const long now = std::clock();
            const int dt = now - lastTime;

            lastTime = now;

            return dt;
        }

        int getTime() const{
            return std::clock() - lastTime;
        }
    private:
        long lastTime;
};

#endif // TIMER_H
