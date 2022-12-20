#pragma once

#include <cstdint>
#include "displayapp/screens/Screen.h"
#include <lvgl/lvgl.h>
#include "components/datetime/DateTimeController.h"
#include "components/brightness/BrightnessController.h"
#include "components/motor/MotorController.h"
#include "components/settings/Settings.h"
#include "components/battery/BatteryController.h"
#include <displayapp/screens/BatteryIcon.h>

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class HollySettings : public Screen {
      public:
        HollySettings(DisplayApp* app,
                      Controllers::BrightnessController& brightness,
                      Pinetime::Controllers::Settings& settingsController);

        ~HollySettings() override;

        void OnButtonEvent(lv_obj_t* object, lv_event_t event);

      private:
        Controllers::BrightnessController& brightness;
        Controllers::Settings& settingsController;

        lv_style_t gradient_border_style;
        lv_style_t content_bg_style;
        lv_style_t btn_style;

        lv_obj_t* bluetooth_btn;
        lv_obj_t* brightness_btn;
        lv_obj_t* labeled_brightness_btn;
        lv_obj_t* more_configs_btn;

        // Temp
        lv_obj_t* btn2_btn;
      };
    }
  }
}
