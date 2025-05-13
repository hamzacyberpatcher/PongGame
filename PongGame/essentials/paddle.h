#pragma once
#include "utility.h"

class Paddle {
    RectangleShape m_rect;

public:
    Paddle(RectangleShape rect) : m_rect(rect) {}

    RectangleShape getRect() const { return m_rect; }
    void setRect(RectangleShape rect) { m_rect = rect; }

    void moveY(float dy) {
        m_rect.y += dy;
        // Clamp to screen
        if (m_rect.y < 0) m_rect.y = 0;
        if (m_rect.y + m_rect.height > HEIGHT) m_rect.y = HEIGHT - m_rect.height;
    }
};