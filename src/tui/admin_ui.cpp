#include "admin_ui.h"

#include <iomanip>
#include <sstream>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>

using namespace ftxui;

int showAdminMenu() {
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> entries = {
        "  显示所有书籍  ",
        "  书籍操作      ",
        "  书籍查询      ",
        "  返回          ",
    };
    int selected = 0;

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    auto menu = Menu(&entries, &selected, option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
                   text("管理员菜单") | bold | center,
                   separator(),
                   menu->Render(),
               }) |
               border | center;
    });

    screen.Loop(renderer);
    return selected;
}

int showAdminBookOperationsMenu() {
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> entries = {
        "  添加书籍      ",
        "  删除书籍      ",
        "  编辑书籍信息  ",
        "  编辑库存      ",
        "  返回          ",
    };
    int selected = 0;

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    auto menu = Menu(&entries, &selected, option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
                   text("书籍操作菜单") | bold | center,
                   separator(),
                   menu->Render(),
               }) |
               border | center;
    });

    screen.Loop(renderer);
    return selected;
}

int showAdminBookInquiryMenu() {
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> entries = {
        "  按价格排序显示    ",
        "  计算所有图书总价  ",
        "  查询书籍简介      ",
        "  返回              ",
    };
    int selected = 0;

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    auto menu = Menu(&entries, &selected, option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
                   text("书籍查询菜单") | bold | center,
                   separator(),
                   menu->Render(),
               }) |
               border | center;
    });

    screen.Loop(renderer);
    return selected;
}

AddBookData showAddBookForm() {
    auto screen = ScreenInteractive::TerminalOutput();

    AddBookData data;
    std::string price_str, stock_str;
    std::string error_msg;

    auto input_id = Input(&data.id, "书籍编号");
    auto input_name = Input(&data.name, "书籍名称");
    auto input_author = Input(&data.author, "作者");
    auto input_intro = Input(&data.intro, "简介");
    auto input_price = Input(&price_str, "价格");
    auto input_stock = Input(&stock_str, "库存数量");

    auto btn_ok = Button("  确认  ", [&] {
        // Validate numeric fields
        try {
            data.price = std::stod(price_str);
            data.stock = std::stoi(stock_str);
            error_msg.clear();
            data.confirmed = true;
            screen.Exit();
        } catch (...) {
            error_msg = "价格或库存格式不正确";
        }
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});

    auto container = Container::Vertical(
        {input_id, input_name, input_author, input_intro, input_price,
         input_stock, buttons});

    auto renderer = Renderer(container, [&] {
        Elements rows = {
            text("添加书籍") | bold | center,
            separator(),
            hbox({text("书籍编号: "), input_id->Render() | flex}),
            hbox({text("书籍名称: "), input_name->Render() | flex}),
            hbox({text("作    者: "), input_author->Render() | flex}),
            hbox({text("简    介: "), input_intro->Render() | flex}),
            hbox({text("价    格: "), input_price->Render() | flex}),
            hbox({text("库    存: "), input_stock->Render() | flex}),
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

std::string showBookIdInputForm(const std::string& title,
                                const std::string& label) {
    auto screen = ScreenInteractive::TerminalOutput();

    std::string book_id;
    bool confirmed = false;

    auto input_id = Input(&book_id, "书籍编号");

    auto btn_ok = Button("  确认  ", [&] {
        confirmed = true;
        screen.Exit();
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});

    auto container = Container::Vertical({input_id, buttons});

    container |= CatchEvent([&](Event event) {
        if (event == Event::Return && input_id->Focused()) {
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
                   hbox({text(label + ": "), input_id->Render() | flex}),
                   separator(),
                   hbox({btn_ok->Render() | flex, btn_cancel->Render() | flex}),
               }) |
               border | center;
    });

    screen.Loop(renderer);

    if (!confirmed || book_id.empty()) return "";
    return book_id;
}

EditBookData showEditBookForm(const Book& book) {
    auto screen = ScreenInteractive::TerminalOutput();

    EditBookData data;
    data.name = book.getName();
    data.author = book.getAuthor();
    data.intro = book.getIntroduction();

    std::string price_str =
        [&]() {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << book.getPrice();
            return oss.str();
        }();
    std::string stock_str = std::to_string(book.getStock());
    std::string error_msg;

    auto input_name = Input(&data.name, "书籍名称");
    auto input_author = Input(&data.author, "作者");
    auto input_intro = Input(&data.intro, "简介");
    auto input_price = Input(&price_str, "价格");
    auto input_stock = Input(&stock_str, "库存");

    auto btn_ok = Button("  保存  ", [&] {
        try {
            data.price = std::stod(price_str);
            data.stock = std::stoi(stock_str);
            error_msg.clear();
            data.confirmed = true;
            screen.Exit();
        } catch (...) {
            error_msg = "价格或库存格式不正确";
        }
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});

    auto container = Container::Vertical(
        {input_name, input_author, input_intro, input_price, input_stock,
         buttons});

    auto renderer = Renderer(container, [&] {
        Elements rows = {
            text("编辑书籍 [" + book.getId() + "]") | bold | center,
            separator(),
            hbox({text("书籍名称: "), input_name->Render() | flex}),
            hbox({text("作    者: "), input_author->Render() | flex}),
            hbox({text("简    介: "), input_intro->Render() | flex}),
            hbox({text("价    格: "), input_price->Render() | flex}),
            hbox({text("库    存: "), input_stock->Render() | flex}),
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

std::pair<std::string, int> showEditStockForm() {
    auto screen = ScreenInteractive::TerminalOutput();

    std::string book_id;
    std::string stock_str;
    std::string error_msg;
    bool confirmed = false;

    auto input_id = Input(&book_id, "书籍编号");
    auto input_stock = Input(&stock_str, "新库存数量");

    auto btn_ok = Button("  确认  ", [&] {
        if (book_id.empty()) {
            error_msg = "请输入书籍编号";
            return;
        }
        try {
            std::stoi(stock_str);
            error_msg.clear();
            confirmed = true;
            screen.Exit();
        } catch (...) {
            error_msg = "库存数量格式不正确";
        }
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});

    auto container = Container::Vertical({input_id, input_stock, buttons});

    auto renderer = Renderer(container, [&] {
        Elements rows = {
            text("编辑库存") | bold | center,
            separator(),
            hbox({text("书籍编号:   "), input_id->Render() | flex}),
            hbox({text("新库存数量: "), input_stock->Render() | flex}),
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

    if (!confirmed) return {"", 0};
    return {book_id, std::stoi(stock_str)};
}

void showBooksTable(const std::vector<Book>& books, const std::string& title) {
    auto screen = ScreenInteractive::TerminalOutput();

    auto btn_back = Button("  返回  ", screen.ExitLoopClosure());

    // Build table data
    auto make_table = [&]() -> Element {
        std::vector<std::vector<std::string>> table_data;
        table_data.push_back({"编号", "书名", "作者", "简介", "价格", "库存"});

        for (const auto& book : books) {
            std::ostringstream price_oss;
            price_oss << std::fixed << std::setprecision(2) << book.getPrice();

            std::string intro = book.getIntroduction();
            if (intro.length() > 20) intro = intro.substr(0, 20) + "...";

            table_data.push_back({book.getId(), book.getName(), book.getAuthor(),
                                  intro, price_oss.str(),
                                  std::to_string(book.getStock())});
        }

        auto tbl = Table(table_data);
        tbl.SelectAll().Border(LIGHT);
        tbl.SelectAll().SeparatorVertical(LIGHT);
        tbl.SelectAll().SeparatorHorizontal(LIGHT);
        tbl.SelectRow(0).Decorate(bold);
        tbl.SelectRow(0).Border(DOUBLE);
        tbl.SelectColumn(4).DecorateCells(align_right);
        tbl.SelectColumn(5).DecorateCells(align_right);
        return tbl.Render();
    };

    auto renderer = Renderer(btn_back, [&] {
        Elements content;
        content.push_back(text(title) | bold | center);
        content.push_back(separator());
        if (books.empty()) {
            content.push_back(text("暂无书籍") | center);
        } else {
            content.push_back(make_table());
        }
        content.push_back(separator());
        content.push_back(btn_back->Render() | center);
        return vbox(content) | border;
    });

    screen.Loop(renderer);
}

void showBookIntroDialog(const Book& book) {
    auto screen = ScreenInteractive::TerminalOutput();

    auto btn_ok = Button("  确认  ", screen.ExitLoopClosure());

    auto renderer = Renderer(btn_ok, [&] {
        return vbox({
                   text("书籍简介") | bold | center,
                   separator(),
                   text("编号: " + book.getId()),
                   text("书名: " + book.getName()),
                   text("作者: " + book.getAuthor()),
                   separator(),
                   paragraph(book.getIntroduction()),
                   separator(),
                   btn_ok->Render() | center,
               }) |
               border | center;
    });

    screen.Loop(renderer);
}

void showTotalPriceDialog(double total) {
    auto screen = ScreenInteractive::TerminalOutput();

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << total;
    std::string total_str = "所有书籍总价: ¥" + oss.str();

    auto btn_ok = Button("  确认  ", screen.ExitLoopClosure());

    auto renderer = Renderer(btn_ok, [&] {
        return vbox({
                   text("书籍总价计算") | bold | center,
                   separator(),
                   text(total_str) | color(Color::Green) | center,
                   separator(),
                   btn_ok->Render() | center,
               }) |
               border | center;
    });

    screen.Loop(renderer);
}
