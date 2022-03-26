/**
 * @file util.h
 * @brief 常用的工具函数
 */

#pragma once

#include <cstdint>
#include <pthread.h>
#include <string>


namespace kong
{
    pid_t GetThreadId();

    uint32_t GetFeberId();
} // namespace kong


