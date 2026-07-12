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
    zone new_zone;
    new_zone.name       = "Keyboard Matrix";
    new_zone.type       = ZONE_TYPE_MATRIX;
    new_zone.leds_min   = 68;
    new_zone.leds_max   = 68;
    new_zone.leds_count = 68;
    new_zone.matrix_map = new matrix_map_type;

    // Define 5 rows and 17 columns matrix
    new_zone.matrix_map->height = 5;
    new_zone.matrix_map->width  = 17;
    new_zone.matrix_map->map    = new unsigned int[5 * 17];

    // Initialize map with 0xFFFFFFFF (unused)
    for (int i = 0; i < 5 * 17; ++i)
    {
        new_zone.matrix_map->map[i] = 0xFFFFFFFF;
    }

    // Row 0 (F-row): 16 keys. (Col 0 to 15)
    // Esc (0), F1-F4 (1-4), F5-F8 (5-8), F9-F12 (9-12), Print (13), Scroll (14), Pause (15)
    for (unsigned int col = 0; col < 16; ++col)
    {
        new_zone.matrix_map->map[0 * 17 + col] = col;
    }

    // Row 1 (Num-row): 17 keys. (Col 0 to 16)
    // ~ to Backspace (16 to 29 -> indices 16 to 29), Insert (30), Home (31), PgUp (32)
    for (unsigned int col = 0; col < 17; ++col)
    {
        new_zone.matrix_map->map[1 * 17 + col] = 16 + col;
    }

    // Row 2 (Q-row): 17 keys. (Col 0 to 16)
    // Tab to | (33 to 46 -> indices 33 to 46), Del (47), End (48), PgDn (49)
    for (unsigned int col = 0; col < 17; ++col)
    {
        new_zone.matrix_map->map[2 * 17 + col] = 33 + col;
    }

    // Row 3 (A-row): 14 keys. (Col 0 to 13)
    // Caps Lock to Enter (50 to 63 -> indices 50 to 63)
    for (unsigned int col = 0; col < 14; ++col)
    {
        new_zone.matrix_map->map[3 * 17 + col] = 50 + col;
    }

    // Row 4 (Z-row): 4 keys. (Col 0 to 3)
    // Left Shift, Z, X, C (64 to 67 -> indices 64 to 67)
    for (unsigned int col = 0; col < 4; ++col)
    {
        new_zone.matrix_map->map[4 * 17 + col] = 64 + col;
    }

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
