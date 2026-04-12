#include "user_ui.h"

#include <iomanip>
#include <sstream>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>

using namespace ftxui;

int showUserMenu(const std::string& username, double balance, bool isVIP) {
    auto screen = ScreenInteractive::TerminalOutput();

    std::ostringstream bal_oss;
    bal_oss << std::fixed << std::setprecision(2) << balance;

    std::vector<std::string> entries = {
        "  浏览书籍  ",
        "  购买书籍  ",
        "  申请VIP   ",
        "  充值余额  ",
        "  查看购物车",
        "  返回      ",
    };
    int selected = 0;

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    auto menu = Menu(&entries, &selected, option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
                   text("用户中心") | bold | center,
                   separator(),
                   hbox({
                       text("用户: ") | bold,
                       text(username) | color(Color::Cyan),
                       filler(),
                       text("余额: ¥" + bal_oss.str()) | color(Color::Green),
                       text("  "),
                       text(isVIP ? "【VIP会员】" : "【普通用户】") |
                           color(isVIP ? Color::Yellow : Color::GrayLight),
                   }),
                   separator(),
                   menu->Render(),
               }) |
               border | center;
    });

    screen.Loop(renderer);
    return selected;
}

BuyBookData showBuyBookForm(const std::vector<Book>& books) {
    auto screen = ScreenInteractive::TerminalOutput();

    BuyBookData data;
    std::string qty_str;
    std::string error_msg;

    auto input_id = Input(&data.bookId, "书籍编号");
    auto input_qty = Input(&qty_str, "购买数量");

    auto btn_ok = Button("  购买  ", [&] {
        if (data.bookId.empty()) {
            error_msg = "请输入书籍编号";
            return;
        }
        try {
            int qty = std::stoi(qty_str);
            if (qty <= 0) {
                error_msg = "数量必须大于0";
                return;
            }
            data.quantity = qty;
            error_msg.clear();
            data.confirmed = true;
            screen.Exit();
        } catch (...) {
            error_msg = "购买数量格式不正确";
        }
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});

    auto container = Container::Vertical({input_id, input_qty, buttons});

    // Build mini book table for reference
    auto make_book_table = [&]() -> Element {
        if (books.empty()) {
            return text("暂无书籍") | center;
        }
        std::vector<std::vector<std::string>> table_data;
        table_data.push_back({"编号", "书名", "价格", "库存"});
        for (const auto& book : books) {
            std::ostringstream price_oss;
            price_oss << std::fixed << std::setprecision(2) << book.getPrice();
            table_data.push_back({book.getId(), book.getName(), price_oss.str(),
                                  std::to_string(book.getStock())});
        }
        auto tbl = Table(table_data);
        tbl.SelectAll().Border(LIGHT);
        tbl.SelectAll().SeparatorVertical(LIGHT);
        tbl.SelectAll().SeparatorHorizontal(LIGHT);
        tbl.SelectRow(0).Decorate(bold);
        tbl.SelectRow(0).Border(DOUBLE);
        tbl.SelectColumn(2).DecorateCells(align_right);
        tbl.SelectColumn(3).DecorateCells(align_right);
        return tbl.Render();
    };

    auto renderer = Renderer(container, [&] {
        Elements rows = {
            text("购买书籍") | bold | center,
            separator(),
            make_book_table(),
            separator(),
            hbox({text("书籍编号: "), input_id->Render() | flex}),
            hbox({text("购买数量: "), input_qty->Render() | flex}),
            separator(),
        };
        if (!error_msg.empty()) {
            rows.push_back(text(error_msg) | color(Color::Red));
            rows.push_back(separator());
        }
        rows.push_back(
            hbox({btn_ok->Render() | flex, btn_cancel->Render() | flex}));
        return vbox(rows) | border;
    });

    screen.Loop(renderer);
    return data;
}

double showChargeBalanceForm() {
    auto screen = ScreenInteractive::TerminalOutput();

    std::string amount_str;
    std::string error_msg;
    double result = 0.0;
    bool confirmed = false;

    auto input_amount = Input(&amount_str, "充值金额");

    auto btn_ok = Button("  确认  ", [&] {
        try {
            double amt = std::stod(amount_str);
            if (amt <= 0) {
                error_msg = "金额必须大于0";
                return;
            }
            result = amt;
            error_msg.clear();
            confirmed = true;
            screen.Exit();
        } catch (...) {
            error_msg = "金额格式不正确";
        }
    });
    auto btn_cancel = Button("  取消  ", screen.ExitLoopClosure());
    auto buttons = Container::Horizontal({btn_ok, btn_cancel});

    auto container = Container::Vertical({input_amount, buttons});

    // Allow pressing Enter in the input field to trigger the OK button
    container |= CatchEvent([&](Event event) {
        if (event == Event::Return && input_amount->Focused()) {
            btn_ok->OnEvent(Event::Return);
            return true;
        }
        return false;
    });

    auto renderer = Renderer(container, [&] {
        Elements rows = {
            text("充值余额") | bold | center,
            separator(),
            hbox({text("充值金额: ¥"), input_amount->Render() | flex}),
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

    if (!confirmed) return 0.0;
    return result;
}

void showCartView(const ShoppingCart& cart, bool isVIP) {
    auto screen = ScreenInteractive::TerminalOutput();

    auto btn_back = Button("  返回  ", screen.ExitLoopClosure());

    auto make_cart_table = [&]() -> Element {
        if (cart.isEmpty()) {
            return text("购物车为空") | center;
        }
        std::vector<std::vector<std::string>> table_data;
        table_data.push_back({"编号", "书名", "单价", "数量", "小计"});

        for (const auto& item : cart.getItems()) {
            std::ostringstream price_oss, sub_oss;
            price_oss << std::fixed << std::setprecision(2)
                      << item.book.getPrice();
            sub_oss << std::fixed << std::setprecision(2) << item.getSubtotal();
            table_data.push_back({item.book.getId(), item.book.getName(),
                                  price_oss.str(),
                                  std::to_string(item.quantity), sub_oss.str()});
        }

        auto tbl = Table(table_data);
        tbl.SelectAll().Border(LIGHT);
        tbl.SelectAll().SeparatorVertical(LIGHT);
        tbl.SelectAll().SeparatorHorizontal(LIGHT);
        tbl.SelectRow(0).Decorate(bold);
        tbl.SelectRow(0).Border(DOUBLE);
        tbl.SelectColumn(2).DecorateCells(align_right);
        tbl.SelectColumn(3).DecorateCells(align_right);
        tbl.SelectColumn(4).DecorateCells(align_right);
        return tbl.Render();
    };

    auto renderer = Renderer(btn_back, [&] {
        Elements content;
        content.push_back(text("购物车") | bold | center);
        content.push_back(separator());
        content.push_back(make_cart_table());

        if (!cart.isEmpty()) {
            double total = isVIP ? cart.getTotalPriceWithDiscount(0.9)
                                 : cart.getTotalPrice();
            std::ostringstream total_oss;
            total_oss << std::fixed << std::setprecision(2) << total;
            std::string total_label = "总计: ¥" + total_oss.str();
            if (isVIP) total_label += "  (VIP 9折)";
            content.push_back(
                text(total_label) | bold | align_right | color(Color::Green));
        }

        content.push_back(separator());
        content.push_back(btn_back->Render() | center);
        return vbox(content) | border;
    });

    screen.Loop(renderer);
}
