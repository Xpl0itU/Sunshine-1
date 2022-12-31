#include <stdio.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#define TRAY_WINAPI 1
//clang-format off
#include <windows.h>
#include <ShellAPI.h>
//clang-format on
#elif defined(__linux__) || defined(linux) || defined(__linux)
#define TRAY_APPINDICATOR 1
#elif defined(__APPLE__) || defined(__MACH__)
#define TRAY_APPKIT 1
#endif

#include "httpcommon.h"
#include "tray.h"
#include <curl/curl.h>

#if TRAY_APPINDICATOR
#define TRAY_ICON1 "indicator-messages"
#define TRAY_ICON2 "indicator-messages-new"
#elif TRAY_APPKIT
#define TRAY_ICON1 "icon.png"
#define TRAY_ICON2 "icon.png"
#elif TRAY_WINAPI
#define TRAY_ICON1 "icon.ico"
#define TRAY_ICON2 "icon.ico"
#endif

static struct tray tray;

static void open_web_ui(struct tray_menu *item) {
#if TRAY_WINAPI
  ShellExecute(NULL, NULL, "https://localhost:47990", NULL, NULL, SW_RESTORE);
#elif TRAY_APPINDICATOR
  system("xdg-open https://localhost:47990");
#elif TRAY_APPKIT
  system("open https://localhost:47990");
#endif
  tray_update(&tray);
}

int main() {
  struct tray_menu menus[] = {
    { "Open Interface",
      0,
      0,
      0,
      open_web_ui,
      NULL,
      NULL },
    { NULL }
  };
  tray = {
    TRAY_ICON1,
    "Sunshine",
    menus
  };
  if(tray_init(&tray) < 0) {
    printf("failed to create tray\n");
    return 1;
  }
  while(tray_loop(1) == 0) {
    printf("iteration\n");
  }
  return 0;
}