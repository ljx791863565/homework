#include "stdafx.h"
#include "selectserver.h"

using namespace std;

int main(int argc, char *argv[])
{
    uint16_t port = 2020;
    if (argc >= 2) {
        port = atoi(argv[1]);
    }

    SelectServer server;
    do {
        if (!server.init()) {
            cout << "server.init() fail" << endl;
        }
        cout << "server.init() ok" << endl;
        if (!server._listen(port)) {
            cout << "server._listen() fail" << endl;
        }
        cout << "server._listen() ok on " << port << endl;
        while (server.pulse()) {
            usleep(1000);
        }
    } while (false);

    return 0;
}
