#include "events.h"
#include "gt.hpp"
#include "proton/hash.hpp"
#include "proton/rtparam.hpp"
#include "proton/variant.hpp"
#include "server.h"
#include "utils.h"
#include <thread>
#include <limits.h>

bool events::out::variantlist(gameupdatepacket_t* packet) {
    variantlist_t varlist{};
    varlist.serialize_from_mem(utils::get_extended(packet));
    PRINTS("varlist: %s\n", varlist.print().c_str());
    return false;
}

bool events::out::pingreply(gameupdatepacket_t* packet) {
    //since this is a pointer we do not need to copy memory manually again
    packet->m_vec2_x = 1000.f;  //gravity
    packet->m_vec2_y = 250.f;   //move speed
    packet->m_vec_x = 64.f;     //punch range
    packet->m_vec_y = 64.f;     //build range
    packet->m_jump_amount = 0;  //for example unlim jumps set it to high which causes ban
    packet->m_player_flags = 0; //effect flags. good to have as 0 if using mod noclip, or etc.
    return false;
}

bool find_command(std::string chat, std::string name) {
    bool found = chat.find("/" + name) == 0;
    if (found)
        gt::send_log("`6" + chat);
    return found;
}
bool wrench = false;
bool fastdrop = false;
bool fasttrash = false;
std::string mode = "pull";
bool events::out::generictext(std::string packet) {
    PRINTS("Generic text: %s\n", packet.c_str());
    auto& world = g_server->m_world;
    rtvar var = rtvar::parse(packet);
    if (!var.valid())
        return false;
        if (wrench == true) {
            if (packet.find("action|wrench") != -1) {
                g_server->send(false, packet);
                std::string sr = packet.substr(packet.find("netid|") + 6, packet.length() - packet.find("netid|") - 1);
                std::string motion = sr.substr(0, sr.find("|"));
                if (mode.find("pull") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + motion + "|\nnetID|" + motion + "|\nbuttonClicked|pull");
                }
                if (mode.find("kick") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + motion + "|\nnetID|" + motion + "|\nbuttonClicked|kick");
                }
                if (mode.find("ban") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + motion + "|\nnetID|" + motion + "|\nbuttonClicked|worldban");
                }
                return true;
            }
        }
    if (var.get(0).m_key == "action" && var.get(0).m_value == "input") {
        if (var.size() < 2)
            return false;
        if (var.get(1).m_values.size() < 2)
            return false;

        if (!world.connected)
            return false;

        auto chat = var.get(1).m_values[1];
        if (find_command(chat, "name ")) { //ghetto solution, but too lazy to make a framework for commands.
            std::string name = "``" + chat.substr(6) + "``";
            variantlist_t va{ "OnNameChanged" };
            va[1] = name;
            g_server->send(true, va, world.local.netid, -1);
            gt::send_log("name set to: " + name);
            return true;
        } else if (find_command(chat, "flag ")) {
            int flag = atoi(chat.substr(6).c_str());
            variantlist_t va{ "OnGuildDataChanged" };
            va[1] = 1;
            va[2] = 2;
            va[3] = flag;
            va[4] = 3;
            g_server->send(true, va, world.local.netid, -1);
            gt::send_log("flag set to item id: " + std::to_string(flag));
            return true;
        } else if (find_command(chat, "ghost")) {
            gt::ghost = !gt::ghost;
            if (gt::ghost)
                gt::send_log("Ghost is now enabled.");
            else
                gt::send_log("Ghost is now disabled.");
            return true;
        } else if (find_command(chat, "country ")) {
            std::string cy = chat.substr(9);
            gt::flag = cy;
            gt::send_log("your country set to " + cy + ", (Relog to game to change it successfully!)");
            return true;
        }
        else if (find_command(chat, "fd")) {
            fastdrop = !fastdrop;
            if (fastdrop)
                gt::send_log("Fast Drop is now enabled.");
            else
                gt::send_log("Fast Drop is now disabled.");
            return true;
        }
        else if (find_command(chat, "ft")) {
            fasttrash = !fasttrash;
            if (fasttrash)
                gt::send_log("Fast Trash is now enabled.");
            else
                gt::send_log("Fast Trash is now disabled.");
            return true;
        }        
        else if (find_command(chat, "wrenchset ")) {
            mode = chat.substr(10);
            gt::send_log("Wrench mode set to " + mode);
            return true;        
        }
        else if (find_command(chat, "wrenchmode")) {
            wrench = !wrench;
            if (wrench)
                gt::send_log("Wrench mode is on.");
            else
                gt::send_log("Wrench mode is off.");
            return true;
        }
        else if (find_command(chat, "uid ")) {
            std::string name = chat.substr(5);
            gt::send_log("resolving uid for " + name);
            g_server->send(false, "action|input\n|text|/ignore " + name);
            g_server->send(false, "action|friends");
            g_server->send(false, "action|dialog_return\ndialog_name|playerportal\nbuttonClicked|socialportal");
            g_server->send(false, "action|dialog_return\ndialog_name|friends_guilds\nbuttonClicked|showfriend");
            g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|friend_all");
            gt::resolving_uid2 = true;
            return true;
        } else if (find_command(chat, "tp ")) {
            std::string name = chat.substr(4);
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
                if (name_2.find(name) == 0) {
                    gt::send_log("Teleporting to " + player.name);
                    variantlist_t varlist{ "OnSetPos" };
                    varlist[1] = player.pos;
                    g_server->m_world.local.pos = player.pos;
                    g_server->send(true, varlist, g_server->m_world.local.netid, -1);
                    break;
                }
            }
            return true;
        } else if (find_command(chat, "warp ")) {
            std::string name = chat.substr(6);
            gt::send_log("`7Warping to " + name);
            g_server->send(false, "action|join_request\nname|" + name, 3);
            return true;
           } else if (find_command(chat, "pullall")) {
            std::string username = chat.substr(6);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                if (name_2.find(username)) {
                    g_server->send(false, "action|wrench\n|netid|" + std::to_string(player.netid));
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(player.netid) + "|\nbuttonClicked|pull"); 
                    // You Can |kick |trade |worldban 
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    gt::send_log("Pulled");
                  
                }
            }
            return true;
        } else if (find_command(chat, "skin ")) {
            int skin = atoi(chat.substr(6).c_str());
            variantlist_t va{ "OnChangeSkin" };
            va[1] = skin;
            g_server->send(true, va, world.local.netid, -1);
            return true;
        } else if (find_command(chat, "wrench ")) {
            std::string name = chat.substr(6);
            std::string username = ".";
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2);
                std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
                    g_server->send(false, "action|wrench\n|netid|" + std::to_string(player.netid));
            }
            return true;
        } else if (find_command(chat, "proxy")) {
            gt::send_log(
                "/tp [name] (teleports to a player in the world), /ghost (toggles ghost, you wont move for others when its enabled), /uid "
                "[name] (resolves name to uid), /flag [id] (sets flag to item id), /name [name] (sets name to name)");
            return true;
        } 
        return false;
    }

    if (packet.find("game_version|") != -1) {
        rtvar var = rtvar::parse(packet);
        auto mac = utils::generate_mac();
        auto hash_str = mac + "RT";
        auto hash2 = utils::hash((uint8_t*)hash_str.c_str(), hash_str.length());
        var.set("mac", mac);
        var.set("wk", utils::generate_rid());
        var.set("rid", utils::generate_rid());
        var.set("fz", std::to_string(utils::random(INT_MIN, INT_MAX)));
        var.set("zf", std::to_string(utils::random(INT_MIN, INT_MAX)));
        var.set("hash", std::to_string(utils::random(INT_MIN, INT_MAX)));
        var.set("hash2", std::to_string(hash2));
        var.set("meta", utils::random(utils::random(6, 10)) + ".com");
        var.set("game_version", gt::version);
        var.set("country", gt::flag);

        /*
        AAP Bypass
        Only making this public because after 1 month being reported to ubi, nothing happened
        Then after a month (around 15.3) it got fixed for a whole single 1 day, and they publicly said it had been fixed
        And at that time we shared how to do it because thought its useless, and then aap bypass started working again
        and then 9999 new aap bypass services came to be public, and even playingo started selling it so no point keeping it private
        With publishing this I hope ubi actually does something this time
        */

        //Finally patched, I guess they finally managed to fix this after maybe a year!

        //if (var.find("tankIDName") && gt::aapbypass) {
        //    var.find("mac")->m_values[0] = "02:00:00:00:00:00";
        //    var.find("platformID")->m_values[0] = "4"; //android
        //    var.remove("fz");
        //    var.remove("rid");
        //}

        packet = var.serialize();
        gt::in_game = false;
        PRINTS("Spoofing login info\n");
        g_server->send(false, packet);
        return true;
    }

    return false;
}

bool events::out::gamemessage(std::string packet) {
    PRINTS("Game message: %s\n", packet.c_str());
    if (packet == "action|quit") {
        g_server->quit();
        return true;
    }

    return false;
}

bool events::out::state(gameupdatepacket_t* packet) {
    if (!g_server->m_world.connected)
        return false;

    g_server->m_world.local.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
    PRINTS("local pos: %.0f %.0f\n", packet->m_vec_x, packet->m_vec_y);

    if (gt::ghost)
        return true;
    return false;
}

bool events::in::variantlist(gameupdatepacket_t* packet) {
    variantlist_t varlist{};
    auto extended = utils::get_extended(packet);
    extended += 4; //since it casts to data size not data but too lazy to fix this
    varlist.serialize_from_mem(extended);
    PRINTC("varlist: %s\n", varlist.print().c_str());
    auto func = varlist[0].get_string();

    //probably subject to change, so not including in switch statement.
    if (func.find("OnSuperMainStartAcceptLogon") != -1)
        gt::in_game = true;

    switch (hs::hash32(func.c_str())) {
        //solve captcha
        case fnv32("onShowCaptcha"): {
            auto menu = varlist[1].get_string();
            gt::solve_captcha(menu);
            return true;
        } break;
        case fnv32("OnRequestWorldSelectMenu"): {
            auto& world = g_server->m_world;
            world.players.clear();
            world.local = {};
            world.connected = false;
            world.name = "EXIT";
        } break;
        case fnv32("OnSendToServer"): g_server->redirect_server(varlist); return true;

        case fnv32("OnConsoleMessage"): {
            varlist[1] = "`4[PROXY]`` " + varlist[1].get_string();
            g_server->send(true, varlist);
            return true;
        } break;
        case fnv32("OnDialogRequest"): {
            auto content = varlist[1].get_string();

            if (content.find("set_default_color|`o") != -1) 
            {
                if (content.find("end_dialog|captcha_submit||Submit|") != -1) 
                {
                    gt::solve_captcha(content);
                    return true;
                }
                if (content.find("add_label_with_icon|big|`wThe Growtopia Gazette") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|gazette");
                    return true;
                }
            }
        if (wrench == true) {
            if (content.find("add_button|report_player|`wReport Player``|noflags|0|0|") != -1) {
                if (content.find("embed_data|netID") != -1) {
                    return true; // block wrench dialog
                }
            }
        }
        if (fastdrop == true) {
            std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
            std::string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
            if (content.find("embed_data|itemID|") != -1) {
                if (content.find("Drop") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + count);
                    return true;
                }
            }
        }
        if (fasttrash == true) {
            std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
            std::string count = content.substr(content.find("you have ") + 9, content.length() - content.find("you have ") - 1);
            std::string delimiter = ")";
            std::string token = count.substr(0, count.find(delimiter));
            if (content.find("embed_data|itemID|") != -1) {
                if (content.find("Trash") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|trash_item\nitemID|" + itemid + "|\ncount|" + token);
                    return true;
                }
            }
                    
            //hide unneeded ui when resolving
            //for the /uid command

            } else if (gt::resolving_uid2 && content.find("`4Stop ignoring") != -1) {
                int pos = content.rfind("|`4Stop ignoring");
                auto ignore_substring = content.substr(0, pos);
                auto uid = ignore_substring.substr(ignore_substring.rfind("add_button|") + 11);
                auto uid_int = atoi(uid.c_str());
                if (uid_int == 0) {
                    gt::resolving_uid2 = false;
                    gt::send_log("name resolving seems to have failed.");
                } else {
                    gt::send_log("Target UID: " + uid);
                    g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|" + uid);
                    g_server->send(false, "action|dialog_return\ndialog_name|friends_remove\nfriendID|" + uid + "|\nbuttonClicked|remove");
                }
                return true;
            }
        } break;
        case fnv32("OnRemove"): {
            auto text = varlist.get(1).get_string();
            if (text.find("netID|") == 0) {
                auto netid = atoi(text.substr(6).c_str());

                if (netid == g_server->m_world.local.netid)
                    g_server->m_world.local = {};

                auto& players = g_server->m_world.players;
                for (size_t i = 0; i < players.size(); i++) {
                    auto& player = players[i];
                    if (player.netid == netid) {
                        players.erase(std::remove(players.begin(), players.end(), player), players.end());
                        break;
                    }
                }
            }
        } break;
        case fnv32("OnSpawn"): {
            std::string meme = varlist.get(1).get_string();
            rtvar var = rtvar::parse(meme);
            auto name = var.find("name");
            auto netid = var.find("netID");
            auto onlineid = var.find("onlineID");

            if (name && netid && onlineid) {
                player ply{};

                if (var.find("invis")->m_value != "1") {
                    ply.name = name->m_value;
                    ply.country = var.get("country");
                    name->m_values[0] += " `4[" + netid->m_value + "]``";
                    auto pos = var.find("posXY");
                    if (pos && pos->m_values.size() >= 2) {
                        auto x = atoi(pos->m_values[0].c_str());
                        auto y = atoi(pos->m_values[1].c_str());
                        ply.pos = vector2_t{ float(x), float(y) };
                    }
                } else {
                    ply.mod = true;
                    ply.invis = true;
                }
                if (var.get("mstate") == "1" || var.get("smstate") == "1")
                    ply.mod = true;
                ply.userid = var.get_int("userID");
                ply.netid = var.get_int("netID");
                if (meme.find("type|local") != -1) {
                    //set mod state to 1 (allows infinite zooming, this doesnt ban cuz its only the zoom not the actual long punch)
                    var.find("mstate")->m_values[0] = "1";
                    g_server->m_world.local = ply;
                }
                g_server->m_world.players.push_back(ply);
                auto str = var.serialize();
                utils::replace(str, "onlineID", "onlineID|");
                varlist[1] = str;
                PRINTC("new: %s\n", varlist.print().c_str());
                g_server->send(true, varlist, -1, -1);
                return true;
            }
        } break;
    }
    return false;
}

bool events::in::generictext(std::string packet) {
    PRINTC("Generic text: %s\n", packet.c_str());
    return false;
}

bool events::in::gamemessage(std::string packet) {
    PRINTC("Game message: %s\n", packet.c_str());

    if (gt::resolving_uid2) {
        if (packet.find("PERSON IGNORED") != -1) {
            g_server->send(false, "action|dialog_return\ndialog_name|friends_guilds\nbuttonClicked|showfriend");
            g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|friend_all");
        } else if (packet.find("Nobody is currently online with the name") != -1) {
            gt::resolving_uid2 = false;
            gt::send_log("Target is offline, cant find uid.");
        } else if (packet.find("Clever perhaps") != -1) {
            gt::resolving_uid2 = false;
            gt::send_log("Target is a moderator, can't ignore them.");
        }
    }
    return false;
}

bool events::in::sendmapdata(gameupdatepacket_t* packet) {
    g_server->m_world = {};
    auto extended = utils::get_extended(packet);
    extended += 4;
    auto data = extended + 6;
    auto name_length = *(short*)data;

    char* name = new char[name_length + 1];
    memcpy(name, data + sizeof(short), name_length);
    char none = '\0';
    memcpy(name + name_length, &none, 1);

    g_server->m_world.name = std::string(name);
    g_server->m_world.connected = true;
    delete[] name;
    PRINTC("world name is %s\n", g_server->m_world.name.c_str());
    return false;
}

bool events::in::state(gameupdatepacket_t* packet) {
    if (!g_server->m_world.connected)
        return false;
    if (packet->m_player_flags == -1)
        return false;

    auto& players = g_server->m_world.players;
    for (auto& player : players) {
        if (player.netid == packet->m_player_flags) {
            player.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
            PRINTC("player %s position is %.0f %.0f\n", player.name.c_str(), player.pos.m_x, player.pos.m_y);
            break;
        }
    }
    return false;
}

bool events::in::tracking(std::string packet) {
    PRINTC("Tracking packet: %s\n", packet.c_str());
    return true;
}
