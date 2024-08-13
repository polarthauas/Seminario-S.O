#include "mouse.h"

#include <windows.h>
#include <thread>
#include <cmath>
#include <SDL_image.h>

Mouse::Mouse(SDL_Renderer* rend) {

}

void Mouse::moveMouseSmoothly(int targetX, int targetY, int durationMs) {
    // Get the current cursor position
    POINT currentPos;
    GetCursorPos(&currentPos);

    // Calculate the distance to move
    int startX = currentPos.x;
    int startY = currentPos.y;
    int deltaX = targetX - startX;
    int deltaY = targetY - startY;

    // Determine the number of steps to take based on the desired duration
    int steps = 100; // Number of steps (higher number for smoother movement)
    int sleepDuration = durationMs / steps; // Duration to sleep between steps

    for (int i = 1; i <= steps; ++i) {
        // Calculate intermediate positions
        int newX = startX + (deltaX * i) / steps;
        int newY = startY + (deltaY * i) / steps;

        // Move the cursor to the new position
        SetCursorPos(newX, newY);

        // Sleep for a short duration to create smooth movement
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
    }
}