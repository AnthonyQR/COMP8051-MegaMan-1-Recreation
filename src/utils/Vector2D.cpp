//
// Created by antho on 2026-01-28.
//

#include "Vector2D.h"

#include <cmath>

// Add two vectors together
// Member operator function so we use the Vector2D that we want to change on the left hand side
Vector2D& Vector2D::operator+=(const Vector2D& vector) {
    this -> x += vector.x;
    this -> y += vector.y;
    return *this;
}

Vector2D Vector2D::operator+(Vector2D& vector) const{
    return Vector2D(x + vector.x, y + vector.y);
}


// Subtracts two vectors
Vector2D& Vector2D::operator-=(const Vector2D& vector) {
    this -> x -= vector.x;
    this -> y -= vector.y;
    return *this;
}

Vector2D Vector2D::operator-(Vector2D& vector) const{
    return Vector2D(x - vector.x, y - vector.y);
}

// Multiply vector by a scalar
// Member operator function so we can use the Vector2D on the left hand side
Vector2D Vector2D::operator*(float scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

// Non-member operator function so we can make use of putting the Vector2D on the right hand side
Vector2D operator*(float scalar, Vector2D& vector) {
    return Vector2D(vector.x * scalar, vector.y * scalar);
}

Vector2D& Vector2D::operator*=(float scalar) {
    this -> x *= scalar;
    this -> y *= scalar;
    return *this;
}


// Divide vector by a scalar
Vector2D& Vector2D::operator/=(float scalar) {
    this -> x /= scalar;
    this -> y /= scalar;
    return *this;
}


// Compare two bools
// If they are equal
bool Vector2D::operator==(Vector2D& vector) const {
    if (x != vector.x || y != vector.y) {
        return false;
    }
    return true;
}

// If they are NOT equal
bool Vector2D::operator!=(Vector2D& vector) const {
    if (x == vector.x && y == vector.y) {
        return false;
    }
    return true;
}


Vector2D operator-(Vector2D& vector) {
    return Vector2D(-vector.x, -vector.y);
}




Vector2D &Vector2D::normalize() {
    // Pythagorean Theorem
    float length = sqrt(x * x + y * y);

    // Ex. If the length was 4.4
    // x = x/4.4
    // y = y/4.4
    if (length > 0) {
        this->x /= length;
        this->y /= length;
    }

    return *this;
}
