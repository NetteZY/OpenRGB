#include <hidapi.h>
#include "DetectionManager.h"
#include "LogManager.h"
#include "RakkLamAngV1Controller.h"
#include "RGBController_RakkLamAngV1.h"

DetectedControllers DetectRakkLamAngV1Keyboards(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;

    LOG_INFO("[RAKK Detector] DetectRakkLamAngV1Keyboards called for path %s", info->path);

    // On Windows, the interface number is reported as -1, so we filter by usage page instead.
    // The vendor-defined control interface has usage page >= 0xFF00.
    if (info->usage_page < 0xFF00)
    {
        LOG_INFO("[RAKK Detector] Skipping path %s due to usage page 0x%04X (expected >= 0xFF00)", info->path, info->usage_page);
        return detected_controllers;
    }

    LOG_INFO("[RAKK Detector] Trying to open path: %s", info->path);
    hid_device* dev = hid_open_path(info->path);
    LOG_INFO("[RAKK Detector] hid_open_path returned %p", dev);

    if (dev)
    {
        LOG_INFO("[RAKK Detector] Instantiating RakkLamAngV1Controller...");
        RakkLamAngV1Controller* controller = new RakkLamAngV1Controller(dev, info->path);
        
        LOG_INFO("[RAKK Detector] Instantiating RGBController_RakkLamAngV1...");
        RGBController_RakkLamAngV1* rgb_controller = new RGBController_RakkLamAngV1(controller);
        
        LOG_INFO("[RAKK Detector] Registering RGBController...");
        detected_controllers.push_back(rgb_controller);
        
        LOG_INFO("[RAKK Detector] Done instantiating.");
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR("RAKK Lam-Ang Lite V1", DetectRakkLamAngV1Keyboards, 0x0C45, 0x8006);


