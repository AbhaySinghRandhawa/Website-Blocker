#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>  // For Sleep function

using namespace std;

const string HOSTS_FILE_PATH = "C:\\Windows\\System32\\drivers\\etc\\hosts";
const string REDIRECT_IP = "127.0.0.1";

void blockWebsites(const vector<string>& websites) {
    ofstream hostsFile;

    // Open the hosts file in append mode
    hostsFile.open(HOSTS_FILE_PATH, ios::app);

    if (!hostsFile.is_open()) {
        cerr << "Error: Could not open the hosts file. Try running this program as Administrator." << endl;
        return;
    }

    // Append each website to the hosts file, redirecting to 127.0.0.1
    for (const auto& website : websites) {
        hostsFile << REDIRECT_IP << " " << website << endl;
        cout << "Blocked: " << website << endl;
    }

    hostsFile.close();
    cout << "Websites successfully blocked!" << endl;
}

void unblockWebsites(const vector<string>& websites) {
    ifstream hostsFileIn(HOSTS_FILE_PATH);
    ofstream hostsFileOut("C:\\Windows\\System32\\drivers\\etc\\temp_hosts");

    if (!hostsFileIn.is_open()) {
        cerr << "Error: Could not open the hosts file for reading." << endl;
        return;
    }

    string line;
    while (getline(hostsFileIn, line)) {
        bool shouldBlock = false;

        // Check if the line contains any website that needs to be unblocked
        for (const auto& website : websites) {
            if (line.find(website) != string::npos) {
                shouldBlock = true;
                break;
            }
        }

        // If the line doesn't contain any website, write it to the temp file
        if (!shouldBlock) {
            hostsFileOut << line << endl;
        }
    }

    hostsFileIn.close();
    hostsFileOut.close();

    // Replace the old hosts file with the new one
    remove(HOSTS_FILE_PATH.c_str());
    rename("C:\\Windows\\System32\\drivers\\etc\\temp_hosts", HOSTS_FILE_PATH.c_str());

    cout << "Websites successfully unblocked!" << endl;
}

int main() {
    vector<string> websitesToBlock = {
        "www.facebook.com",
        "www.youtube.com"
    };

    int choice;
    cout << "Website Blocker\n";
    cout << "1. Block websites\n";
    cout << "2. Unblock websites\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        blockWebsites(websitesToBlock);
    } else if (choice == 2) {
        unblockWebsites(websitesToBlock);
    } else {
        cerr << "Invalid choice" << endl;
    }

    return 0;
}
