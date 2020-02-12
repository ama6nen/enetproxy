#pragma once
#include "gt.hpp"
#include "server.h"
#include "packet.h"
#include "utils.h"

std::string gt::version = "3.01";
std::string gt::flag = "lt";
bool gt::resolving_uid = false;
bool gt::resolving_uid2 = false;
bool gt::connecting = false;
bool gt::in_game = false;
bool gt::ghost = false;

void gt::send_log(std::string text) {
    g_server->send(true, "action|log\nmsg|" + text, NET_MESSAGE_GAME_MESSAGE);
}

void gt::resolve_uid_to_name(std::string uid) {
    g_server->send(false, "action|friends");
    g_server->send(false, "action|dialog_return\ndialog_name|friends_guilds\nbuttonClicked|show_apprentices");
    g_server->send(false, "action|dialog_return\ndialog_name|show_mentees\nbuttonClicked|" + uid);
    resolving_uid = true;
}

void gt::solve_captcha(std::string text) {
    utils::replace(text,"set_default_color|`o\nadd_label_with_icon|big|`wAre you Human?``|left|206|\nadd_spacer|small|\nadd_textbox|What will be the sum of the following numbers|left|\nadd_textbox|", "");
    utils::replace(text,"|left|\nadd_text_input|captcha_answer|Answer:||32|\nend_dialog|captcha_submit||Submit|", "");
    auto sayi1 = text.substr(0, text.find(" +"));
    auto sayi2 = text.substr(sayi1.length() + 3, text.length());
    int cevap = std::stoi(sayi1) + std::stoi(sayi2);
    PRINTC("Captcha Solved!");
    g_server->send(false, "action|dialog_return\ndialog_name|captcha_submit\ncaptcha_answer|" + std::to_string(cevap));
    g_server->send(true, "action|dialog_return\ndialog_name|captcha_submit\ncaptcha_answer|" + std::to_string(cevap));
}
