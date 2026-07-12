#include "RakkLamAngV1Controller.h"
#include "LogManager.h"
#include <cstring>
#include <chrono>
#include <thread>

#define EVISION_KB_MAX_PACKET_SIZE 54

RakkLamAngV1Controller::RakkLamAngV1Controller(hid_device* dev_handle, const char* path)
{
    LOG_INFO("[RAKK Controller] Constructor called");
    dev = dev_handle;
    location = path;
    LOG_INFO("[RAKK Controller] Path: %s, dev: %p", location.c_str(), dev);
}

RakkLamAngV1Controller::~RakkLamAngV1Controller()
{
    if (dev)
    {
        SaveToFlash();
        hid_close(dev);
    }
}

std::string RakkLamAngV1Controller::GetDeviceName()
{
    return "RAKK Lam-Ang Lite V1";
}

std::string RakkLamAngV1Controller::GetSerial()
{
    return "";
}

std::string RakkLamAngV1Controller::GetLocation()
{
    return location;
}

void RakkLamAngV1Controller::SendCmd(const unsigned char* cmd_bytes, size_t len)
{
    LOG_INFO("[RAKK Controller] SendCmd len=%zu, cmd[0]=0x%02X", len, cmd_bytes[0]);
    unsigned char buf[65];
    memset(buf, 0, sizeof(buf));
    buf[0] = 0x00; // Report ID
    memcpy(&buf[1], cmd_bytes, len);

    LOG_INFO("[RAKK Controller] Calling hid_send_feature_report...");
    int ret = hid_send_feature_report(dev, buf, 65);
    LOG_INFO("[RAKK Controller] hid_send_feature_report returned: %d", ret);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    unsigned char resp[65];
    memset(resp, 0, sizeof(resp));
    resp[0] = 0x00;
    
    LOG_INFO("[RAKK Controller] Calling hid_get_feature_report...");
    int ret_get = hid_get_feature_report(dev, resp, 65);
    LOG_INFO("[RAKK Controller] hid_get_feature_report returned: %d", ret_get);
}

void RakkLamAngV1Controller::GetKeyBlock(unsigned char key_id, unsigned char r, unsigned char g, unsigned char b, unsigned char* block)
{
    unsigned char b8 = 0x01;
    unsigned char b9 = 0x0F;
    unsigned char b10 = 0x0A;
    unsigned char b11 = 0x00;

    if (key_id == 2)
    {
        b8 = 0x00;
    }
    else if (key_id == 3)
    {
        b8 = 0x00;
        b9 = 0x08;
    }
    else if (key_id == 10)
    {
        b11 = 0x03;
    }

    block[0]  = key_id;
    block[1]  = r;
    block[2]  = g;
    block[3]  = b;
    block[4]  = 0x00;
    block[5]  = 0x00;
    block[6]  = 0x00;
    block[7]  = 0x00;
    block[8]  = b8;
    block[9]  = b9;
    block[10] = b10;
    block[11] = b11;
    block[12] = 0x00;
    block[13] = 0x00;
    block[14] = 0xAA;
    block[15] = 0x55;
}

void RakkLamAngV1Controller::SetColors(const std::vector<RGBColor>& colors)
{
    LOG_INFO("[RAKK Controller] SetColors called with %zu colors", colors.size());
    // Ensure we have at least 68 colors
    std::vector<RGBColor> padded_colors = colors;
    if (padded_colors.size() < 68)
    {
        padded_colors.resize(68, 0);
    }

    LOG_INFO("[RAKK Controller] Sending Start Command...");
    // Step 1: Start Command
    const unsigned char start_cmd[] = { 0x04, 0x18, 0x00, 0x00 };
    SendCmd(start_cmd, sizeof(start_cmd));

    LOG_INFO("[RAKK Controller] Sending Set Mode Command...");
    // Step 2: Set Mode Command
    const unsigned char mode_cmd[] = { 0x04, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12 };
    SendCmd(mode_cmd, sizeof(mode_cmd));

    LOG_INFO("[RAKK Controller] Sending 17 color packets...");
    // Step 3: Send 17 packets of 4 key blocks (68 slots total)
    for (unsigned char p_idx = 0; p_idx < 17; ++p_idx)
    {
        unsigned char packet_data[65];
        memset(packet_data, 0, sizeof(packet_data));
        packet_data[0] = 0x00; // Report ID

        for (unsigned char k = 0; k < 4; ++k)
        {
            unsigned char key_id = p_idx * 4 + k + 1;
            RGBColor color = padded_colors[key_id - 1];
            unsigned char r = RGBGetRValue(color);
            unsigned char g = RGBGetGValue(color);
            unsigned char b = RGBGetBValue(color);

            GetKeyBlock(key_id, r, g, b, &packet_data[1 + k * 16]);
        }

        int ret = hid_send_feature_report(dev, packet_data, 65);
        if (ret < 0)
        {
            LOG_INFO("[RAKK Controller] Packet %d send failed: %d", p_idx, ret);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    LOG_INFO("[RAKK Controller] Sending Caps Lock packet...");
    // Step 4: Send 18th packet (Caps Lock key - Key 3)
    unsigned char packet18[65];
    memset(packet18, 0, sizeof(packet18));
    packet18[0] = 0x00; // Report ID

    RGBColor cap_color = padded_colors[2]; // key_id 3 is index 2
    unsigned char r_cap = RGBGetRValue(cap_color);
    unsigned char g_cap = RGBGetGValue(cap_color);
    unsigned char b_cap = RGBGetBValue(cap_color);

    GetKeyBlock(3, r_cap, g_cap, b_cap, &packet18[1]);

    int ret18 = hid_send_feature_report(dev, packet18, 65);
    if (ret18 < 0)
    {
        LOG_INFO("[RAKK Controller] Packet 18 send failed: %d", ret18);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    LOG_INFO("[RAKK Controller] Sending Apply Command...");
    // Step 5: Apply Command
    const unsigned char apply_cmd[] = { 0x04, 0x02, 0x00, 0x00 };
    SendCmd(apply_cmd, sizeof(apply_cmd));
    LOG_INFO("[RAKK Controller] Apply Command sent.");
}

void RakkLamAngV1Controller::SaveToFlash()
{
    const unsigned char save_cmd[] = { 0x04, 0xF0, 0x00, 0x00 };
    SendCmd(save_cmd, sizeof(save_cmd));
}
