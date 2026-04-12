#include "common_ui.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

void showMessage(const std::string& title, const std::string& message,
                 MessageType type) {
    auto screen = ScreenInteractive::Fullscreen();

    Color msg_color;
    std::string prefix;
    switch (type) {
        case MessageType::SUCCESS:
            msg_color = Color::Green;
            prefix = "✓  ";
            break;
        case MessageType::ERROR_MSG:
            msg_color = Color::Red;
            prefix = "✗  ";
            break;
        case MessageType::WARNING:
            msg_color = Color::Yellow;
            prefix = "⚠  ";
            break;
        default:
            msg_color = Color::Cyan;
            prefix = "ℹ  ";
            break;
    }

    auto btn_ok = Button("  确认  ", screen.ExitLoopClosure());

    auto renderer = Renderer(btn_ok, [&] {
        Elements rows;
        if (!title.empty()) {
            rows.push_back(text(title) | bold | center);
            rows.push_back(separator());
        }
        rows.push_back(text(prefix + message) | color(msg_color));
        rows.push_back(separator());
        rows.push_back(btn_ok->Render() | center);

        return vbox(rows) | border | center;
    });

    screen.Loop(renderer);
}

bool showConfirmDialog(const std::string& message) {
    auto screen = ScreenInteractive::Fullscreen();

    bool confirmed = false;

    auto btn_yes = Button("  是  ", [&] {
        confirmed = true;
        screen.Exit();
    });
    auto btn_no = Button("  否  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_yes, btn_no});

    auto renderer = Renderer(buttons, [&] {
        return vbox({
                   text("确认操作") | bold | center,
                   separator(),
                   text(message) | color(Color::Yellow),
                   separator(),
                   hbox({btn_yes->Render() | flex, btn_no->Render() | flex}),
               }) |
               border | center;
    });

    screen.Loop(renderer);
    return confirmed;
}
