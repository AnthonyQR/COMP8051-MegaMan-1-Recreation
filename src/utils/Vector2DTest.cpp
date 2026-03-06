//
// Created by antho on 2026-01-28.
//

#include "Vector2DTest.h"
#include <Vector2D.h>
#include <iostream>

Vector2DTest::Vector2DTest(){};

void Vector2DTest::Test() {
    Vector2D a(3, 4);
    Vector2D b(1,2);

    a += b;
    std::cout << "a + b = " << a.x << ", " << a.y << std::endl;

    Vector2D c = a - b;
    std:: cout << "a - b = " << c.x << ", " << c.y << std::endl;

    Vector2D d = a * 2;
    std::cout << "a * 2 = " << d.x << ", " << d.y << std::endl;

    Vector2D e = 2 * a;
    std::cout << "2 * a = " << e.x << ", " << e.y << std::endl;

    Vector2D f = -a;
    std::cout << "-a = " << f.x << ", " << f.y << std::endl;

    bool same = (a == b);
    std::cout << "a == b -> " << same << std::endl;
}