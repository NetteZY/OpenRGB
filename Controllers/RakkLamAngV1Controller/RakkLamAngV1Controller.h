#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBControllerInterface.h"

class RakkLamAngV1Controller
{
public:
    RakkLamAngV1Controller(hid_device* dev_handle, const char* path);
    ~RakkLamAngV1Controller();

    std::string     GetDeviceName();
    std::string     GetSerial();
    std::string     GetLocation();

    void            SetColors(const std::vector<RGBColor>& colors);
    void            SaveToFlash();

private:
    hid_device*     dev;
    std::string     location;

    void            SendCmd(const unsigned char* cmd_bytes, size_t len);
    void            GetKeyBlock(unsigned char key_id, unsigned char r, unsigned char g, unsigned char b, unsigned char* block);
};
