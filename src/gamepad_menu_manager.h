#ifndef GAMEPAD_MENU_MANAGER_H
#define GAMEPAD_MENU_MANAGER_H

#include <vector>

#include "window_manager.h"

namespace fallout {

struct GamepadButton {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    int selectCode = -1;
    unsigned char* normalImage = nullptr;
    unsigned char* hoverImage = nullptr;
};

class VerticalLayout {
public:
    void setWindow(Window* newWindow) { window = newWindow; }

    void reserveSpaceForButtons(size_t size);
    GamepadButton* addButton(
        int x,
        int y,
        int width,
        int height,
        int selectCode,
        unsigned char* normalImage,
        unsigned char* hoverImage);

    void makeCurrent();
    void makeButtonCurrent(int i) { currentButton = i; }

    int updateButtons();

private:
    std::vector<GamepadButton*> buttons;
    int currentButton = 0;
    Window* window = nullptr;
};

VerticalLayout* addVerticalLayout();
void removeVerticalLayout(const VerticalLayout* layout);

int updateLayouts();

} // namespace fallout

#endif /* GAMEPAD_MENU_MANAGER_H */