#pragma once

#include "RGBController.h"
#include "RakkLamAngV1Controller.h"

class RGBController_RakkLamAngV1 : public RGBController
{
public:
    RGBController_RakkLamAngV1(RakkLamAngV1Controller* controller_ptr);
    ~RGBController_RakkLamAngV1();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    RakkLamAngV1Controller* controller;
};
