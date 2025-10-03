

#include "widget.h"
#include "widget_impl.h"
#include <memory>


void Widget::draw(){
    widget_impl_->draw(*this);
}
Widget::Widget(int n) : widget_impl_(std::make_unique<WidgetImpl>(n)){}

Widget::~Widget() = default;

