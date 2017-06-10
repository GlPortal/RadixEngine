# - Finds if the compiler has C++14 support
# This module can be used to detect compiler flags for using C++14, and checks
# a small subset of the language.
#
# The following variables are set:
#   CXX14_FLAGS - flags to add to the CXX compiler for C++14 support
#   CXX14_FOUND - true if the compiler supports C++14
#
# TODO: When compilers starts implementing the whole C++14, check the full set

include(CheckCXXSourceCompiles)
include(FindPackageHandleStandardArgs)

set(CXX14_FLAG_CANDIDATES
    # GCC 6+ and everything that automatically accepts C++14
    " "
    # GCC < 6 and Intel Linux
    "-std=c++14"
    # Intel Windows
    "/Qstd=c++14"
    )

set(CXX14_TEST_SOURCE
"
[[deprecated]] void unused() {}

template<typename T>
constexpr T pi = T(3.1415926535897932385);

constexpr int numberwang(int n) {
    if (n < 0) {
        return 0b1'1001'0101;
    }
    return n - 1;
}

auto lambda = [v = 0](auto a, auto b) { return v + a * b; };

auto square(int n)  {
    return lambda(n, n) * numberwang(2);
}

int main() {
    int s = square(3);
    double pie = pi<double>;
    return 0;
}
")

set(SAVE_CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
foreach(FLAG ${CXX14_FLAG_CANDIDATES})
    set(CMAKE_REQUIRED_FLAGS "${FLAG}")
    unset(CXX14_FLAG_DETECTED CACHE)
    message(STATUS "Trying C++14 flag '${FLAG}'")
    check_cxx_source_compiles("${CXX14_TEST_SOURCE}" CXX14_FLAG_DETECTED)
    if(CXX14_FLAG_DETECTED)
        set(CXX14_FLAGS_INTERNAL "${FLAG}")
        break()
    endif(CXX14_FLAG_DETECTED)
endforeach(FLAG ${CXX14_FLAG_CANDIDATES})
set(CMAKE_REQUIRED_FLAGS "${SAVE_CMAKE_REQUIRED_FLAGS}")
unset(SAVE_CMAKE_REQUIRED_FLAGS)

set(CXX14_FLAGS "${CXX14_FLAGS_INTERNAL}")

find_package_handle_standard_args(CXX14 DEFAULT_MSG CXX14_FLAGS)
mark_as_advanced(CXX14_FLAGS)
