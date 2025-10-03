#pragma once
#include <memory>
class Widget {
private:
    class WidgetImpl;
    std::unique_ptr<WidgetImpl> widget_impl_;
public:
    void draw();
    Widget(int n);
    ~Widget();
    Widget(const Widget& rhs) = delete;
    Widget(Widget&& rhs) noexcept;
    Widget operator=(Widget&&) noexcept;
    Widget operator=(const Widget&) = delete;
};