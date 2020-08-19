#if defined(__linux) || defined(__linux__) || defined(linux)
#define LINUX
#elif defined(__APPLE__)
#define MACOS
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64)
#define WINDOWS
#endif

#if defined(LINUX)
#include "linux/linux_system.h"
#elif defined(MACOS)
#include "macosx/macosx_system.h"
#endif

#include "ncurses_display.h"

int main() {
#if defined(LINUX)
  Linux_System system;
#elif defined(MACOS)
  MacOSX_System system;
#endif
  NCursesDisplay::Display(system, 25);
}