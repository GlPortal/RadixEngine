#include <radix/core/diag/LogInput.hpp>

#include <iomanip>
#include <sstream>
#include <string>

#include <radix/core/math/Vector2f.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Vector4f.hpp>

namespace radix {

LogInput::LogInput(Logger &sink, LogLevel lvl, const char *tag) :
  sink(sink), lvl(lvl), tag(tag) {
}

LogInput::LogInput(Logger &sink, LogLevel lvl, const std::string &tag) :
  sink(sink), lvl(lvl), tag(tag) {
}

LogInput::~LogInput() {
  if ( !buf.empty()) {
    sink.log(buf, lvl, tag);
  }
}

LogInput::LogInput(LogInput &&o) :
  sink(o.sink), lvl(o.lvl), buf(o.buf) {
  o.buf.clear();
}

LogInput& LogInput::operator<<(const char *s) {
  buf.append(s);
  return *this;
}

LogInput& LogInput::operator<<(const std::string &s) {
  buf.append(s);
  return *this;
}

LogInput& LogInput::operator<<(const stx::string_view &s) {
  buf.append(s.cbegin(), s.cend());
  return *this;
}


LogInput& LogInput::operator<<(bool b) {
  buf.append(b ? "true" : "false");
  return *this;
}

LogInput& LogInput::operator<<(char c) {
  buf.push_back(c);
  return *this;
}

LogInput& LogInput::operator<<(const void *ptr) {
  std::stringbuf hbuf;
  std::ostream os(&hbuf);
  os << "0x" << std::setfill('0') << std::setw(sizeof(ptr) * 2) << std::hex << (uint64_t)ptr;
  buf.append(hbuf.str());
  return *this;
}


LogInput& LogInput::operator<<(const Vector2f &v) {
  *this << '(' << v.x << ", " << v.y << ')';
  return *this;
}

LogInput& LogInput::operator<<(const Vector3f &v) {
  *this << '(' << v.x << ", " << v.y << ", " << v.z << ')';
  return *this;
}

LogInput& LogInput::operator<<(const Vector4f &v) {
  *this << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
  return *this;
}

} /* namespace radix */
