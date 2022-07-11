#pragma once

#include "displayapp/screens/Screen.h"
#include <lvgl/lvgl.h>

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class HollyUnused : public Screen {
      public:
        HollyUnused(DisplayApp* app);
        ~HollyUnused() override;

      private:
        lv_style_t gradient_border_style;
        lv_style_t content_bg_style;
      };
    }
  }
}
