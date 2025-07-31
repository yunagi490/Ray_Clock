#include "Raylib.h"
#include "../config/02_Clock.h"
#include "../config/01_GuiButton.h"
#include "../config/00_AppState.h"
#include "../config/03_Alarm.h"

int main()
{
  InitWindow(640, 480, "Clock App");
  SetTargetFPS(10);

  InitAudioDevice();
  Sound alarmSound = LoadSound("Resource/Sounds/alarm.wav");

  AppState state = {MODE_CLOCK, true};
  Clock clock;
  Alarm alarm;
  char timeStr[16];

  // GUIボタン定義
  GuiButton btnClock(10, 10, 100, 30, "Clock");
  GuiButton btnAlarm(120, 10, 100, 30, "Alarm");
  GuiButton btnTimer(230, 10, 100, 30, "Timer");
  GuiButton btnStopWatch(340, 10, 120, 30, "StopWatch");
  GuiButton btnMode(480, 10, 100, 30, "Light/Dark");
  GuiButton btnExit(590, 10, 40, 30, "X");

  while (!WindowShouldClose())
  {
    // 色設定
    Color bgColor = state.darkMode ? BLACK : WHITE;
    Color fgColor = state.darkMode ? WHITE : BLACK;
    Color borderColor = state.darkMode ? GRAY : DARKGRAY;

    // 描画開始
    BeginDrawing();
    ClearBackground(bgColor);

    // モードによる画面描画
    if (state.currentMode == MODE_ALARM)
    {
      DrawAlarmMode(alarm, fgColor);

      if (alarm.enabled && !alarm.triggered &&
          clock.hour == alarm.hour && clock.minute == alarm.minute)
      {
        alarm.triggered = true;
        PlaySound(alarmSound);
      }

      if (alarm.triggered && IsKeyPressed(KEY_Z))
      {
        StopSound(alarmSound);
        alarm.triggered = false;
      }
    }

    // ボタン描画＆処理
    btnClock.Draw(fgColor, bgColor, borderColor, btnClock.isClicked());
    btnAlarm.Draw(fgColor, bgColor, borderColor, btnAlarm.isClicked());
    btnTimer.Draw(fgColor, bgColor, borderColor, btnTimer.isClicked());
    btnStopWatch.Draw(fgColor, bgColor, borderColor, btnStopWatch.isClicked());
    btnMode.Draw(fgColor, bgColor, borderColor, btnMode.isClicked());
    btnExit.Draw(fgColor, bgColor, borderColor, btnExit.isClicked());

    if (btnClock.isClicked())
      state.currentMode = MODE_CLOCK;
    if (btnAlarm.isClicked())
      state.currentMode = MODE_ALARM;
    if (btnTimer.isClicked())
      state.currentMode = MODE_TIMER;
    if (btnStopWatch.isClicked())
      state.currentMode = MODE_STOPWATCH;
    if (btnMode.isClicked())
      state.darkMode = !state.darkMode;
    if (btnExit.isClicked())
      break;

    // 時計描画
    switch (state.currentMode)
    {
    case MODE_CLOCK:
      clock.Update();
      clock.GetTimeString(timeStr, sizeof(timeStr));
      DrawText(timeStr, 280, 240, 20, fgColor);
      break;
    case MODE_ALARM:
      DrawText("Alarm Mode", 280, 240, 20, fgColor);
      break;
    case MODE_TIMER:
      DrawText("Timer Mode", 280, 240, 20, fgColor);
      break;
    case MODE_STOPWATCH:
      DrawText("Stopwatch Mode", 280, 240, 20, fgColor);
      break;
    }

    EndDrawing();
  }

  UnloadSound(alarmSound);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}