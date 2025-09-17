// SPDX-FileCopyrightText: 2025 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileComment: Host-Demo für Backlight-Steuerung via USB HID (finale, korrigierte Version)
// SPDX-FileType: SOURCE

#include <cstdint>
#include <cstring>
#include <hidapi/hidapi.h>
#include <iomanip>
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// --- Varikey Firmware Defines ---
constexpr unsigned short VARIKEY_VID = 0xabcd;
constexpr unsigned short VARIKEY_PID = 0x0064;

constexpr uint8_t REPORT_ID_CUSTOM = 0xE0;
constexpr uint8_t REPORT_TYPE_BACKLIGHT = 0x01;

enum class BacklightProgram : uint8_t
{
    ALERT = 0x00,
    CONST = 0x01,
    MEDIUM = 0x02,
    MORPH = 0x03,
    MOUNT = 0x04,
    OFF = 0x05,
    SET = 0x06,
    SLOW = 0x07,
    SUSPEND = 0x08,
    TURBO = 0x09,
    UNDEFINED = 0xff,
};

enum class CHANNEL : uint8_t
{
    TOP_LIGHT = 0x00,
    BOTTOM_LIGHT = 0x01,
    UNDEFINED = 0xff,
};

constexpr size_t REPORT_BUFFER_SIZE = 65;

// --- Hilfsfunktionen ---
void hexdump(const unsigned char *data, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i] << " ";
    }
    std::cout << std::dec << std::endl;
}

// --- Hauptfunktionen ---
bool send_backlight_command(hid_device *dev, BacklightProgram program, uint8_t r, uint8_t g, uint8_t b)
{
    unsigned char report[REPORT_BUFFER_SIZE] = {0};

    // Byte 0: Report ID (wird von hid_write() verwendet und von TinyUSB entfernt)
    report[0] = REPORT_ID_CUSTOM;

    // Ab hier beginnt die Payload, die an `set_report_handler` übergeben wird.
    // Byte 1 (Payload[0]): Identifier für `case SET_REPORT::BACKLIGHT`
    report[1] = REPORT_TYPE_BACKLIGHT;

    // Byte 2 (Payload[1]): Beginn der `backlight_t` Struktur für memcpy
    report[2] = static_cast<uint8_t>(program); // program
    report[3] = static_cast<uint8_t>(CHANNEL::BOTTOM_LIGHT); // channel
    report[4] = r;                             // left.r
    report[5] = g;                             // left.g
    report[6] = b;                             // left.b
    report[7] = r;                             // right.r (identisch)
    report[8] = g;                             // right.g
    report[9] = b;                             // right.b

    // Gesamtlänge für hid_write: 1 (ReportID) + 8 (Payload für set_report_handler) = 9 Bytes
    const size_t data_length_to_send = 9;

    std::cout << "Sende final korrigierten Output Report (Länge " << data_length_to_send << " Bytes):" << std::endl;
    hexdump(report, data_length_to_send);

    int res = hid_write(dev, report, data_length_to_send);
    if (res < 0)
    {
        std::wcerr << L"hid_write fehlgeschlagen: " << hid_error(dev) << std::endl;
        return false;
    }

    std::cout << "Report erfolgreich gesendet." << std::endl;
    return true;
}

int main(int argc, char *argv[])
{
    if (hid_init() != 0)
    {
        std::cerr << "hidapi init fehlgeschlagen!" << std::endl;
        return 1;
    }

    hid_device *dev = hid_open(VARIKEY_VID, VARIKEY_PID, nullptr);
    if (!dev)
    {
        std::cerr << "Gerät nicht gefunden (VID: " << std::hex << VARIKEY_VID
                  << ", PID: " << VARIKEY_PID << ")" << std::endl;
        hid_exit();
        return 2;
    }
    std::cout << "Gerät verbunden!" << std::endl;

    const int pause_duration_s = 4;

    // --- Beispiel 1: Backlight auf BLAU setzen (CONST) ---
    std::cout << "\n[1] Setze Backlight auf BLAU (CONST)..." << std::endl;
    if (!send_backlight_command(dev, BacklightProgram::CONST, 0x00, 0x00, 0xFF))
    {
        // Fehlerbehandlung
    }

    // Längere Pause, damit man den Effekt sieht
#ifdef _WIN32
    Sleep(pause_duration_s * 1000);
#else
    sleep(pause_duration_s);
#endif

    // --- Beispiel 2: Backlight auf GELB morphen ---
    std::cout << "\n[2] Starte Morphing zu GELB..." << std::endl;
    if (!send_backlight_command(dev, BacklightProgram::MORPH, 0xFF, 0xFF, 0x00))
    {
        // Fehlerbehandlung
    }

#ifdef _WIN32
    Sleep(pause_duration_s * 1000);
#else
    sleep(pause_duration_s);
#endif

    // --- Beispiel 3: ALARM/ALERT in ROT starten ---
    std::cout << "\n[3] Starte ALARM/ALERT in ROT..." << std::endl;
    if (!send_backlight_command(dev, BacklightProgram::ALERT, 0xFF, 0x00, 0x00))
    {
        // Fehlerbehandlung
    }

#ifdef _WIN32
    Sleep(pause_duration_s * 1000);
#else
    sleep(pause_duration_s);
#endif

    // --- Beispiel 4: Backlight ausschalten ---
    std::cout << "\n[4] Schalte Backlight aus..." << std::endl;
    if (!send_backlight_command(dev, BacklightProgram::OFF, 0x00, 0x00, 0x00))
    {
        // Fehlerbehandlung
    }

    hid_close(dev);
    hid_exit();
    return 0;
}
