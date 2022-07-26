#include "displayapp/screens/holly/HollyWorkout.h"
#include "displayapp/DisplayApp.h"

#include "displayapp/screens/holly/images/fionna.c"

using namespace Pinetime::Applications::Screens;

namespace {
  void ButtonEventHandler(lv_obj_t* obj, lv_event_t event) {
    auto* screen = static_cast<HollyWorkout*>(obj->user_data);
    screen->OnButtonEvent(obj, event);
  }
}

constexpr std::array<const char*, 16> HollyWorkout::exercises;

HollyWorkout::HollyWorkout(DisplayApp* app, Controllers::DateTime& dateTimeController)
  : Screen(app),
    dateTimeController {dateTimeController} {

  // Gradient border
  lv_style_init(&gradient_border_style);
  lv_style_set_radius(&gradient_border_style, LV_STATE_DEFAULT, 24);
  lv_style_set_bg_color(&gradient_border_style, LV_STATE_DEFAULT, lv_color_hex(0xffebb9));
  lv_style_set_bg_grad_color(&gradient_border_style, LV_STATE_DEFAULT, lv_color_hex(0xa7375f));
  lv_style_set_bg_grad_dir(&gradient_border_style, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
  lv_obj_t* gradient_border = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_add_style(gradient_border, LV_OBJ_PART_MAIN, &gradient_border_style);
  lv_obj_set_size(gradient_border, 240, 240);

  // Content background
  lv_style_init(&content_bg_style);
  lv_style_set_radius(&content_bg_style, LV_STATE_DEFAULT, 12);
  lv_style_set_bg_color(&content_bg_style, LV_STATE_DEFAULT, lv_color_hex(0xF6DAB5));
  lv_obj_t* content_bg = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_align(content_bg, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);
  lv_obj_add_style(content_bg, LV_OBJ_PART_MAIN, &content_bg_style);
  lv_obj_set_size(content_bg, 200, 200);

  // Exercise
  exercise_label = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(exercise_label, exercises[app->hollyState.current_exercise]);
  lv_label_set_align(exercise_label, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(exercise_label, lv_scr_act(), LV_ALIGN_IN_TOP_MID, -20, 40);
  lv_obj_set_style_local_text_color(exercise_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xA43C62));

  // Set indication
  set_label = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_fmt(set_label, "%d / %d", app->hollyState.current_set + 1, set_amount);
  lv_label_set_align(set_label, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(set_label, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 50, -10);
  lv_obj_set_style_local_text_color(set_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xA43C62));
  //lv_obj_set_style_local_text_font(set_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_42);

  // Button style
  lv_style_init(&btn_style);
  lv_style_set_bg_color(&btn_style, LV_STATE_DEFAULT, lv_color_hex(0xA43C62));

  // Done button
  done_btn = lv_btn_create(lv_scr_act(), nullptr);
  done_btn->user_data = this;
  lv_obj_set_event_cb(done_btn, ButtonEventHandler);
  lv_obj_add_style(done_btn, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(done_btn, 80, 40);
  lv_obj_align(done_btn, nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 30, -30);
  labeled_done_btn = lv_label_create(done_btn, nullptr);
  lv_label_set_text_static(labeled_done_btn, "Next");
  lv_obj_set_style_local_text_color(labeled_done_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xF6DAB5));

  // Reminder dismissal count
  if (app->hollyState.today_exercise != dateTimeController.Day()) {
    app->hollyState.today_exercise = dateTimeController.Day();
    app->hollyState.todays_exercise_done_count = 0;
  }

  exercise_done_bg = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_align(exercise_done_bg, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -5, 20);
  lv_obj_set_size(exercise_done_bg, 40, 30);
  lv_obj_set_style_local_bg_color(exercise_done_bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xA43C62));
  lv_obj_set_style_local_radius(exercise_done_bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 15);
  exercise_done_label = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_fmt(exercise_done_label, "%d", app->hollyState.todays_exercise_done_count);
  lv_label_set_align(exercise_done_label, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(exercise_done_label, exercise_done_bg, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_local_text_color(exercise_done_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xF6DAB5));

  // Decorative sprite
  lv_obj_t* fionna_img = lv_img_create(lv_scr_act(), nullptr);
  lv_img_set_src(fionna_img, &fionna);
  lv_obj_align(fionna_img, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, 0, -8);
}

HollyWorkout::~HollyWorkout() {
  lv_style_reset(&gradient_border_style);
  lv_style_reset(&content_bg_style);
  lv_style_reset(&btn_style);
  lv_obj_clean(lv_scr_act());
}

void HollyWorkout::OnButtonEvent(lv_obj_t* object, lv_event_t event) {
  if (event == LV_EVENT_CLICKED && object == done_btn) {

    app->hollyState.current_set = (app->hollyState.current_set + 1) % set_amount;

    if (!app->hollyState.current_set) {
      app->hollyState.current_exercise = (app->hollyState.current_exercise + 1) % exercise_amount;
      lv_label_set_text_static(exercise_label, exercises[app->hollyState.current_exercise]);
      lv_obj_align(exercise_label, lv_scr_act(), LV_ALIGN_IN_TOP_MID, -20, 40);
      //lv_label_set_text_static(labeled_done_btn, "Done");
    }
    //else if (app->hollyState.current_set == set_amount - 1) {
    //  lv_label_set_text_static(labeled_done_btn, "Next");
    //}

    lv_obj_set_hidden(done_btn, true);

    lv_label_set_text_fmt(set_label, "%d / %d", app->hollyState.current_set + 1, set_amount);

    if (app->hollyState.today_exercise != dateTimeController.Day()) {
      app->hollyState.today_exercise = dateTimeController.Day();
      app->hollyState.todays_exercise_done_count = 0;
    }

    app->hollyState.todays_exercise_done_count += 1;
    lv_label_set_text_fmt(exercise_done_label, "%d", app->hollyState.todays_exercise_done_count);
    lv_obj_align(exercise_done_label, exercise_done_bg, LV_ALIGN_CENTER, 0, 0);
  }
}
