/*
 * This code is distributed under the terms of the GNU General Public License.
 * For more information, please refer to the LICENSE file in the root directory.
 * -------------------------------------------------
 * Copyright (C) 2025 Rodrigo R.
 * This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type `show c' for details.
*/

#ifndef FLUENT_LIBC_CLOCK_LIBRARY_H
#define FLUENT_LIBC_CLOCK_LIBRARY_H

// ============= FLUENT LIB C =============

#ifdef _WIN32
#   include <windows.h>
#else
#   include <time.h>
#endif

/**
 * \brief Returns the current monotonic time in nanoseconds.
 *
 * This function provides a high-resolution timer for both Windows and POSIX systems.
 * On Windows, it uses QueryPerformanceCounter and QueryPerformanceFrequency to compute
 * the elapsed time in nanoseconds. On POSIX systems, it uses clock_gettime with
 * CLOCK_MONOTONIC for nanosecond precision.
 *
 * \return The current monotonic time in nanoseconds as a 64-bit integer.
 */
static inline long long get_nano_time()
{
#ifdef _WIN32
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    // Convert to nanoseconds: (counter / frequency) * 1e9
    return (long long)((double)counter.QuadPart / frequency.QuadPart * 1e9);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
#endif
}

/**
 * \brief Calculates the elapsed time in nanoseconds since a given start time.
 *
 * This function returns the difference between the current monotonic time and
 * the provided \p start_time, both in nanoseconds.
 *
 * \param start_time The starting time in nanoseconds, typically obtained from get_nano_time().
 * \return The elapsed time in nanoseconds as a 64-bit integer.
 */
static long long time_since(const long long start_time)
{
    return get_nano_time() - start_time;
}

#endif //FLUENT_LIBC_CLOCK_LIBRARY_H