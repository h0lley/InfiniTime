#include "displayapp/screens/holly/HollyReminder.h"
#include "displayapp/DisplayApp.h"

#include "displayapp/screens/holly/images/glurak1.c"
#include "displayapp/screens/holly/images/glurak2.c"

using namespace Pinetime::Applications::Screens;

namespace {
  void ButtonEventHandler(lv_obj_t* obj, lv_event_t event) {
    auto* screen = static_cast<HollyReminder*>(obj->user_data);
    screen->OnButtonEvent(obj, event);
  }
}

HollyReminder::HollyReminder(DisplayApp* app,
             Controllers::ReminderController& reminderController,
             Pinetime::Controllers::Settings& settingsController,
             Controllers::DateTime& dateTimeController,
             System::SystemTask& systemTask)
  : Screen(app),
    reminderController {reminderController},
    settingsController {settingsController},
    dateTimeController {dateTimeController},
    systemTask {systemTask} {

  bool evening = dateTimeController.Hours() >= evening_start_hour;

  lv_color_t bg_gradient_top;
  lv_color_t bg_gradient_bottom;
  lv_color_t bg_content;
  lv_color_t text_color;
  lv_color_t label_bg;

  if (!evening) {
    bg_gradient_top = lv_color_hex(0xb6bbd8);
    bg_gradient_bottom = lv_color_hex(0x2c3154);
    bg_content = lv_color_hex(0xACAED5);
    text_color = lv_color_hex(0x31385A);
    label_bg = lv_color_hex(0x31385A);
  } else {
    bg_gradient_top = lv_color_hex(0x66659e);
    bg_gradient_bottom = lv_color_hex(0x130b21);
    bg_content = lv_color_hex(0x5A5D94);
    text_color = lv_color_hex(0xACAED5);
    label_bg = lv_color_hex(0x211831);
  }

  // Gradient border
  lv_style_init(&gradient_border_style);
  lv_style_set_radius(&gradient_border_style, LV_STATE_DEFAULT, 24);
  lv_style_set_bg_color(&gradient_border_style, LV_STATE_DEFAULT, bg_gradient_top);
  lv_style_set_bg_grad_color(&gradient_border_style, LV_STATE_DEFAULT, bg_gradient_bottom);
  lv_style_set_bg_grad_dir(&gradient_border_style, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
  lv_obj_t* gradient_border = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_add_style(gradient_border, LV_OBJ_PART_MAIN, &gradient_border_style);
  lv_obj_set_size(gradient_border, 240, 240);

  // Content background
  lv_style_init(&content_bg_style);
  lv_style_set_radius(&content_bg_style, LV_STATE_DEFAULT, 12);
  lv_style_set_bg_color(&content_bg_style, LV_STATE_DEFAULT, bg_content);
  lv_obj_t* content_bg = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_align(content_bg, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);
  lv_obj_add_style(content_bg, LV_OBJ_PART_MAIN, &content_bg_style);
  lv_obj_set_size(content_bg, 200, 200);

  // Remaining minutes
  time_remaining_label = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(time_remaining_label, "...");
  lv_label_set_align(time_remaining_label, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(time_remaining_label, lv_scr_act(), LV_ALIGN_CENTER, 0, 15);
  lv_obj_set_style_local_text_color(time_remaining_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, text_color);

  // Button style
  lv_style_init(&btn_style);
  lv_style_set_bg_color(&btn_style, LV_STATE_DEFAULT, label_bg);

  // Snooze button
  snooze_btn = lv_btn_create(lv_scr_act(), nullptr);
  snooze_btn->user_data = this;
  lv_obj_set_event_cb(snooze_btn, ButtonEventHandler);
  lv_obj_add_style(snooze_btn, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(snooze_btn, 85, 40);
  lv_obj_align(snooze_btn, nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 30, -30);
  lv_obj_t* labeled_snooze_btn = lv_label_create(snooze_btn, nullptr);
  lv_label_set_text_static(labeled_snooze_btn, "Snooze");
  lv_obj_set_style_local_text_color(labeled_snooze_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xACAED5));
  lv_obj_set_hidden(snooze_btn, true);

  // Dismiss button
  dismiss_btn = lv_btn_create(lv_scr_act(), nullptr);
  dismiss_btn->user_data = this;
  lv_obj_set_event_cb(dismiss_btn, ButtonEventHandler);
  lv_obj_add_style(dismiss_btn, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(dismiss_btn, 85, 40);
  lv_obj_align(dismiss_btn, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, -30, -30);
  lv_obj_t* labeled_dismiss_btn = lv_label_create(dismiss_btn, nullptr);
  lv_label_set_text_static(labeled_dismiss_btn, "Check");
  lv_obj_set_style_local_text_color(labeled_dismiss_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xACAED5));
  lv_obj_set_hidden(dismiss_btn, true);

  // On/off toggle
  enabled_switch = lv_switch_create(lv_scr_act(), nullptr);
  enabled_switch->user_data = this;
  lv_obj_set_event_cb(enabled_switch, ButtonEventHandler);
  lv_obj_set_size(enabled_switch, 60, 30);
  lv_obj_align(enabled_switch, lv_scr_act(), LV_ALIGN_CENTER, -12, -28);
  lv_obj_set_style_local_bg_color(enabled_switch, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, label_bg);
  lv_obj_set_style_local_bg_color(enabled_switch, LV_SWITCH_PART_INDIC, LV_STATE_DEFAULT, label_bg);
  lv_obj_set_style_local_bg_color(enabled_switch, LV_SWITCH_PART_KNOB, LV_STATE_DEFAULT, lv_color_hex(0x398583));
  lv_obj_set_style_local_bg_color(enabled_switch, LV_SWITCH_PART_KNOB, LV_STATE_CHECKED, lv_color_hex(0xFF5921));

  // Reminder dismissal count
  if (app->hollyState.today_reminder != dateTimeController.Day()) {
    app->hollyState.today_reminder = dateTimeController.Day();
    app->hollyState.todays_reminder_dismissal_count = 0;
  }

  reminder_dismissal_bg = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_align(reminder_dismissal_bg, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -5, 20);
  lv_obj_set_size(reminder_dismissal_bg, 40, 30);
  lv_obj_set_style_local_bg_color(reminder_dismissal_bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, label_bg);
  lv_obj_set_style_local_radius(reminder_dismissal_bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 15);
  reminder_dismissal_label = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_fmt(reminder_dismissal_label, "%d", app->hollyState.todays_reminder_dismissal_count);
  lv_label_set_align(reminder_dismissal_label, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(reminder_dismissal_label, reminder_dismissal_bg, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_local_text_color(reminder_dismissal_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xACAED5));

  // Decorative sprite
  if (!evening) {
    lv_img_set_src(lv_img_create(lv_scr_act(), nullptr),  &glurak1);
    lv_obj_t* glurak2_img = lv_img_create(lv_scr_act(), nullptr);
    lv_img_set_src(glurak2_img, &glurak2);
    lv_obj_align(glurak2_img, nullptr, LV_ALIGN_IN_TOP_LEFT, 76, 2);

    if (app->hollyState.reminder_enabled) {
      lv_switch_on(enabled_switch, LV_ANIM_OFF);

      if (reminderController.SecondsToReminder() <= 0) {
        lv_obj_set_hidden(snooze_btn, false);
        lv_obj_set_hidden(dismiss_btn, false);
      }
    }
  }

  taskRefresh = lv_task_create(RefreshTaskCallback, 1000, LV_TASK_PRIO_MID, this);
}

HollyReminder::~HollyReminder() {
  lv_style_reset(&gradient_border_style);
  lv_style_reset(&content_bg_style);
  lv_style_reset(&btn_style);
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
  reminderController.StopAlerting();
}

void HollyReminder::Refresh() {
  if (!app->hollyState.reminder_enabled)
    return;

  if (awaiting_response_tick_cd > 0) {
    awaiting_response_tick_cd -= 1;

    if (awaiting_response_tick_cd <= 0) {
      reminderController.SetReminder(ignore_duration_min);
      reminderController.StopAlerting();
    }
  } else {

    int16_t sec_remaining = reminderController.SecondsToReminder();

    if (sec_remaining <= 0) {
      lv_obj_set_hidden(snooze_btn, false);
      lv_obj_set_hidden(dismiss_btn, false);
      awaiting_response_tick_cd = 5;
    }

    if (sec_remaining >= 0) {
      lv_label_set_text_fmt(time_remaining_label, "%d min", sec_remaining / 60);
      lv_obj_align(time_remaining_label, lv_scr_act(), LV_ALIGN_CENTER, 0, 15);
    }
  }
}

void HollyReminder::OnButtonEvent(lv_obj_t* object, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {

    if (object == enabled_switch) {

      if (!reminderController.IsReminderActive()) { // lv_switch_get_state(enabled_switch)

        lv_switch_on(enabled_switch, LV_ANIM_OFF);
        lv_obj_set_hidden(time_remaining_label, false);
        app->hollyState.reminder_enabled = true;

        reminderController.SetReminder(full_duration_min);

      } else {

        lv_switch_off(enabled_switch, LV_ANIM_OFF);
        lv_obj_set_hidden(snooze_btn, true);
        lv_obj_set_hidden(dismiss_btn, true);
        lv_label_set_text_static(time_remaining_label, "...");
        app->hollyState.reminder_enabled = false;

        reminderController.DisableReminder();

      }
    } else {
      lv_obj_set_hidden(snooze_btn, true);
      lv_obj_set_hidden(dismiss_btn, true);

      if (object == snooze_btn) {
        reminderController.SetReminder(snooze_duration_min);
      } else if (object == dismiss_btn) {
        reminderController.SetReminder(full_duration_min);

        if (app->hollyState.today_reminder != dateTimeController.Day()) {
          app->hollyState.today_reminder = dateTimeController.Day();
          app->hollyState.todays_reminder_dismissal_count = 0;
        }

        app->hollyState.todays_reminder_dismissal_count += 1;
        lv_label_set_text_fmt(reminder_dismissal_label, "%d", app->hollyState.todays_reminder_dismissal_count);
        lv_obj_align(reminder_dismissal_label, reminder_dismissal_bg, LV_ALIGN_CENTER, 0, 0);
      }
    }

    reminderController.StopAlerting();
    awaiting_response_tick_cd = 0;
  }
}
