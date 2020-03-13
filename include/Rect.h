#ifndef RECT_H
#define RECT_H

class Rect{
    public:
        int x1;
        int y1;

        int x2;
        int y2;

        Rect& operator *= (int i){
            int width = x2 - x1;
            int height = y2 - y1;

            int newWidth = width*i;
            int newHeight = height*i;

            int difWidth = (newWidth-width)/2;
            int difHeight = (newHeight-height)/2;

            x1 -= difWidth;
            y1 -= difHeight;

            x2 += difWidth;
            y2 += difHeight;

            return *this;
        }
};

#endif // RECT_H
