# NTS_HidReader_CLI

**Version**: 1.0.0  
**Developed by**: NanoTS Touchscreen Development Team  
**Contact**: nhlee@nanots.co.kr

---

## 🔍 Overview

`NTS_HidReader_CLI` is a lightweight command-line tool designed for inspecting and verifying USB HID-based Touch Controller devices used in LnW systems.

This tool helps identify connected HID devices and display their firmware version information in a simple and script-friendly format.

---

## 🚀 Features

- Lists all connected NanoTS HID devices (Vendor ID: `0x0914`)
- Displays firmware version (`BcdDevice`) in `vX.YY` format
- Supports index-based or default device selection
- Build and compiler information shown via `info:about`

---

## 💻 Supported Commands

| Command                   | Description                                                   |
|---------------------------|---------------------------------------------------------------|
| `info:about`              | Display tool version, author, build date, and compiler info  |
| `devicelist`              | List all connected NanoTS HID devices                        |
| `getversion[n]`           | Show firmware version of device at index `[n]` (e.g. `v0.01`) |
| `getversion`              | Show firmware version of the **first** connected device      |
| `?` or `help`             | Show command help                                             |

---

## 📦 Example Usage

```bash
> NTS_HidReader_CLI.exe devicelist
[0] VendorID: 0x0914 | ProductID: 0x1005 | ProductString: NanoTS Touch Controller

> NTS_HidReader_CLI.exe getversion[0]
v0.01

> NTS_HidReader_CLI.exe info:about
NTS_HidReader_CLI version 1.0.0
Designed and developed by the NanoTS Touchscreen Development Team
Crafted with care by NH, for LnW
Contact: nhlee@nanots.co.kr
Build Date: Apr 30 2025 12:00:00
C++ Standard: C++17
