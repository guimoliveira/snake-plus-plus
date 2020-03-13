#include "SnakePlusPlus.h"

Style::Style(HPEN pen, HBRUSH brush):
    mPen(pen), mBrush(brush)
{}

Style::~Style() {
    DeleteObject(mPen);
    DeleteObject(mBrush);
}

Style::Style(Style&& rhs) {
    *this = std::move(rhs);
}

Style& Style::operator = (Style&& rhs) {
    mPen = rhs.mPen;
    mBrush = rhs.mBrush;

    rhs.mPen = NULL;
    rhs.mBrush = NULL;

    return *this;
}

void Style::select(HDC hdc) {
    SelectObject(hdc, mPen);
    SelectObject(hdc, mBrush);
}


