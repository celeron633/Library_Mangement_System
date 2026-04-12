#include "system_ui.h"

#include <iomanip>
#include <sstream>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>

using namespace ftxui;

int showSystemConfigMenu() {
    auto screen = ScreenInteractive::Fullscreen();

    std::vector<std::string> entries = {
        "  显示管理员列表  ",
        "  创建新管理员    ",
        "  显示用户列表    ",
        "  删除用户        ",
        "  清除所有书籍数据",
        "  清除所有用户数据",
        "  返回            ",
    };
    int selected = 0;

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    auto menu = Menu(&entries, &selected, option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
                   text("系统设置") | bold | center,
                   separator(),
                   menu->Render(),
               }) |
               border | center;
    });

    screen.Loop(renderer);
    return selected;
}

std::pair<std::string, std::string> showAdminAuthForm() {
    auto screen = ScreenInteractive::Fullscreen();

    std::string admin_name, password;
    bool confirmed = false;

    auto input_name = Input(&admin_name, "管理员名字");

    InputOption pwd_opt;
    pwd_opt.password = true;
    auto input_pwd = Input(&password, "密码", pwd_opt);

    auto btn_ok = Button("  确认  ", [&] {
        confirmed = true;
        screen.Exit();
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});

    auto container = Container::Vertical({input_name, input_pwd, buttons});

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
                   text("系统设置 - 管理员验证") | bold | center,
                   separator(),
                   text("需要管理员权限，请验证身份") | color(Color::Yellow),
                   separator(),
                   hbox({text("管理员名字: "), input_name->Render() | flex}),
                   hbox({text("密      码: "), input_pwd->Render() | flex}),
                   separator(),
                   hbox({btn_ok->Render() | flex, btn_cancel->Render() | flex}),
               }) |
               border | center;
    });

    screen.Loop(renderer);

    if (!confirmed || admin_name.empty()) return {"", ""};
    return {admin_name, password};
}

AdminCreateData showCreateAdminForm() {
    auto screen = ScreenInteractive::Fullscreen();

    AdminCreateData data;
    std::string error_msg;

    auto input_name = Input(&data.name, "管理员名字");

    InputOption pwd_opt;
    pwd_opt.password = true;
    auto input_pwd = Input(&data.password, "密码", pwd_opt);
    auto input_confirm = Input(&data.confirmPassword, "再次输入密码", pwd_opt);

    auto btn_ok = Button("  创建  ", [&] {
        if (data.name.empty()) {
            error_msg = "管理员名字不能为空";
            return;
        }
        if (data.password.empty()) {
            error_msg = "密码不能为空";
            return;
        }
        error_msg.clear();
        data.confirmed = true;
        screen.Exit();
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});

    auto container = Container::Vertical(
        {input_name, input_pwd, input_confirm, buttons});

    auto renderer = Renderer(container, [&] {
        Elements rows = {
            text("创建管理员账号") | bold | center,
            separator(),
            hbox({text("管理员名字: "), input_name->Render() | flex}),
            hbox({text("密      码: "), input_pwd->Render() | flex}),
            hbox({text("确  认密码: "), input_confirm->Render() | flex}),
            separator(),
        };
        if (!error_msg.empty()) {
            rows.push_back(text(error_msg) | color(Color::Red));
            rows.push_back(separator());
        }
        rows.push_back(
            hbox({btn_ok->Render() | flex, btn_cancel->Render() | flex}));
        return vbox(rows) | border | center;
    });

    screen.Loop(renderer);
    return data;
}

void showAdminListTable(const std::vector<Admin>& admins) {
    auto screen = ScreenInteractive::Fullscreen();

    auto btn_back = Button("  返回  ", screen.ExitLoopClosure());

    auto make_table = [&]() -> Element {
        if (admins.empty()) {
            return text("暂无管理员") | center;
        }
        std::vector<std::vector<std::string>> table_data;
        table_data.push_back({"管理员名称", "密码"});
        for (const auto& admin : admins) {
            table_data.push_back({admin.getName(), admin.getPassword()});
        }
        auto tbl = Table(table_data);
        tbl.SelectAll().Border(LIGHT);
        tbl.SelectAll().SeparatorVertical(LIGHT);
        tbl.SelectAll().SeparatorHorizontal(LIGHT);
        tbl.SelectRow(0).Decorate(bold);
        tbl.SelectRow(0).Border(DOUBLE);
        return tbl.Render();
    };

    auto renderer = Renderer(btn_back, [&] {
        return vbox({
                   text("管理员列表") | bold | center,
                   separator(),
                   make_table(),
                   separator(),
                   btn_back->Render() | center,
               }) |
               border | center;
    });

    screen.Loop(renderer);
}

void showUserListTable(const std::vector<User>& users) {
    auto screen = ScreenInteractive::Fullscreen();

    auto btn_back = Button("  返回  ", screen.ExitLoopClosure());

    auto make_table = [&]() -> Element {
        if (users.empty()) {
            return text("暂无用户") | center;
        }
        std::vector<std::vector<std::string>> table_data;
        table_data.push_back({"用户名", "余额", "VIP"});
        for (const auto& user : users) {
            std::ostringstream bal_oss;
            bal_oss << std::fixed << std::setprecision(2) << user.getBalance();
            table_data.push_back(
                {user.getName(), bal_oss.str(), user.isVIP() ? "是" : "否"});
        }
        auto tbl = Table(table_data);
        tbl.SelectAll().Border(LIGHT);
        tbl.SelectAll().SeparatorVertical(LIGHT);
        tbl.SelectAll().SeparatorHorizontal(LIGHT);
        tbl.SelectRow(0).Decorate(bold);
        tbl.SelectRow(0).Border(DOUBLE);
        tbl.SelectColumn(1).DecorateCells(align_right);
        tbl.SelectColumn(2).DecorateCells(align_right);
        return tbl.Render();
    };

    auto renderer = Renderer(btn_back, [&] {
        return vbox({
                   text("用户列表") | bold | center,
                   separator(),
                   make_table(),
                   separator(),
                   btn_back->Render() | center,
               }) |
               border | center;
    });

    screen.Loop(renderer);
}

std::string showDeleteUserForm() {
    auto screen = ScreenInteractive::Fullscreen();

    std::string username;
    bool confirmed = false;

    auto input_name = Input(&username, "用户名");

    auto btn_ok = Button("  删除  ", [&] {
        confirmed = true;
        screen.Exit();
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});

    auto container = Container::Vertical({input_name, buttons});

    container |= CatchEvent([&](Event event) {
        if (event == Event::Return && input_name->Focused()) {
            confirmed = true;
            screen.Exit();
            return true;
        }
        return false;
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
                   text("删除用户") | bold | center,
                   separator(),
                   hbox({text("用户名: "), input_name->Render() | flex}),
                   separator(),
                   hbox({btn_ok->Render() | flex, btn_cancel->Render() | flex}),
               }) |
               border | center;
    });

    screen.Loop(renderer);

    if (!confirmed || username.empty()) return "";
    return username;
}
