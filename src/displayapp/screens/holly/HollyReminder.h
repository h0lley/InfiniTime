#pragma once

#include "displayapp/screens/Screen.h"
#include <lvgl/lvgl.h>

#include "components/holly/ReminderController.h"
#include "components/datetime/DateTimeController.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class HollyReminder : public Screen {
      public:
        HollyReminder(DisplayApp* app,
              Controllers::ReminderController& reminderController,
              Pinetime::Controllers::Settings& settingsController,
              Controllers::DateTime& dateTimeController,
              System::SystemTask& systemTask);

        ~HollyReminder() override;

        void Refresh() override;
        void OnButtonEvent(lv_obj_t* object, lv_event_t event);

      private:
        Controllers::ReminderController& reminderController;
        Controllers::Settings& settingsController;
        Controllers::DateTime& dateTimeController;
        System::SystemTask& systemTask;

        lv_style_t gradient_border_style;
        lv_style_t content_bg_style;
        lv_style_t btn_style;

        lv_obj_t* time_remaining_label;
        lv_obj_t* reminder_dismissal_bg;
        lv_obj_t* reminder_dismissal_label;
        lv_obj_t* snooze_btn;
        lv_obj_t* dismiss_btn;
        lv_obj_t* enabled_switch;

        lv_task_t* taskRefresh;

        const uint16_t full_duration_min = 40;
        const uint16_t snooze_duration_min = 20;
        const uint16_t ignore_duration_min = 5;

        uint16_t awaiting_response_tick_cd = 0;
      };
    }
  }
}
