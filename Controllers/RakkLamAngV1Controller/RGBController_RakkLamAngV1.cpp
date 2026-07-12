#include "RGBController_RakkLamAngV1.h"

RGBController_RakkLamAngV1::RGBController_RakkLamAngV1(RakkLamAngV1Controller* controller_ptr)
{
    controller = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "RAKK";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "RAKK Lam-Ang Lite V1 Keyboard Device";
    location    = controller->GetLocation();
    serial      = controller->GetSerial();

    SetupZones();
}

RGBController_RakkLamAngV1::~RGBController_RakkLamAngV1()
{
    delete controller;
}

void RGBController_RakkLamAngV1::SetupZones()
{
    // Define 5 rows and 17 columns matrix map
    // 0xFFFFFFFF = unused position
    static unsigned int matrix_map_data[5][17] =
    {
        // Row 0 (F-row): 16 keys
        {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 0xFFFFFFFF },
        // Row 1 (Num-row): 17 keys
        { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
        // Row 2 (Q-row): 17 keys
        { 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49 },
        // Row 3 (A-row): 14 keys
        { 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF },
        // Row 4 (Z-row): 4 keys
        { 63, 64, 65, 66, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF }
    };

    zone new_zone;
    new_zone.name       = "Keyboard Matrix";
    new_zone.type       = ZONE_TYPE_MATRIX;
    new_zone.leds_min   = 68;
    new_zone.leds_max   = 68;
    new_zone.leds_count = 68;
    new_zone.matrix_map.Set(5, 17, (unsigned int *)&matrix_map_data);

    zones.push_back(new_zone);


    // List of key names matching the XML
    const char* led_names[68] = {
        "Key: Escape", "Key: F1", "Key: F2", "Key: F3", "Key: F4", "Key: F5", "Key: F6", "Key: F7", "Key: F8", "Key: F9", "Key: F10", "Key: F11", "Key: F12", "Key: Print Screen", "Key: Scroll Lock", "Key: Pause",
        "Key: Grave Accent", "Key: 1", "Key: 2", "Key: 3", "Key: 4", "Key: 5", "Key: 6", "Key: 7", "Key: 8", "Key: 9", "Key: 0", "Key: Minus", "Key: Equals", "Key: Backspace", "Key: Insert", "Key: Home", "Key: Page Up",
        "Key: Tab", "Key: Q", "Key: W", "Key: E", "Key: R", "Key: T", "Key: Y", "Key: U", "Key: I", "Key: O", "Key: P", "Key: Left Bracket", "Key: Right Bracket", "Key: Backslash", "Key: Delete", "Key: End", "Key: Page Down",
        "Key: Caps Lock", "Key: A", "Key: S", "Key: D", "Key: F", "Key: G", "Key: H", "Key: J", "Key: K", "Key: L", "Key: Semicolon", "Key: Apostrophe", "Key: Enter",
        "Key: Left Shift", "Key: Z", "Key: X", "Key: C"
    };

    for (int i = 0; i < 68; ++i)
    {
        led new_led;
        new_led.name = led_names[i];
        leds.push_back(new_led);
    }

    // Set up modes
    mode custom_mode;
    custom_mode.name       = "Direct";
    custom_mode.value      = 0x12;
    custom_mode.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    custom_mode.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(custom_mode);

    active_mode = 0;
}

void RGBController_RakkLamAngV1::ResizeZone(int /*zone*/, int /*new_size*/)
{
    // Custom zones cannot be resized
}

void RGBController_RakkLamAngV1::DeviceUpdateLEDs()
{
    controller->SetColors(colors);
}

void RGBController_RakkLamAngV1::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RakkLamAngV1::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RakkLamAngV1::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_RakkLamAngV1::DeviceUpdateMode()
{
    // The device only supports custom mode via this driver
}
