//
// Created by Zhang Yulei on 7/31/21.
//

#include "EventDump/EventDump.h"

using namespace std;

namespace {
    void PrintUsage() {
        cout << " Usage: " << endl;
        cout << " PDump <root file> <tree_name> <event number>" << endl;
        cout << " -- OPTIONS :" << endl;
        cout << " --              -h            print this help" << endl;
        cout << " --              event number: if no number specified, will print information for all events" << endl;
        cout << " --                            if number is larger than total entries, will print information for the last event" << endl;
        cout << endl;
    }
} // namespace

int main(int argc, char **argv) {

    long long event_number = -1;
    string file_name = "p_out.root";
    string tree_name = "p";

    if ((argc < 3 && argc > 0) || argc > 4) {
        PrintUsage();
        return 1;
    }
    if (argc >= 3) {
        file_name = argv[1];
        tree_name = argv[2];
    }
    if (argc == 4) event_number = stoll(argv[3]);

    EventDump ed(file_name, tree_name, event_number);
    ed.DumpEvent();

    return 0;
}