#ifndef RADIX_UTIL_PROFILER_HPP
#define RADIX_UTIL_PROFILER_HPP

#include <chrono>
#include <map>
#include <string>

namespace radix {
namespace util {

class Profiler {
protected:
  std::string m_name;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_begin, m_end;
  std::map<std::string, Profiler> m_subprofilers;

public:
  Profiler(const std::string &name) :
    m_name(name) {
  }

  Profiler& operator[](const std::string &key) {
    decltype(m_subprofilers)::iterator it = m_subprofilers.find(key);
    if (it == m_subprofilers.end()) {
      return m_subprofilers.emplace(std::piecewise_construct,
              std::forward_as_tuple(key),
              std::forward_as_tuple(key)
          ).first->second;
    }
    return it->second;
  }

  void start() {
    m_begin = std::chrono::high_resolution_clock::now();
  }

  void stop() {
    m_end = std::chrono::high_resolution_clock::now();
  }

  std::string name() const {
    return m_name;
  }

  decltype(m_begin) begin() const {
    return m_begin;
  }

  decltype(m_end) end() const {
    return m_end;
  }

  decltype(m_end - m_begin) delta() const {
    return m_end - m_begin;
  }

  std::string dump() const;
};

class ScopedProfile {
protected:
  Profiler &m_prof;

public:
  ScopedProfile(Profiler &prof) :
    m_prof(prof) {
  }

  ~ScopedProfile() {
    m_prof.stop();
  }

  void stop() {
    m_prof.stop();
  }
};

} /* namespace util */
} /* namespace radix */

#endif /* RADIX_UTIL_PROFILER_HPP */
