#pragma once

#include <array>
#include <lvgl/lvgl.h>
#include "displayapp/screens/Screen.h"
#include "components/fs/FS.h"

typedef enum
{
  METRIC_UNSET,
  METRIC_MACHINE,
  METRIC_REPS,
  METRIC_WEIGHT,
} MetricType;

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class HollyGym : public Screen {
      public:
        HollyGym(DisplayApp* app, Pinetime::Controllers::FS& fs);
        ~HollyGym() override;

        struct Selection {
          uint8_t pos = 0;
          MetricType metric = METRIC_UNSET;
          HollyGym* for_cb = nullptr;
        };

        void OnButtonEvent(lv_obj_t* object, lv_event_t event, Selection* selection);

      private:
        Pinetime::Controllers::FS& fs;

        lv_style_t gradient_border_style;
        lv_style_t content_bg_style;
        lv_style_t btn_style;
        lv_style_t btn_style_bordercover;

        void Refresh() override;

        void RemoveSelection();
        void SetSelection(Selection* selection);

        void LoadGymDataFromFile();
        void SaveGymDataToFile();

        uint8_t current_page = 0;

        static const uint8_t machine_amount = 40; // when changed, GymData version needs to be upped
        static const uint8_t machines_per_page = 4; // machine_amount should be divisible by this
        static constexpr float weight_increments = 2.5f;

        std::array<Selection, machines_per_page * 3> selections;
        Selection* active_selection = nullptr;

        static constexpr std::array<const char*, machine_amount> machine_names = {
          // Max. 7 chars
          "Should.",
          "Lw.back",
          "Up.back",
          "ChstPrs",

          "Biceps",
          "LegPrs.",
          "Corner?",
          "-",

          "-",
          "-",
          "-",
          "-",

          "1",
          "2",
          "3",
          "4",

          "5",
          "6",
          "7",
          "8",

          "9",
          "10",
          "11",
          "12",

          "13",
          "14",
          "15",
          "16",

          "17",
          "18",
          "19",
          "20",

          "21",
          "22",
          "23",
          "24",

          "-",
          "-",
          "-",
          "-",
        };

        std::array<lv_obj_t*, machines_per_page> machine_labels;
        std::array<lv_obj_t*, machines_per_page> machine_btns;
        std::array<lv_obj_t*, machines_per_page> reps_labels;
        std::array<lv_obj_t*, machines_per_page> weight_labels;
        std::array<lv_obj_t*, machines_per_page> reps_btns;
        std::array<lv_obj_t*, machines_per_page> weight_btns;

        lv_obj_t* prev_btn;
        lv_obj_t* next_btn;
        lv_obj_t* increase_btn;
        lv_obj_t* decrease_btn;

        static constexpr uint32_t gymDataVersion = 0x0002;
        struct GymData {
          uint32_t version = gymDataVersion;
          std::array<uint8_t, machine_amount> reps = {};
          std::array<float, machine_amount> weights = {};
          std::array<uint8_t, machine_amount> order = {};
        } gymData;
      };
    }
  }
}
