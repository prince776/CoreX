#include <cassert>

extern "C" void Assert(bool x) {
    return assert(x);
}
