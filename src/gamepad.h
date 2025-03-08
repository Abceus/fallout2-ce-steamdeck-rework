#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "SDL_events.h"

namespace fallout {

struct Stick {
    int horizontalAxis = 0;
    int verticalAxis = 0;
};

struct GamepadState {
    bool aButtonPresed = false;
    bool bButtonPresed = false;
    bool startButtonPresed = false;
    Stick leftStick;
    Stick rightStick;

    bool anyButtonPressed() const { return aButtonPresed || bButtonPresed || startButtonPresed; }
};

void initGamepad();

const GamepadState* getGamepadState();

} // namespace fallout

#endif /* GAMEPAD_H */