#include "ImGuiLayer.hpp"

namespace MultiStation {


    ImGuiKey MS_to_ImGui_key(uint32_t ms_key)noexcept {

        switch (ms_key)
        {
        case MS_KEY_TAB: return ImGuiKey_Tab;
        case MS_KEY_LEFT: return ImGuiKey_LeftArrow;
        case MS_KEY_RIGHT: return ImGuiKey_RightArrow;
        case MS_KEY_UP: return ImGuiKey_UpArrow;
        case MS_KEY_DOWN: return ImGuiKey_DownArrow;
        case MS_KEY_PAGE_UP: return ImGuiKey_PageUp;
        case MS_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
        case MS_KEY_HOME: return ImGuiKey_Home;
        case MS_KEY_END: return ImGuiKey_End;
        case MS_KEY_INSERT: return ImGuiKey_Insert;
        case MS_KEY_DELETE: return ImGuiKey_Delete;
        case MS_KEY_BACKSPACE: return ImGuiKey_Backspace;
        case MS_KEY_SPACE: return ImGuiKey_Space;
        case MS_KEY_ENTER: return ImGuiKey_Enter;
        case MS_KEY_ESCAPE: return ImGuiKey_Escape;

        case MS_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
        case MS_KEY_COMMA: return ImGuiKey_Comma;
        case MS_KEY_MINUS: return ImGuiKey_Minus;
        case MS_KEY_PERIOD: return ImGuiKey_Period;
        case MS_KEY_SLASH: return ImGuiKey_Slash;
        case MS_KEY_SEMICOLON: return ImGuiKey_Semicolon;
        case MS_KEY_EQUAL: return ImGuiKey_Equal;
        case MS_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
        case MS_KEY_BACKSLASH: return ImGuiKey_Backslash;
        case MS_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
        case MS_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;

        case MS_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
        case MS_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
        case MS_KEY_NUM_LOCK: return ImGuiKey_NumLock;
        case MS_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
        case MS_KEY_PAUSE: return ImGuiKey_Pause;

        case MS_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
        case MS_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
        case MS_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
        case MS_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
        case MS_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
        case MS_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
        case MS_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
        case MS_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
        case MS_KEY_MENU: return ImGuiKey_Menu;
        }

        // Letters
        if (ms_key >= MS_KEY_A && ms_key <= MS_KEY_Z)
            return (ImGuiKey)(ImGuiKey_A + (ms_key - MS_KEY_A));

        // Numbers
        if (ms_key >= MS_KEY_0 && ms_key <= MS_KEY_9)
            return (ImGuiKey)(ImGuiKey_0 + (ms_key - MS_KEY_0));

        // F keys
        if (ms_key >= MS_KEY_F1 && ms_key <= MS_KEY_F12)
            return (ImGuiKey)(ImGuiKey_F1 + (ms_key - MS_KEY_F1));

        return ImGuiKey_None;
    }

    uint32_t ImGui_to_MS_key(ImGuiKey imgui_key) noexcept {

        switch (imgui_key)
        {
        case ImGuiKey_Tab: return MS_KEY_TAB;
        case ImGuiKey_LeftArrow: return MS_KEY_LEFT;
        case ImGuiKey_RightArrow: return MS_KEY_RIGHT;
        case ImGuiKey_UpArrow: return MS_KEY_UP;
        case ImGuiKey_DownArrow: return MS_KEY_DOWN;
        case ImGuiKey_PageUp: return MS_KEY_PAGE_UP;
        case ImGuiKey_PageDown: return MS_KEY_PAGE_DOWN;
        case ImGuiKey_Home: return MS_KEY_HOME;
        case ImGuiKey_End: return MS_KEY_END;
        case ImGuiKey_Insert: return MS_KEY_INSERT;
        case ImGuiKey_Delete: return MS_KEY_DELETE;
        case ImGuiKey_Backspace: return MS_KEY_BACKSPACE;
        case ImGuiKey_Space: return MS_KEY_SPACE;
        case ImGuiKey_Enter: return MS_KEY_ENTER;
        case ImGuiKey_Escape: return MS_KEY_ESCAPE;

        case ImGuiKey_LeftShift: return MS_KEY_LEFT_SHIFT;
        case ImGuiKey_LeftCtrl: return MS_KEY_LEFT_CONTROL;
        case ImGuiKey_LeftAlt: return MS_KEY_LEFT_ALT;
        case ImGuiKey_RightShift: return MS_KEY_RIGHT_SHIFT;
        case ImGuiKey_RightCtrl: return MS_KEY_RIGHT_CONTROL;
        case ImGuiKey_RightAlt: return MS_KEY_RIGHT_ALT;
        }

        // Letters
        if (imgui_key >= ImGuiKey_A && imgui_key <= ImGuiKey_Z)
            return MS_KEY_A + (imgui_key - ImGuiKey_A);

        // Numbers
        if (imgui_key >= ImGuiKey_0 && imgui_key <= ImGuiKey_9)
            return MS_KEY_0 + (imgui_key - ImGuiKey_0);

        // F keys
        if (imgui_key >= ImGuiKey_F1 && imgui_key <= ImGuiKey_F12)
            return MS_KEY_F1 + (imgui_key - ImGuiKey_F1);

        return -1;

    }

    int MS_to_ImGui_mouse(int button) noexcept
    {
        switch (button)
        {
        case MS_MOUSE_BUTTON_LEFT:   return 0; // ImGuiMouseButton_Left
        case MS_MOUSE_BUTTON_RIGHT:  return 1; // ImGuiMouseButton_Right
        case MS_MOUSE_BUTTON_MIDDLE: return 2; // ImGuiMouseButton_Middle
        case MS_MOUSE_BUTTON_4:      return 3;
        case MS_MOUSE_BUTTON_5:      return 4;
        default: return -1;
        }
    }

    uint32_t ImGui_to_MS_mouse(int button) noexcept
    {
        switch (button)
        {
        case 0: return MS_MOUSE_BUTTON_LEFT;
        case 1: return MS_MOUSE_BUTTON_RIGHT;
        case 2: return MS_MOUSE_BUTTON_MIDDLE;
        case 3: return MS_MOUSE_BUTTON_4;
        case 4: return MS_MOUSE_BUTTON_5;
        default: return 0xFFFFFFFF;
        }
    }

}
