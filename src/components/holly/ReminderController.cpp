#include "components/holly/ReminderController.h"
#include "systemtask/SystemTask.h"
#include "task.h"
#include <chrono>

using namespace Pinetime::Controllers;
using namespace std::chrono_literals;

ReminderController::ReminderController() {
}

namespace {
  void SetOffAlarm(TimerHandle_t xTimer) {
    auto controller = static_cast<Pinetime::Controllers::ReminderController*>(pvTimerGetTimerID(xTimer));
    controller->SetOffReminderNow();
  }
}

void ReminderController::Init(System::SystemTask* systemTask) {
  this->systemTask = systemTask;
  alarmTimer = xTimerCreate("Reminder", 1, pdFALSE, this, SetOffAlarm);
}

void ReminderController::SetReminder(uint8_t min) {
  xTimerStop(alarmTimer, 0);
  xTimerChangePeriod(alarmTimer, min * 60 * configTICK_RATE_HZ, 0);
  xTimerStart(alarmTimer, 0);
}

bool ReminderController::IsReminderActive() {
  return xTimerIsTimerActive(alarmTimer) != pdFALSE;
}

void ReminderController::DisableReminder() {
  xTimerStop(alarmTimer, 0);
}

void ReminderController::SetOffReminderNow() {
  systemTask->PushMessage(System::Messages::SetOffReminder);
}

uint32_t ReminderController::SecondsToReminder() {
  TickType_t remainingTime = xTimerGetExpiryTime(alarmTimer) - xTaskGetTickCount();
  return remainingTime / configTICK_RATE_HZ;
}

void ReminderController::StopAlerting() {
  systemTask->PushMessage(System::Messages::StopRinging);
}
