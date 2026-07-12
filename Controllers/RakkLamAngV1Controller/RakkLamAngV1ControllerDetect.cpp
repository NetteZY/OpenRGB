#include <hidapi.h>
#include "DetectionManager.h"
#include "RakkLamAngV1Controller.h"
#include "RGBController_RakkLamAngV1.h"

DetectedControllers DetectRakkLamAngV1Keyboards(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;

    // On Windows, the interface number is reported as -1, so we filter by usage page instead.
    // The vendor-defined control interface has usage page >= 0xFF00.
    if (info->usage_page < 0xFF00)
    {
        return detected_controllers;
    }

    hid_device* dev = hid_open_path(info->path);

    if (dev)
    {
        RakkLamAngV1Controller* controller = new RakkLamAngV1Controller(dev, info->path);
        RGBController_RakkLamAngV1* rgb_controller = new RGBController_RakkLamAngV1(controller);
        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR("RAKK Lam-Ang Lite V1", DetectRakkLamAngV1Keyboards, 0x0C45, 0x8006);

