#include <iostream>
#include <string>

#include "sigslot.h"

using namespace std;
using namespace sigslot;

class Error: public has_slots<>
{
public:
    void onError(string msg, int code)
    {
        cout << code << ":" << msg << endl;
    }
};

int main(int argc, const char *argv[]) {

    signal2<string, int> ReportError;

    Error err;
    ReportError.connect(&err, &Error::onError);

    ReportError("Everything is OK", 0);

    return 0;
}