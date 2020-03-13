#ifndef STYLE_H
#define STYLE_H

class Style {

    public:
        Style() = default;
        Style(HPEN pen, HBRUSH brush);
        ~Style();

        Style(const Style&) = delete;
        Style& operator = (const Style&) = delete;

        Style(Style&& rhs);
        Style& operator = (Style&& rhs);

        void select(HDC hdc);

    private:
        HPEN   mPen = NULL;
        HBRUSH mBrush = NULL;

};

#endif // STYLE_H
