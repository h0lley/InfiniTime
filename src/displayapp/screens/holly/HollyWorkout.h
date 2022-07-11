#pragma once

#include <array>
#include <lvgl/lvgl.h>
#include "displayapp/screens/Screen.h"

#include "components/datetime/DateTimeController.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class HollyWorkout : public Screen {
      public:
        HollyWorkout(DisplayApp* app, Controllers::DateTime& dateTimeController);
        ~HollyWorkout() override;

        void OnButtonEvent(lv_obj_t* object, lv_event_t event);

      private:
        Controllers::DateTime& dateTimeController;

        lv_style_t gradient_border_style;
        lv_style_t content_bg_style;
        lv_style_t btn_style;

        static const uint8_t exercise_amount = 15;
        static const uint8_t set_amount = 2;

        static constexpr std::array<const char*, exercise_amount> exercises = {
          "Push ups",
          "Dumbbell\nshrugs",
          "Lateral\nraises",
          "Overhead\npress",
          "Band chest\nflies",
          "Dips",
          "Squads",
          "Glute\nbridges",
          "Pull ups",
          "Chin ups",
          "Levitation\ncrunches",
          "Reverse\ncrunches",
          "Hip\nhuggers",
          "Dumbbell\ncurls",
          "Dumbbell\ntripod rows"
        };

        lv_obj_t* exercise_label;
        lv_obj_t* set_label;
        lv_obj_t* labeled_done_btn;
        lv_obj_t* exercise_done_bg;
        lv_obj_t* exercise_done_label;
      };
    }
  }
}
