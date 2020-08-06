#include "linux_system.h"
#include "ncurses_display.h"

int main() {
  Linux_System system;
  NCursesDisplay::Display(system);
}