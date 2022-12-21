
#include "displayapp/screens/holly/HollyGym.h"
#include "displayapp/DisplayApp.h"

using namespace Pinetime::Applications::Screens;

namespace {
  void ButtonEventHandler(lv_obj_t* obj, lv_event_t event) {
    auto* screen = static_cast<HollyGym*>(obj->user_data);
    screen->OnButtonEvent(obj, event, nullptr);
  }

  void SelectionEventHandler(lv_obj_t* obj, lv_event_t event) {
    auto* selection = static_cast<HollyGym::Selection*>(obj->user_data);
    auto* screen = static_cast<HollyGym*>(selection->self);
    screen->OnButtonEvent(obj, event, selection);
  }
}

constexpr std::array<const char*, HollyGym::machine_amount> HollyGym::machine_names;

HollyGym::HollyGym(DisplayApp* app, Pinetime::Controllers::FS& fs)
  : Screen(app), fs {fs} {

  // Gradient border
  lv_style_init(&gradient_border_style);
  lv_style_set_radius(&gradient_border_style, LV_STATE_DEFAULT, 24);
  lv_style_set_bg_color(&gradient_border_style, LV_STATE_DEFAULT, lv_color_hex(0xe6544d));
  lv_style_set_bg_grad_color(&gradient_border_style, LV_STATE_DEFAULT, lv_color_hex(0x640748));
  lv_style_set_bg_grad_dir(&gradient_border_style, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
  lv_obj_t* gradient_border = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_add_style(gradient_border, LV_OBJ_PART_MAIN, &gradient_border_style);
  lv_obj_set_size(gradient_border, 240, 240);

  // Content background
  lv_style_init(&content_bg_style);
  lv_style_set_radius(&content_bg_style, LV_STATE_DEFAULT, 12);
  lv_style_set_bg_color(&content_bg_style, LV_STATE_DEFAULT, lv_color_hex(0xDE4C4A));
  lv_obj_t* content_bg = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_align(content_bg, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);
  lv_obj_add_style(content_bg, LV_OBJ_PART_MAIN, &content_bg_style);
  lv_obj_set_size(content_bg, 200, 200);

  // Button style
  lv_style_init(&btn_style);
  lv_style_set_bg_color(&btn_style, LV_STATE_DEFAULT, lv_color_hex(0x640748));

  lv_style_init(&btn_style_bordercover);
  lv_style_set_bg_color(&btn_style_bordercover, LV_STATE_DEFAULT, lv_color_hex(0xDE4C4A));

  // Content
  for (uint8_t i = 0; i < machines_per_page; i++) {
    uint8_t entry_idx = (current_page * machines_per_page) + i;
    uint8_t y_offset = 32 + i * 32;

    // Machine name
    lv_obj_t* machine_btn = lv_btn_create(lv_scr_act(), nullptr);
    lv_obj_set_event_cb(machine_btn, SelectionEventHandler);
    lv_obj_add_style(machine_btn, LV_BTN_PART_MAIN, &btn_style_bordercover);
    lv_obj_set_size(machine_btn, 92, 34);
    lv_obj_align(machine_btn, nullptr, LV_ALIGN_IN_TOP_RIGHT, -141, y_offset - 4);
    //lv_obj_set_style_local_bg_opa(machine_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    machine_btns[i] = machine_btn;

    lv_obj_t* machine_label = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_align(machine_label, LV_LABEL_ALIGN_RIGHT);
    lv_label_set_long_mode(machine_label, LV_LABEL_LONG_BREAK);
    lv_obj_set_width(machine_label, 100);
    lv_obj_align(machine_label, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -145, y_offset);
    lv_obj_set_style_local_text_color(machine_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x640748));
    machine_labels[i] = machine_label;

    // Reps
    lv_obj_t* reps_btn = lv_btn_create(lv_scr_act(), nullptr);
    lv_obj_set_event_cb(reps_btn, SelectionEventHandler);
    lv_obj_add_style(reps_btn, LV_BTN_PART_MAIN, &btn_style);
    lv_obj_set_size(reps_btn, 44, 34);
    lv_obj_align(reps_btn, nullptr, LV_ALIGN_IN_TOP_RIGHT, -96, y_offset - 4);
    lv_obj_set_style_local_bg_opa(reps_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    reps_btns[i] = reps_btn;

    lv_obj_t* reps_label = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_align(reps_label, LV_LABEL_ALIGN_RIGHT);
    lv_label_set_long_mode(reps_label, LV_LABEL_LONG_BREAK);
    lv_obj_set_width(reps_label, 44);
    lv_obj_align(reps_label, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -100, y_offset);
    lv_obj_set_style_local_text_color(reps_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x640748));
    reps_labels[i] = reps_label;

    // Weight
    lv_obj_t* weight_btn = lv_btn_create(lv_scr_act(), nullptr);
    lv_obj_set_event_cb(weight_btn, SelectionEventHandler);
    lv_obj_add_style(weight_btn, LV_BTN_PART_MAIN, &btn_style);
    lv_obj_set_size(weight_btn, 80, 34);
    lv_obj_align(weight_btn, nullptr, LV_ALIGN_IN_TOP_RIGHT, -18, y_offset - 4);
    lv_obj_set_style_local_bg_opa(weight_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    weight_btns[i] = weight_btn;

    lv_obj_t* weight_label = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_align(weight_label, LV_LABEL_ALIGN_RIGHT);
    lv_label_set_long_mode(weight_label, LV_LABEL_LONG_BREAK);
    lv_obj_set_width(weight_label, 80);
    lv_obj_align(weight_label, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -22, y_offset);
    lv_obj_set_style_local_text_color(weight_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x640748));
    weight_labels[i] = weight_label;
  }

  // Prev button
  prev_btn = lv_btn_create(lv_scr_act(), nullptr);
  prev_btn->user_data = this;
  lv_obj_set_event_cb(prev_btn, ButtonEventHandler);
  lv_obj_add_style(prev_btn, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(prev_btn, 40, 40);
  lv_obj_align(prev_btn, nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 30, -30);
  lv_obj_t* labeled_prev_btn = lv_label_create(prev_btn, nullptr);
  lv_label_set_text_static(labeled_prev_btn, "<");
  lv_obj_set_style_local_text_color(labeled_prev_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xDE4C4A));

  // Next button
  next_btn = lv_btn_create(lv_scr_act(), nullptr);
  next_btn->user_data = this;
  lv_obj_set_event_cb(next_btn, ButtonEventHandler);
  lv_obj_add_style(next_btn, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(next_btn, 40, 40);
  lv_obj_align(next_btn, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, -30, -30);
  lv_obj_t* labeled_next_btn = lv_label_create(next_btn, nullptr);
  lv_label_set_text_static(labeled_next_btn, ">");
  lv_obj_set_style_local_text_color(labeled_next_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xDE4C4A));

  // Decrease button
  increase_btn = lv_btn_create(lv_scr_act(), nullptr);
  increase_btn->user_data = this;
  lv_obj_set_event_cb(increase_btn, ButtonEventHandler);
  lv_obj_add_style(increase_btn, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(increase_btn, 40, 40);
  lv_obj_align(increase_btn, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, -77, -30);
  lv_obj_t* labeled_increase_btn = lv_label_create(increase_btn, nullptr);
  lv_label_set_text_static(labeled_increase_btn, "+");
  lv_obj_set_style_local_text_color(labeled_increase_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xDE4C4A));

  // Increase button
  decrease_btn = lv_btn_create(lv_scr_act(), nullptr);
  decrease_btn->user_data = this;
  lv_obj_set_event_cb(decrease_btn, ButtonEventHandler);
  lv_obj_add_style(decrease_btn, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(decrease_btn, 40, 40);
  lv_obj_align(decrease_btn, nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 77, -30);
  lv_obj_t* labeled_decrease_btn = lv_label_create(decrease_btn, nullptr);
  lv_label_set_text_static(labeled_decrease_btn, "-");
  lv_obj_set_style_local_text_color(labeled_decrease_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xDE4C4A));

  // Initial order mapping when not loaded from storage
  for (uint8_t i = 0; i < machine_amount; i++) {
    gymData.order[i] = i;
  }

  LoadGymDataFromFile();
  Refresh();
}

HollyGym::~HollyGym() {
  SaveGymDataToFile();

  lv_style_reset(&gradient_border_style);
  lv_style_reset(&content_bg_style);
  lv_style_reset(&btn_style);
  lv_style_reset(&btn_style_bordercover);
  lv_obj_clean(lv_scr_act());

  lv_obj_clean(prev_btn);
  lv_obj_clean(next_btn);
  lv_obj_clean(increase_btn);
  lv_obj_clean(decrease_btn);

  for (uint8_t i = 0; i < machines_per_page; i++) {
    lv_obj_clean(machine_labels[i]);
    lv_obj_clean(reps_labels[i]);
    lv_obj_clean(weight_labels[i]);
    lv_obj_clean(machine_btns[i]);
    lv_obj_clean(reps_btns[i]);
    lv_obj_clean(weight_btns[i]);
  }

  for (uint8_t i = 0; i < machine_amount * 3; i++) {
    if (selections[i].label != nullptr) {
      lv_obj_clean(selections[i].label);
      lv_obj_clean(selections[i].btn);
    }
  }
}

void HollyGym::Refresh() {
  for (uint8_t i = 0; i < machines_per_page; i++) {
    uint8_t entry_idx = (current_page * machines_per_page) + i;

    lv_label_set_text_static(machine_labels[i], machine_names[gymData.order[entry_idx]]);
    lv_label_set_text_fmt(reps_labels[i], "%dx", gymData.reps[gymData.order[entry_idx]]);
    lv_label_set_text_fmt(weight_labels[i], "%.1fkg", gymData.weights[gymData.order[entry_idx]]);

    Selection* machine_selection = &selections[entry_idx];
    machine_selection->idx = entry_idx;
    machine_selection->metric = METRIC_MACHINE;
    machine_selection->label = machine_labels[i];
    machine_selection->btn = machine_btns[i];
    machine_selection->self = this;
    machine_btns[i]->user_data = machine_selection;

    Selection* reps_selection = &selections[machine_amount + entry_idx];
    reps_selection->idx = entry_idx;
    reps_selection->metric = METRIC_REPS;
    reps_selection->label = reps_labels[i];
    reps_selection->btn = reps_btns[i];
    reps_selection->self = this;
    reps_btns[i]->user_data = reps_selection;

    Selection* weight_selection = &selections[machine_amount * 2 + entry_idx];
    weight_selection->idx = entry_idx;
    weight_selection->metric = METRIC_WEIGHT;
    weight_selection->label = weight_labels[i];
    weight_selection->btn = weight_btns[i];
    weight_selection->self = this;
    weight_btns[i]->user_data = weight_selection;

  }

  lv_obj_set_hidden(prev_btn, current_page == 0 ? true : false);
  lv_obj_set_hidden(next_btn, current_page == machine_amount / machines_per_page - 1 ? true : false);

  if (active_selection) {
    lv_obj_set_hidden(increase_btn,
      active_selection->metric == METRIC_MACHINE && active_selection->idx == machine_amount - 1);
    lv_obj_set_hidden(decrease_btn,
      active_selection->metric == METRIC_MACHINE && active_selection->idx == 0);
  } else {
    lv_obj_set_hidden(increase_btn, true);
    lv_obj_set_hidden(decrease_btn, true);
  }
}

void HollyGym::RemoveSelection() {
  if (active_selection != nullptr) {
    if (active_selection->metric == METRIC_MACHINE) {
      lv_obj_add_style(active_selection->btn, LV_BTN_PART_MAIN, &btn_style_bordercover);
    } else {
      lv_obj_set_style_local_bg_opa(active_selection->btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    }
    lv_obj_set_style_local_text_color(active_selection->label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x640748));
    active_selection = nullptr;
  }
}

void HollyGym::SetSelection(Selection* selection) {
  if (selection != nullptr) {
    RemoveSelection();
    if (selection->metric == METRIC_MACHINE) {
      lv_obj_add_style(selection->btn, LV_BTN_PART_MAIN, &btn_style);
    } else {
      lv_obj_set_style_local_bg_opa(selection->btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_100);
    }
    lv_obj_set_style_local_text_color(selection->label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xDE4C4A));
    active_selection = selection;
  }
}

void HollyGym::OnButtonEvent(lv_obj_t* object, lv_event_t event, Selection* selection) {
  if (event == LV_EVENT_CLICKED) {
    if (object == prev_btn) {
      current_page--;
      RemoveSelection();
    } else if (object == next_btn) {
      current_page++;
      RemoveSelection();
    } else if (object == increase_btn) {
      if (active_selection->metric == METRIC_REPS) {
        if (gymData.reps[active_selection->idx] < 99) {
          gymData.reps[active_selection->idx] += 1;
        }
      } else if (active_selection->metric == METRIC_WEIGHT) {
        if (gymData.reps[active_selection->idx] < 97.0) {
          gymData.weights[active_selection->idx] += 2.5;
        }
      } else if (active_selection->metric == METRIC_MACHINE) {
        if (active_selection->idx < machine_amount - 1) {
          uint8_t tmp = gymData.order[active_selection->idx];
          gymData.order[active_selection->idx] = gymData.order[active_selection->idx + 1];
          gymData.order[active_selection->idx + 1] = tmp;

          Selection* new_selection = &selections[active_selection->idx + 1];
          if (new_selection->idx % machines_per_page == 0) {
            RemoveSelection();
          } else {
            SetSelection(new_selection);
          }
        }
      }
    } else if (object == decrease_btn) {
      if (active_selection->metric == METRIC_REPS) {
        if (gymData.reps[active_selection->idx] > 0) {
          gymData.reps[active_selection->idx] -= 1;
        }
      } else if (active_selection->metric == METRIC_WEIGHT) {
        if (gymData.weights[active_selection->idx] > 2.0) {
          gymData.weights[active_selection->idx] -= 2.5;
        }
      } else if (active_selection->metric == METRIC_MACHINE) {
        if (active_selection->idx > 0) {
          uint8_t tmp = gymData.order[active_selection->idx];
          gymData.order[active_selection->idx] = gymData.order[active_selection->idx - 1];
          gymData.order[active_selection->idx - 1] = tmp;

          Selection* new_selection = &selections[active_selection->idx - 1];
          if (new_selection->idx % machines_per_page == machines_per_page - 1) {
            RemoveSelection();
          } else {
            SetSelection(new_selection);
          }
        }
      }
    } else {
      if (active_selection == selection) {
        RemoveSelection();
      } else {
        SetSelection(selection);
      }
    }

    Refresh();
  }
}

void HollyGym::LoadGymDataFromFile() {
  GymData bufferGymData;
  lfs_file_t gymDataFile;

  if (fs.FileOpen(&gymDataFile, "/gym.dat", LFS_O_RDONLY) != LFS_ERR_OK) {
    return;
  }

  fs.FileRead(&gymDataFile, reinterpret_cast<uint8_t*>(&bufferGymData), sizeof(gymData));
  fs.FileClose(&gymDataFile);

  if (bufferGymData.version == gymDataVersion) {
    gymData = bufferGymData;
  }
}

void HollyGym::SaveGymDataToFile() {
  lfs_file_t gymDataFile;

  if (fs.FileOpen(&gymDataFile, "/gym.dat", LFS_O_WRONLY | LFS_O_CREAT) != LFS_ERR_OK) {
    return;
  }

  fs.FileWrite(&gymDataFile, reinterpret_cast<uint8_t*>(&gymData), sizeof(gymData));
  fs.FileClose(&gymDataFile);
}
