
#include <lvgl/lvgl.h>
#include <components/heartrate/HeartRateController.h>

#include "displayapp/screens/holly/HollyHeart.h"
#include "displayapp/DisplayApp.h"

using namespace Pinetime::Applications::Screens;

namespace {
  const char* ToString(Pinetime::Controllers::HeartRateController::States s) {
    switch (s) {
      case Pinetime::Controllers::HeartRateController::States::NotEnoughData:
        return "Not enough data,\nplease wait...";
      case Pinetime::Controllers::HeartRateController::States::NoTouch:
        return "No touch detected";
      case Pinetime::Controllers::HeartRateController::States::Running:
        return "Measuring...";
      case Pinetime::Controllers::HeartRateController::States::Stopped:
        return "Stopped";
    }
    return "";
  }

  void btnStartStopEventHandler(lv_obj_t* obj, lv_event_t event) {
    auto* screen = static_cast<HollyHeart*>(obj->user_data);
    screen->OnStartStopEvent(event);
  }
}

HollyHeart::HollyHeart(Pinetime::Applications::DisplayApp* app,
                     Controllers::HeartRateController& heartRateController,
                     System::SystemTask& systemTask)
  : Screen(app), heartRateController {heartRateController}, systemTask {systemTask} {

  // Gradient border
  lv_style_init(&gradient_border_style);
  lv_style_set_radius(&gradient_border_style, LV_STATE_DEFAULT, 24);
  lv_style_set_bg_color(&gradient_border_style, LV_STATE_DEFAULT, lv_color_hex(0xffb2d7));
  lv_style_set_bg_grad_color(&gradient_border_style, LV_STATE_DEFAULT, lv_color_hex(0x911d5e));
  lv_style_set_bg_grad_dir(&gradient_border_style, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
  lv_obj_t* gradient_border = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_add_style(gradient_border, LV_OBJ_PART_MAIN, &gradient_border_style);
  lv_obj_set_size(gradient_border, 240, 240);

  // Content background
  lv_style_init(&content_bg_style);
  lv_style_set_radius(&content_bg_style, LV_STATE_DEFAULT, 12);
  lv_style_set_bg_color(&content_bg_style, LV_STATE_DEFAULT, lv_color_hex(0xF6A5CD));
  lv_obj_t* content_bg = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_align(content_bg, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);
  lv_obj_add_style(content_bg, LV_OBJ_PART_MAIN, &content_bg_style);
  lv_obj_set_size(content_bg, 200, 200);

  // Heart rate
  bool isHrRunning = heartRateController.State() != Controllers::HeartRateController::States::Stopped;
  label_hr = lv_label_create(lv_scr_act(), nullptr);

  lv_obj_set_style_local_text_font(label_hr, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_76);

  if (isHrRunning)
    lv_obj_set_style_local_text_color(label_hr, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xB52473));
  else
    lv_obj_set_style_local_text_color(label_hr, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xb37697));

  lv_label_set_text_static(label_hr, "000");
  lv_obj_align(label_hr, nullptr, LV_ALIGN_CENTER, 0, -50);

  label_bpm = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(label_bpm, "BPM");
  lv_obj_align(label_bpm, label_hr, LV_ALIGN_IN_BOTTOM_RIGHT, 30, -5);
  lv_obj_set_style_local_text_color(label_bpm, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xB52473));

  // Status
  label_status = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(label_status, ToString(Pinetime::Controllers::HeartRateController::States::NotEnoughData));
  lv_obj_align(label_status, label_hr, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
  lv_obj_set_style_local_text_color(label_status, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xb37697));

  // Start button
  btn_startStop = lv_btn_create(lv_scr_act(), nullptr);
  btn_startStop->user_data = this;
  lv_obj_set_size(btn_startStop, 80, 40);
  lv_obj_set_event_cb(btn_startStop, btnStartStopEventHandler);
  lv_obj_align(btn_startStop, nullptr, LV_ALIGN_IN_BOTTOM_MID, 0, -30);
  lv_obj_set_style_local_bg_color(btn_startStop, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xB52473));

  label_startStop = lv_label_create(btn_startStop, nullptr);
  lv_obj_set_style_local_text_color(label_startStop, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xF6A5CD));
  UpdateStartStopButton(isHrRunning);
  if (isHrRunning)
    systemTask.PushMessage(Pinetime::System::Messages::DisableSleeping);


  taskRefresh = lv_task_create(RefreshTaskCallback, 100, LV_TASK_PRIO_MID, this);
}

HollyHeart::~HollyHeart() {
  lv_style_reset(&gradient_border_style);
  lv_style_reset(&content_bg_style);
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
  systemTask.PushMessage(Pinetime::System::Messages::EnableSleeping);
}

void HollyHeart::Refresh() {

  auto state = heartRateController.State();
  switch (state) {
    case Controllers::HeartRateController::States::NoTouch:
    case Controllers::HeartRateController::States::NotEnoughData:
      // case Controllers::HeartRateController::States::Stopped:
      lv_label_set_text_static(label_hr, "000");
      break;
    default:
      lv_label_set_text_fmt(label_hr, "%03d", heartRateController.HeartRate());
  }

  lv_label_set_text_static(label_status, ToString(state));
  lv_obj_align(label_status, label_hr, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}

void HollyHeart::OnStartStopEvent(lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    if (heartRateController.State() == Controllers::HeartRateController::States::Stopped) {
      heartRateController.Start();
      UpdateStartStopButton(heartRateController.State() != Controllers::HeartRateController::States::Stopped);
      systemTask.PushMessage(Pinetime::System::Messages::DisableSleeping);
      lv_obj_set_style_local_text_color(label_hr, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xB52473));
    } else {
      heartRateController.Stop();
      UpdateStartStopButton(heartRateController.State() != Controllers::HeartRateController::States::Stopped);
      systemTask.PushMessage(Pinetime::System::Messages::EnableSleeping);
      lv_obj_set_style_local_text_color(label_hr, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xb37697));
    }
  }
}

void HollyHeart::UpdateStartStopButton(bool isRunning) {
  if (isRunning)
    lv_label_set_text_static(label_startStop, "Stop");
  else
    lv_label_set_text_static(label_startStop, "Start");
}
