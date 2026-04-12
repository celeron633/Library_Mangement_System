#include "login_ui.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

int showMainMenu() {
    auto screen = ScreenInteractive::Fullscreen();

    std::vector<std::string> entries = {
        "  管理员登录  ",
        "  用户登录    ",
        "  用户注册    ",
        "  系统设置    ",
        "  退出系统    ",
    };
    int selected = 0;

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    auto menu = Menu(&entries, &selected, option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
                   text("📚  图书馆管理系统") | bold | center,
                   separator(),
                   menu->Render(),
               }) |
               border | center;
    });

    screen.Loop(renderer);
    return selected;
}

std::pair<std::string, std::string> showLoginForm(const std::string& title) {
    auto screen = ScreenInteractive::Fullscreen();

    std::string username, password;
    bool confirmed = false;

    auto input_user = Input(&username, "用户名");

    InputOption pwd_opt;
    pwd_opt.password = true;
    auto input_pwd = Input(&password, "密码", pwd_opt);

    auto btn_ok = Button("  登录  ", [&] {
        confirmed = true;
        screen.Exit();
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});

    auto container = Container::Vertical({input_user, input_pwd, buttons});

    // Allow pressing Enter in password field to submit
    container |= CatchEvent([&](Event event) {
        if (event == Event::Return && input_pwd->Focused()) {
            confirmed = true;
            screen.Exit();
            return true;
        }
        return false;
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
                   text(title) | bold | center,
                   separator(),
                   hbox({text("用户名: "), input_user->Render() | flex}),
                   hbox({text("密  码: "), input_pwd->Render() | flex}),
                   separator(),
                   hbox({btn_ok->Render() | flex, btn_cancel->Render() | flex}),
               }) |
               border | center;
    });

    screen.Loop(renderer);

    if (!confirmed || username.empty()) {
        return {"", ""};
    }
    return {username, password};
}

RegisterData showRegisterForm() {
    auto screen = ScreenInteractive::Fullscreen();

    RegisterData data;

    auto input_user = Input(&data.username, "用户名");

    InputOption pwd_opt;
    pwd_opt.password = true;
    auto input_pwd = Input(&data.password, "密码", pwd_opt);
    auto input_confirm = Input(&data.confirmPassword, "再次输入密码", pwd_opt);

    auto btn_ok = Button("  注册  ", [&] {
        data.confirmed = true;
        screen.Exit();
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});

    auto container =
        Container::Vertical({input_user, input_pwd, input_confirm, buttons});

    auto renderer = Renderer(container, [&] {
        return vbox({
                   text("用户注册") | bold | center,
                   separator(),
                   hbox({text("用户名:   "), input_user->Render() | flex}),
                   hbox({text("密    码: "), input_pwd->Render() | flex}),
                   hbox({text("确认密码: "), input_confirm->Render() | flex}),
                   separator(),
                   hbox({btn_ok->Render() | flex, btn_cancel->Render() | flex}),
               }) |
               border | center;
    });

    screen.Loop(renderer);
    return data;
}

std::string showPasswordInputForm(const std::string& title) {
    auto screen = ScreenInteractive::Fullscreen();

    std::string password;
    bool confirmed = false;

    InputOption pwd_opt;
    pwd_opt.password = true;
    auto input_pwd = Input(&password, "密码", pwd_opt);

    auto btn_ok = Button("  确认  ", [&] {
        confirmed = true;
        screen.Exit();
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});

    auto container = Container::Vertical({input_pwd, buttons});

    container |= CatchEvent([&](Event event) {
        if (event == Event::Return && input_pwd->Focused()) {
            confirmed = true;
            screen.Exit();
            return true;
        }
        return false;
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
                   text(title) | bold | center,
                   separator(),
                   text("请输入密码以验证身份") | color(Color::Yellow),
                   separator(),
                   hbox({text("密  码: "), input_pwd->Render() | flex}),
                   separator(),
                   hbox({btn_ok->Render() | flex, btn_cancel->Render() | flex}),
               }) |
               border | center;
    });

    screen.Loop(renderer);

    if (!confirmed) return "";
    return password;
}
