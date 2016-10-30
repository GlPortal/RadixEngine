#ifndef VBO_HPP
#define VBO_HPP

#include <cstdint>
#include <vector>

namespace radix {

class VBO {
protected:
  std::size_t size;
  unsigned int id;

public:
  enum Usage : uint8_t {
    Static = 0,
    Dynamic,
    Stream,

    Draw = 1 << 2,
    Read,
    Copy
  };
  constexpr static Usage DefaultUsage = static_cast<Usage>(Static | Draw);

  // Ctor / dtor
  VBO();
  VBO(std::size_t size, Usage usage = DefaultUsage);
  ~VBO();
  // No copy
  VBO(const VBO&) = delete;
  VBO& operator=(const VBO&) = delete;
  // Move
  VBO(VBO&&);
  VBO& operator=(VBO&&);
  
  operator unsigned int() const { return id; }
  void setSize(std::size_t size, Usage usage = DefaultUsage);
  std::size_t getSize() const {
    return size;
  }

  void setData(const void *data, std::size_t count, Usage usage = DefaultUsage);

  template <typename T>
  void setData(const std::vector<T> &data, Usage usage = DefaultUsage) {
    setData(data.data(), data.size()*sizeof(T), usage);
  }
  template <typename T>
  void setData(const T *data, std::size_t count, Usage usage = DefaultUsage) {
    setData(data, count*sizeof(T), usage);
  }

  void update(const void *data, std::size_t count = 0, std::size_t offset = 0);

  template <typename T>
  void update(const std::vector<T> &data, std::size_t count = 0, std::size_t offset = 0) {
    if (count == 0) {
      count = data.size();
    }
    update(data.data(), count*sizeof(T), offset);
  }
  template <typename T>
  void update(const T *data, std::size_t count, std::size_t offset = 0) {
    update(data, count*sizeof(T), offset);
  }
  void bind() const;
};

constexpr inline VBO::Usage operator|(const VBO::Usage a, const VBO::Usage b) {
  return static_cast<VBO::Usage>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

} /* namespace radix */

#endif /* VBO_HPP */
