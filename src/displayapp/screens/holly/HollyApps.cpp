#include "displayapp/screens/holly/HollyApps.h"
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Symbols.h"

using namespace Pinetime::Applications::Screens;

namespace {
  void ButtonEventHandler(lv_obj_t* obj, lv_event_t event) {
    auto* screen = static_cast<HollyApps*>(obj->user_data);
    screen->OnButtonEvent(obj, event);
  }
}

HollyApps::HollyApps(DisplayApp* app) : Screen(app) {

  // Gradient border
  lv_style_init(&gradient_border_style);
  lv_style_set_radius(&gradient_border_style, LV_STATE_DEFAULT, 24);
  lv_style_set_bg_color(&gradient_border_style, LV_STATE_DEFAULT, lv_color_hex(0xffc061));
  lv_style_set_bg_grad_color(&gradient_border_style, LV_STATE_DEFAULT, lv_color_hex(0x683b3c));
  lv_style_set_bg_grad_dir(&gradient_border_style, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
  lv_obj_t* gradient_border = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_add_style(gradient_border, LV_OBJ_PART_MAIN, &gradient_border_style);
  lv_obj_set_size(gradient_border, 240, 240);

  // Content background
  lv_style_init(&content_bg_style);
  lv_style_set_radius(&content_bg_style, LV_STATE_DEFAULT, 12);
  lv_style_set_bg_color(&content_bg_style, LV_STATE_DEFAULT, lv_color_hex(0xF6B662));
  lv_obj_t* content_bg = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_align(content_bg, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);
  lv_obj_add_style(content_bg, LV_OBJ_PART_MAIN, &content_bg_style);
  lv_obj_set_size(content_bg, 200, 200);

  // Button style
  lv_style_init(&btn_style);
  lv_style_set_bg_color(&btn_style, LV_STATE_DEFAULT, lv_color_hex(0x8B4C4A));

  // App buttons
  btn1 = lv_btn_create(lv_scr_act(), nullptr);
  btn1->user_data = this;
  lv_obj_set_event_cb(btn1, ButtonEventHandler);
  lv_obj_add_style(btn1, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(btn1, 85, 53);
  lv_obj_align(btn1, nullptr, LV_ALIGN_IN_TOP_LEFT, 30, 30);
  lv_obj_t* labeled_btn1 = lv_label_create(btn1, nullptr);
  lv_label_set_text_static(labeled_btn1, Symbols::hourGlass);
  lv_obj_set_style_local_text_color(labeled_btn1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xF6B662));

  btn2 = lv_btn_create(lv_scr_act(), nullptr);
  btn2->user_data = this;
  lv_obj_set_event_cb(btn2, ButtonEventHandler);
  lv_obj_add_style(btn2, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(btn2, 85, 53);
  lv_obj_align(btn2, nullptr, LV_ALIGN_IN_TOP_RIGHT, -30, 30);
  lv_obj_t* labeled_btn2 = lv_label_create(btn2, nullptr);
  lv_label_set_text_static(labeled_btn2, Symbols::clock);
  lv_obj_set_style_local_text_color(labeled_btn2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xF6B662));

  btn3 = lv_btn_create(lv_scr_act(), nullptr);
  btn3->user_data = this;
  lv_obj_set_event_cb(btn3, ButtonEventHandler);
  lv_obj_add_style(btn3, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(btn3, 85, 53);
  lv_obj_align(btn3, nullptr, LV_ALIGN_IN_LEFT_MID, 30, 0);
  lv_obj_t* labeled_btn3 = lv_label_create(btn3, nullptr);
  lv_label_set_text_static(labeled_btn3, Symbols::stopWatch);
  lv_obj_set_style_local_text_color(labeled_btn3, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xF6B662));

  btn4 = lv_btn_create(lv_scr_act(), nullptr);
  btn4->user_data = this;
  lv_obj_set_event_cb(btn4, ButtonEventHandler);
  lv_obj_add_style(btn4, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(btn4, 85, 53);
  lv_obj_align(btn4, nullptr, LV_ALIGN_IN_RIGHT_MID, -30, 0);
  lv_obj_t* labeled_btn4 = lv_label_create(btn4, nullptr);
  lv_label_set_text_static(labeled_btn4, Symbols::batteryHalf);
  lv_obj_set_style_local_text_color(labeled_btn4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xF6B662));

  btn5 = lv_btn_create(lv_scr_act(), nullptr);
  btn5->user_data = this;
  lv_obj_set_event_cb(btn5, ButtonEventHandler);
  lv_obj_add_style(btn5, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(btn5, 85, 53);
  lv_obj_align(btn5, nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 30, -30);
  lv_obj_t* labeled_btn5 = lv_label_create(btn5, nullptr);
  lv_label_set_text_static(labeled_btn5, Symbols::drum);
  lv_obj_set_style_local_text_color(labeled_btn5, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xF6B662));

  btn6 = lv_btn_create(lv_scr_act(), nullptr);
  btn6->user_data = this;
  lv_obj_set_event_cb(btn6, ButtonEventHandler);
  lv_obj_add_style(btn6, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(btn6, 85, 53);
  lv_obj_align(btn6, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, -30, -30);
  lv_obj_t* labeled_btn6 = lv_label_create(btn6, nullptr);
  lv_label_set_text_static(labeled_btn6, Symbols::flashlight);
  lv_obj_set_style_local_text_color(labeled_btn6, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xF6B662));
  lv_obj_set_style_local_text_font(labeled_btn6, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_48);
}

HollyApps::~HollyApps() {
  lv_style_reset(&gradient_border_style);
  lv_style_reset(&content_bg_style);
  lv_style_reset(&btn_style);
  lv_obj_clean(lv_scr_act());
}

void HollyApps::OnButtonEvent(lv_obj_t* object, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    if (object == btn1) {
      app->StartApp(Apps::Timer, DisplayApp::FullRefreshDirections::Up);
    } else if (object == btn2) {
      app->StartApp(Apps::Alarm, DisplayApp::FullRefreshDirections::Up);
    } else if (object == btn3) {
      app->StartApp(Apps::StopWatch, DisplayApp::FullRefreshDirections::Up);
    } else if (object == btn4) {
      app->StartApp(Apps::BatteryInfo, DisplayApp::FullRefreshDirections::Up);
    } else if (object == btn5) {
      app->StartApp(Apps::Metronome, DisplayApp::FullRefreshDirections::Up);
    } else if (object == btn6) {
      app->StartApp(Apps::FlashLight, DisplayApp::FullRefreshDirections::Up);
    }
  }
}
