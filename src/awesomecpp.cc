#include <iostream>
#include "pimp/widget.h"
#include <memory.h>
#include <memory>
int main () {
    std::make_unique<Widget>(10)->draw();
    return 0;
}