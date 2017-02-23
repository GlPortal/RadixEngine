#include <radix/util/Profiler.hpp>

#include <functional>
#include <sstream>
#include <stack>

#include <ciso646>

namespace radix {
namespace util {

using ns = std::chrono::nanoseconds;
using ms = std::chrono::milliseconds;

std::string Profiler::dump() const {
  std::ostringstream oss;
  std::stack<std::reference_wrapper<const Profiler>> profstack;
  std::stack<unsigned int> depthstack;
  profstack.push(std::cref(*this));
  depthstack.push(0);
  while (not profstack.empty()) {
    const Profiler &prof = profstack.top().get();
    unsigned int depth = depthstack.top();
    profstack.pop();
    depthstack.pop();
    decltype(m_subprofilers)::const_reverse_iterator rit = prof.m_subprofilers.crbegin();
    for (; rit != prof.m_subprofilers.crend(); ++rit) {
      profstack.push(std::cref(rit->second));
      depthstack.push(depth + 1);
    }
    oss << std::string(depth * 2, ' ') << prof.m_name << ": " <<
        std::chrono::duration_cast<ns>(prof.delta()).count() << " ns" <<
        " (" << std::chrono::duration_cast<ms>(prof.delta()).count() << " ms)" << std::endl;
  }
  return oss.str();
}

} /* namespace util */
} /* namespace radix */
