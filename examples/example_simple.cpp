#include <iostream>

#include "sigslot_simple.h"

using namespace std;
using namespace sigslot;

class Switch
{
public:
    signal1<bool> Clicked;
};

class Light: public has_slots<>
{
public:
    void onClick(bool on)
    {
        cout << "light " << "is " << (on ? "on" : "off") << endl;
    };
};

class TV: public has_slots<>
{
public:
    void onClick(bool on)
    {
        cout << "TV " << "is " << (on ? "on" : "off") << endl;
    };
};

int main(int argc, char* argv[])
{

    Switch sw;
    Light light;
    TV tv;

    // bind
    sw.Clicked.connect(&light, &Light::onClick);
    sw.Clicked.connect(&tv, &TV::onClick);

    bool on = true;
    bool off = false;
    sw.Clicked(on);
    sw.Clicked(off);

    // unbind
    sw.Clicked.disconnect(&light);
    sw.Clicked.disconnect(&tv);

    return 0;
}