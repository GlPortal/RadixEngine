#include <radix/core/diag/AnsiConsoleLogger.hpp>

#include <algorithm>
#include <iostream>

#ifndef _WIN32
#include <unistd.h>
#endif

using std::cout;

namespace radix {

/*! \cond PRIVATE */
static const struct LogLevelOutputInfo {
  char letter;
  const char *colorCode;
  const char *bgColorCode;
} outInfo[] = {
  /* This array must have the same order as the LogLevel enum */
  {'V', "238", "235"},
  {'D', "2", "22"},
  {'I', "20", "17"},
  {'W', "220", "94"},
  {'E', "160", "52"},
  {'F', "92", "53"}
};
/*! \endcond */

AnsiConsoleLogger::AnsiConsoleLogger() :
  enableColors(true),
  enableBackground(false) {
#ifndef _WIN32
  const auto term = getenv("TERM");
  const bool tty = isatty(fileno(stdout));
  if (not tty or
      (term != nullptr and std::string(term) == "linux")) {
    // Linux fbcon VTs don't handle extended colors
    // This is also what's reported by some IDEs' output window which aren't full fledged terminals
    enableColors = false;
  }
#endif
}

const char* AnsiConsoleLogger::getName() const {
  return "ANSI/vt100 logger";
}

int getBackgroundColor(const std::string &tag) {
  if (tag.empty()) {
    return 233; // Dark gray
  }
  uint8_t bg = 0xAA;
  for (size_t i = 0; i < tag.size(); ++i) {
    bg ^= (uint8_t)tag[i] ^ i;
  }
  return bg%(231-15)+16;
}

bool needBlackText(int bgcol) {
  // Gets the color position in the 6x6x6 color cube
  // http://www.pixelbeat.org/docs/terminal_colours/
  const int x = ((bgcol-16)%36)%6, y = ((bgcol-16)%36)/6, z = (bgcol-16)/36;
  // Bottom and right parts of the cube are the brightest. Higher you go, brightest also.
  return x > 3 || y > 3 || (z >= 3 && (x > 2 || y > 2));
}

void AnsiConsoleLogger::log(const std::string &message, LogLevel lvl, const std::string &tag) {
  std::unique_lock<std::mutex> lk(mtx);

  const LogLevelOutputInfo &oi = outInfo[(int)lvl];
  if (enableColors) {
    cout << "\x1B[97m\x1B[48;5;" << oi.colorCode << 'm';
  }
  cout << oi.letter;
  if (enableColors) {
    int bgcol = getBackgroundColor(tag);
    cout << "\x1B[48;5;" << bgcol << 'm';
    if (needBlackText(bgcol)) {
      cout << "\x1B[30m";
    }
  }
  cout << std::string(16-std::min(16, (int)tag.size()), ' ') << tag.substr(0, 16) << ' ';
  if (enableColors) {
    if (enableBackground) {
      cout << "\x1B[97m\x1B[48;5;" << oi.bgColorCode << "m ";
    } else {
      cout << "\x1B[0m";
    }
  }
  cout << ' ' << message;
  if (enableColors) {
    cout << "\x1B[0m";
  }
  cout << std::endl;
}

} /* namespace radix */
