#pragma once

#include <FreeRTOS.h>
#include <timers.h>
#include <cstdint>
#include "components/settings/Settings.h"

namespace Pinetime {
  namespace System {
    class SystemTask;
  }
  namespace Controllers {
    class ReminderController {
    public:
      ReminderController();

      void Init(System::SystemTask* systemTask);
      void SetReminder(uint8_t min);
      void DisableReminder();
      void SetOffRemindermNow();

      uint32_t SecondsToReminder();
      void StopAlerting();

    private:
      System::SystemTask* systemTask = nullptr;
      TimerHandle_t alarmTimer;
    };
  }
}
