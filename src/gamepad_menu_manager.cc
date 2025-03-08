#include "gamepad_menu_manager.h"

#include "SDL_timer.h"
#include "draw.h"
#include "gamepad.h"
#include "input.h"
#include "kb.h"

#include <list>

namespace fallout {

static std::list<GamepadButton> allButtons;
static std::list<VerticalLayout> verticalLayouts;

static VerticalLayout* currentLayout = nullptr;
int stickActiveTimestamp = -1;

void VerticalLayout::reserveSpaceForButtons(size_t size)
{
    buttons.reserve(size);
}

GamepadButton* VerticalLayout::addButton(
    int x,
    int y,
    int width,
    int height,
    int selectCode,
    unsigned char* normalImage,
    unsigned char* hoverImage)
{
    const auto newButton = &allButtons.emplace_back(GamepadButton { x, y, width, height, selectCode, normalImage, hoverImage });
    buttons.emplace_back(newButton);
    return newButton;
}

void VerticalLayout::makeCurrent()
{
    currentLayout = this;
}

int VerticalLayout::updateButtons()
{
    constexpr auto stickActiveTimeout = 300;
    const auto* gamepadState = getGamepadState();
    if (gamepadState->leftStick.verticalAxis > 3200) {
        if (stickActiveTimestamp == -1 || SDL_GetTicks() - stickActiveTimestamp >= stickActiveTimeout) {
            stickActiveTimestamp = SDL_GetTicks();
            currentButton += 1;
            if (currentButton >= buttons.size()) {
                currentButton = 0;
            }
        }
    } else if (gamepadState->leftStick.verticalAxis < -3200) {
        if (stickActiveTimestamp == -1 || SDL_GetTicks() - stickActiveTimestamp >= stickActiveTimeout) {
            stickActiveTimestamp = SDL_GetTicks();
            currentButton -= 1;
            if (currentButton < 0) {
                currentButton = buttons.size() - 1;
            }
        }
    } else {
        stickActiveTimestamp = -1;
    }

    for (int i = 0; i < buttons.size(); ++i) {
        const auto* button = buttons[i];
        blitBufferToBufferTrans(
            i == currentButton ? button->hoverImage : button->normalImage,
            button->width,
            button->height,
            button->width,
            window->buffer + window->width * button->y + button->x,
            window->width);

        Rect updateRect;
        updateRect.left = button->x;
        updateRect.right = button->x + button->width;
        updateRect.top = button->y;
        updateRect.bottom = button->y + button->height;
        _GNW_win_refresh(window, &updateRect, nullptr);
    }

    return gamepadState->aButtonPresed ? buttons[currentButton]->selectCode : -1;
}

VerticalLayout* addVerticalLayout()
{
    return &verticalLayouts.emplace_back();
}

void removeVerticalLayout(const VerticalLayout* layout)
{
    if (currentLayout == layout) {
        currentLayout = nullptr;
    }

    verticalLayouts.remove_if([layout](const auto& element) {
        return layout == &element;
    });
}

int updateLayouts()
{
    inputGetInput();
    if (currentLayout == nullptr) {
        return -1;
    }

    return currentLayout->updateButtons();
}

} // namespace fallout
