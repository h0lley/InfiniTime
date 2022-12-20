#include "displayapp/screens/holly/HollySettings.h"
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Symbols.h"

using namespace Pinetime::Applications::Screens;

namespace {
  void ButtonEventHandler(lv_obj_t* obj, lv_event_t event) {
    auto* screen = static_cast<HollySettings*>(obj->user_data);
    screen->OnButtonEvent(obj, event);
  }
}

HollySettings::HollySettings(Pinetime::Applications::DisplayApp* app,
                             Controllers::BrightnessController& brightness,
                             Pinetime::Controllers::Settings& settingsController)
  : Screen(app),
    brightness {brightness},
    settingsController {settingsController} {

  // Gradient border
  lv_style_init(&gradient_border_style);
  lv_style_set_radius(&gradient_border_style, LV_STATE_DEFAULT, 24);
  lv_style_set_bg_color(&gradient_border_style, LV_STATE_DEFAULT, lv_color_hex(0xb5ffd0));
  lv_style_set_bg_grad_color(&gradient_border_style, LV_STATE_DEFAULT, lv_color_hex(0x009e7e));
  lv_style_set_bg_grad_dir(&gradient_border_style, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
  lv_obj_t* gradient_border = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_add_style(gradient_border, LV_OBJ_PART_MAIN, &gradient_border_style);
  lv_obj_set_size(gradient_border, 240, 240);

  // Content background
  lv_style_init(&content_bg_style);
  lv_style_set_radius(&content_bg_style, LV_STATE_DEFAULT, 12);
  lv_style_set_bg_color(&content_bg_style, LV_STATE_DEFAULT, lv_color_hex(0xA4F6CD));
  lv_obj_t* content_bg = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_align(content_bg, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);
  lv_obj_add_style(content_bg, LV_OBJ_PART_MAIN, &content_bg_style);
  lv_obj_set_size(content_bg, 200, 200);

  // Button style
  lv_style_init(&btn_style);
  lv_style_set_bg_opa(&btn_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);

  // Toggle bluetooth
  bluetooth_btn = lv_btn_create(lv_scr_act(), nullptr);
  bluetooth_btn->user_data = this;
  lv_obj_set_event_cb(bluetooth_btn, ButtonEventHandler);
  lv_obj_add_style(bluetooth_btn, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(bluetooth_btn, 80, 80);
  lv_obj_align(bluetooth_btn, nullptr, LV_ALIGN_IN_TOP_LEFT, 35, 35);
  lv_obj_t* labeled_bluetooth_btn = lv_label_create(bluetooth_btn, nullptr);
  lv_label_set_text_static(labeled_bluetooth_btn, "Blue-\ntooth");
  //lv_obj_set_style_local_text_font(labeled_bluetooth_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_80);
  lv_obj_set_style_local_text_color(labeled_bluetooth_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x009D7B));

  // Validate OS
  btn2_btn = lv_btn_create(lv_scr_act(), nullptr);
  btn2_btn->user_data = this;
  lv_obj_set_event_cb(btn2_btn, ButtonEventHandler);
  lv_obj_add_style(btn2_btn, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(btn2_btn, 80, 80);
  lv_obj_align(btn2_btn, nullptr, LV_ALIGN_IN_TOP_RIGHT, -35, 35);
  lv_obj_t* labeled_btn2_btn = lv_label_create(btn2_btn, nullptr);
  lv_label_set_text_static(labeled_btn2_btn, "Vali-\ndate");
  //lv_obj_set_style_local_text_font(labeled_btn2_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_80);
  lv_obj_set_style_local_text_color(labeled_btn2_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x009D7B));

  // Brightness button
  brightness_btn = lv_btn_create(lv_scr_act(), nullptr);
  brightness_btn->user_data = this;
  lv_obj_set_event_cb(brightness_btn, ButtonEventHandler);
  lv_obj_add_style(brightness_btn, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(brightness_btn, 80, 80);
  lv_obj_align(brightness_btn, nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 35, -35);
  labeled_brightness_btn = lv_label_create(brightness_btn, nullptr);
  lv_label_set_text_static(labeled_brightness_btn, brightness.GetIcon());
  lv_obj_set_style_local_text_font(labeled_brightness_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_80);
  lv_obj_set_style_local_text_color(labeled_brightness_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x009D7B));

  // More configs button
  more_configs_btn = lv_btn_create(lv_scr_act(), nullptr);
  more_configs_btn->user_data = this;
  lv_obj_set_event_cb(more_configs_btn, ButtonEventHandler);
  lv_obj_add_style(more_configs_btn, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(more_configs_btn, 80, 80);
  lv_obj_align(more_configs_btn, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, -35, -35);
  lv_obj_t* labeled_more_configs_btn = lv_label_create(more_configs_btn, nullptr);
  lv_label_set_text_static(labeled_more_configs_btn, Symbols::settings);
  lv_obj_set_style_local_text_font(labeled_more_configs_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_80);
  lv_obj_set_style_local_text_color(labeled_more_configs_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x009D7B));
}

HollySettings::~HollySettings() {
  lv_style_reset(&gradient_border_style);
  lv_style_reset(&content_bg_style);
  lv_style_reset(&btn_style);
  lv_obj_clean(lv_scr_act());
  settingsController.SaveSettings();
}

void HollySettings::OnButtonEvent(lv_obj_t* object, lv_event_t event) {
  if (event != LV_EVENT_CLICKED) {
    return;
  }

  if (object == brightness_btn) {
    brightness.Step();
    lv_label_set_text_static(labeled_brightness_btn, brightness.GetIcon());
    settingsController.SetBrightness(brightness.Level());
  } else if (object == more_configs_btn) {
    running = false;
    settingsController.SetSettingsMenu(0);
    app->StartApp(Apps::Settings, DisplayApp::FullRefreshDirections::Up);
  } else if (object == bluetooth_btn) {

    running = false;
    app->StartApp(Apps::SettingBluetooth, DisplayApp::FullRefreshDirections::Up);

  } else if (object == btn2_btn) {

    running = false;
    app->StartApp(Apps::FirmwareValidation, DisplayApp::FullRefreshDirections::Up);

  }
}
