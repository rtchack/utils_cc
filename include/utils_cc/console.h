/**
 * Created on: 6/27/17
 *     Author: xing
 */

#pragma once

#include <termios.h>
#include <unistd.h>
#include <stdio.h>

namespace utils
{
/**
 * Enable or disable the echo of console
 */
void
enable_console_echo(bool enable = true)
{
  termios tm{};
  tcgetattr(STDIN_FILENO, &tm);
  if (enable) {
    tm.c_lflag |= ECHO;
  } else {
    tm.c_lflag &= ~ECHO;
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &tm);
}

/**
 * Disable console echo
 */
class NoConsoleEcho final
{
 public:
  NoConsoleEcho() { enable_console_echo(false); }

  ~NoConsoleEcho()
  {
    enable_console_echo();
    puts("");
  }
};

}  // namespace utils
