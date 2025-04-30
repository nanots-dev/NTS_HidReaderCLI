#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <set>
#include <iomanip>

extern "C" {
#include "hidapi.h"
}

// Display supported commands
void print_help() {
    std::cout << "Supported Commands:\n"
        << "  NTS_HidReader_CLI info:about\n"
        << "  NTS_HidReader_CLI devicelist\n"
        << "  NTS_HidReader_CLI getversion[n] (e.g. [0], [1])\n"
        << "    - If [n] is omitted, getversion reads the version of the first device.\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Syntax Error: Too much elements in the arguments.\n";
        print_help();
        return 1;
    }

    std::string command = argv[1];

    // info:about → version, authorship, contact, build info
    if (command == "info:about") {
        std::cout << "NTS_HidReader_CLI version 1.0.0\n";
        std::cout << "Designed and developed by the NanoTS TS Dev Team\n";
        std::cout << "Crafted with care by NH, for LnW\n";
        std::cout << "Contact: nhlee@nanots.co.kr\n";
        std::cout << "Build Date: " << __DATE__ << " " << __TIME__ << "\n";
        return 0;
    }

    if (command == "?" || command == "help") {
        print_help();
        return 0;
    }

    if (hid_init()) {
        std::cerr << "hid_init failed!\n";
        return 1;
    }

    const unsigned short TARGET_VID = 0x0914;
    std::vector<hid_device_info*> deviceList;
    std::set<std::pair<unsigned short, unsigned short>> seen;

    // Enumerate all HID devices and filter by Vendor ID
    struct hid_device_info* devs = hid_enumerate(0x0, 0x0);
    struct hid_device_info* cur = devs;

    while (cur) {
        if (cur->vendor_id == TARGET_VID) {
            std::pair<unsigned short, unsigned short> vidpid = { cur->vendor_id, cur->product_id };
            if (seen.find(vidpid) == seen.end()) {
                seen.insert(vidpid);
                deviceList.push_back(cur);
            }
        }
        cur = cur->next;
    }

    // devicelist command
    if (command == "devicelist") {
        if (deviceList.empty()) {
            std::cout << "No HID devices found with Vendor ID 0x0914.\n";
        }
        else {
            for (size_t i = 0; i < deviceList.size(); ++i) {
                std::wcout << L"[" << i << L"] "
                    << L"VendorID: 0x" << std::hex << deviceList[i]->vendor_id
                    << L" | ProductID: 0x" << deviceList[i]->product_id
                    << L" | ProductString: "
                    << (deviceList[i]->product_string ? deviceList[i]->product_string : L"<unavailable>")
                    << std::endl;
            }
        }
        hid_free_enumeration(devs);
        hid_exit();
        return 0;
    }

    // getversion or getversion[n] command
    std::regex versionWithIndex(R"(^getversion\[(\d+)\]$)");
    std::smatch match;
    int index = -1;

    if (command == "getversion") {
        index = 0;
    }
    else if (std::regex_match(command, match, versionWithIndex)) {
        index = std::stoi(match[1]);
    }

    if (index != -1) {
        if (index >= 0 && index < static_cast<int>(deviceList.size())) {
            const auto* dev = deviceList[index];
            unsigned short release = dev->release_number;
            int major = (release >> 8) & 0xFF;
            int minor = release & 0xFF;
            std::cout << "v" << major << "." << std::setw(2) << std::setfill('0') << minor << std::endl;
        }
        else {
            std::cout << "Invalid device index.\n";
        }

        hid_free_enumeration(devs);
        hid_exit();
        return 0;
    }

    std::cout << "Unknown command: " << command << "\n";
    print_help();

    hid_free_enumeration(devs);
    hid_exit();
    return 1;
}
