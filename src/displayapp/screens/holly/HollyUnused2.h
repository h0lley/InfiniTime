#pragma once

#include "displayapp/screens/Screen.h"
#include <lvgl/lvgl.h>

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class HollyUnused2 : public Screen {
      public:
        HollyUnused2(DisplayApp* app);
        ~HollyUnused2() override;

      private:
        lv_style_t gradient_border_style;
        lv_style_t content_bg_style;
      };
    }
  }
}
