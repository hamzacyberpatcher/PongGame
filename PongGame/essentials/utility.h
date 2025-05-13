#pragma once

struct Vector2d {
    float x, y;
    Vector2d(float x = 0, float y = 0) : x(x), y(y) {}
};

struct RectangleShape {
    float x, y, width, height;
    RectangleShape(float x = 0, float y = 0, float width = 0, float height = 0)
        : x(x), y(y), width(width), height(height) {
    }
};
