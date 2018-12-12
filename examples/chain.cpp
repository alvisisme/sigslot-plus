/**
 * 
 * 网络状态变化(1)-->用户状态变化(2)-->通知程序显示(3)
 * 
 */
#include <iostream>
#include <string>
#include "../sigslot.h"

class Screen: public sigslot::has_slots<>
{
public:
    void Show(std::string user, bool status)
    {
        std::cout << "user " << user << " is " << (status ? "online" : "offline") << std::endl;
    };
};

/** 接收端定义公开的 get 和 set 函数，提供给源端调用，需要继承 sigslot::has_slots */
class User: public sigslot::has_slots<>
{
private:
    std::string userName;
    bool onlineStatus;
public:
    User(std::string name)
    {
        userName = name;
        onlineStatus = false;
    };
    bool GetOnlineStatus()
    {
        return onlineStatus;
    };
    void SetOnlineStatus(bool status)
    {
        onlineStatus = status;
        NotifyScreen(userName, onlineStatus);
    };

    sigslot::signal2<std::string,bool> NotifyScreen;
};

class Network
{
public:
    sigslot::signal1<bool> ChangeNetworkStatus;
};

int main(int argc, char* argv[])
{
    Screen screen;
    User bob("Bob");
    Network network;

    network.ChangeNetworkStatus.connect(&bob, &User::SetOnlineStatus);
    bob.NotifyScreen.connect(&screen, &Screen::Show);


    network.ChangeNetworkStatus(true);
    network.ChangeNetworkStatus(false);

    bob.NotifyScreen.disconnect(&screen);
    network.ChangeNetworkStatus.disconnect(&bob);
    return 0;
}