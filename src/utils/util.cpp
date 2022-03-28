#include "util.hpp"
#include <string>


namespace kong {

pid_t GetThreadId()
{
    return pthread_self();
}

uint32_t GetFiberId()
{
    return 0;
}


} // namespace kong