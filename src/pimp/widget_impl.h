#pragma once
#include "widget.h"
class Widget::WidgetImpl {
public:
    void draw(const Widget& w);
    WidgetImpl(int n);
private:
    int n_;
};