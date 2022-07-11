#pragma once

#include "displayapp/screens/Screen.h"
#include <lvgl/lvgl.h>

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class HollyApps : public Screen {
      public:
        HollyApps(DisplayApp* app);
        ~HollyApps() override;

        void OnButtonEvent(lv_obj_t* object, lv_event_t event);

      private:
        lv_style_t gradient_border_style;
        lv_style_t content_bg_style;
        lv_style_t btn_style;

        lv_obj_t* btn1;
        lv_obj_t* btn2;
        lv_obj_t* btn3;
        lv_obj_t* btn4;
        lv_obj_t* btn5;
        lv_obj_t* btn6;
      };
    }
  }
}
