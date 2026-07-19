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

std::string showTextInputForm(const std::string& title,
                              const std::string& label,
                              const std::string& placeholder) {
    auto screen = ScreenInteractive::Fullscreen();

    std::string value;
    bool confirmed = false;
    auto input = Input(&value, placeholder);
    auto btn_ok = Button("  确认  ", [&] {
        if (!value.empty()) {
            confirmed = true;
            screen.Exit();
        }
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});
    auto container = Container::Vertical({input, buttons});

    container |= CatchEvent([&](Event event) {
        if (event == Event::Return && input->Focused()) {
            btn_ok->OnEvent(Event::Return);
            return true;
        }
        return false;
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
                   text(title) | bold | center,
                   separator(),
                   hbox({text(label + ": "), input->Render() | flex}),
                   separator(),
                   hbox({btn_ok->Render() | flex, btn_cancel->Render() | flex}),
               }) |
               border | center;
    });

    screen.Loop(renderer);
    return confirmed ? value : "";
}

PasswordChangeData showChangePasswordForm(const std::string& title) {
    auto screen = ScreenInteractive::Fullscreen();

    PasswordChangeData data;
    std::string error;
    InputOption password_option;
    password_option.password = true;

    auto current = Input(&data.currentPassword, "当前密码", password_option);
    auto next = Input(&data.newPassword, "新密码", password_option);
    auto confirm = Input(&data.confirmPassword, "确认新密码", password_option);

    auto btn_ok = Button("  修改  ", [&] {
        if (data.currentPassword.empty() || data.newPassword.empty()) {
            error = "密码不能为空";
            return;
        }
        if (data.newPassword != data.confirmPassword) {
            error = "两次输入的新密码不一致";
            return;
        }
        error.clear();
        data.confirmed = true;
        screen.Exit();
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});
    auto container = Container::Vertical({current, next, confirm, buttons});

    auto renderer = Renderer(container, [&] {
        Elements rows = {
            text(title) | bold | center,
            separator(),
            hbox({text("当前密码: "), current->Render() | flex}),
            hbox({text("新 密 码: "), next->Render() | flex}),
            hbox({text("确认密码: "), confirm->Render() | flex}),
            separator(),
        };
        if (!error.empty()) {
            rows.push_back(text(error) | color(Color::Red));
            rows.push_back(separator());
        }
        rows.push_back(
            hbox({btn_ok->Render() | flex, btn_cancel->Render() | flex}));
        return vbox(rows) | border | center;
    });

    screen.Loop(renderer);
    return data;
}
