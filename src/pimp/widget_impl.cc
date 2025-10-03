#include "widget_impl.h"
#include "widget.h"
#include <iostream>
using std::cout;
Widget::WidgetImpl::WidgetImpl(int n) : n_(n){}

void Widget::WidgetImpl::draw(const Widget& w) {
    cout << "the number var of widget_impl : " << n_ << "\n";
}