//
// Created by antho on 2026-01-28.
//

#ifndef TUTORIAL1_VECTOR2D_H
#define TUTORIAL1_VECTOR2D_H

class Vector2D {
public:
    float x = 0.0f;
    float y = 0.0f;

    Vector2D() {
        x = 0.0f;
        y = 0.0f;
    }

    Vector2D(float x, float y) : x(x), y(y){}

    // Addition
    // Member operator function
    // Vector that we want to return is on the left
    // Another vector is on the right
    Vector2D& operator+=(const Vector2D& vector);
    Vector2D operator+(Vector2D &vector) const;


    // Subtraction
    Vector2D& operator-=(const Vector2D& vector);
    Vector2D operator-(Vector2D& vector) const;


    // Multiplication
    Vector2D& operator*=(float scalar);

    // Member operator function
    // Vector 2D has to be on the left, float on the right
    Vector2D operator*(float scalar) const;

    // Vector 2D is on the right, and the float is on the left
    // Need to make this a non-member function
    // Friend makes this a non-member function
    friend Vector2D operator*(float scalar, Vector2D& vector);


    // Division
    Vector2D& operator/=(float scalar);


    // Compare two vectors
    bool operator==(Vector2D& vector) const;
    bool operator!=(Vector2D& vector) const;


    // Negate vector (flip its direction)
    friend Vector2D operator-(Vector2D& vector);

    Vector2D& normalize();
};

#endif //TUTORIAL1_VECTOR2D_H