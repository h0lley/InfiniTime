#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "displayapp/screens/Screen.h"
#include <components/motion/MotionController.h>

namespace Pinetime {

  namespace Controllers {
    class Settings;
  }

  namespace Applications {
    namespace Screens {

      class HollySteps : public Screen {
      public:
        HollySteps(DisplayApp* app, Controllers::MotionController& motionController, Controllers::Settings& settingsController);
        ~HollySteps() override;

        void Refresh() override;
        void lapBtnEventHandler(lv_event_t event);

      private:
        Controllers::MotionController& motionController;
        Controllers::Settings& settingsController;

        uint32_t currentTripSteps = 0;

        lv_obj_t* lSteps;
        lv_obj_t* lStepsIcon;
        lv_obj_t* stepsArc;
        lv_obj_t* resetBtn;
        lv_obj_t* resetButtonLabel;
        lv_obj_t* tripLabel;

        uint32_t stepsCount;

        lv_task_t* taskRefresh;

        lv_style_t gradient_border_style;
        lv_style_t content_bg_style;
        lv_style_t btn_style;
      };
    }
  }
}
