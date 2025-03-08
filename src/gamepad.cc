#include "gamepad.h"
#include "SDL_gamecontroller.h"
#include "input.h"

namespace fallout {

static GamepadState gamepadState;
SDL_GameController* mainController = nullptr;

void _handle_gamepad_events(const SDL_Event* event)
{
    if (mainController == nullptr) {
        return;
    }

    switch (event->type) {
    case SDL_CONTROLLERBUTTONDOWN:
        switch (event->cbutton.button) {
        case SDL_CONTROLLER_BUTTON_A:
            gamepadState.aButtonPresed = true;
            break;
        case SDL_CONTROLLER_BUTTON_B:
            gamepadState.bButtonPresed = true;
            break;
        case SDL_CONTROLLER_BUTTON_START:
            gamepadState.startButtonPresed = true;
            break;
        }
        break;

    case SDL_CONTROLLERBUTTONUP:
        switch (event->cbutton.button) {
        case SDL_CONTROLLER_BUTTON_A:
            gamepadState.aButtonPresed = false;
            break;
        case SDL_CONTROLLER_BUTTON_B:
            gamepadState.bButtonPresed = false;
            break;
        case SDL_CONTROLLER_BUTTON_START:
            gamepadState.startButtonPresed = false;
            break;
        }
        break;

    case SDL_CONTROLLERAXISMOTION:
        if (event->caxis.axis == 0) {
            gamepadState.leftStick.horizontalAxis = event->caxis.value;
        }

        if (event->caxis.axis == 1) {
            gamepadState.leftStick.verticalAxis = event->caxis.value;
        }

        if (event->caxis.axis == 2) {
            gamepadState.rightStick.horizontalAxis = event->caxis.value;
        }

        if (event->caxis.axis == 3) {
            gamepadState.rightStick.verticalAxis = event->caxis.value;
        }
        break;
    }
}

void initGamepad()
{
    SDL_GameControllerEventState(SDL_ENABLE);
    const auto joys = SDL_NumJoysticks();
    mainController = SDL_GameControllerOpen(0);

    eventsHandlerAdd(&_handle_gamepad_events);
}

const GamepadState* getGamepadState()
{
    return &gamepadState;
}

} // namespace fallout