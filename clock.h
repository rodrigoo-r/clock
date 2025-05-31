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

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
extern "C"
{
#endif

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

/**
 * \struct hr_clock_t
 * \brief High-resolution clock structure for time measurement.
 *
 * This structure holds the start time in nanoseconds, typically used
 * for measuring elapsed time intervals with high precision.
 */
typedef struct
{
    long long start_time; ///< The start time in nanoseconds
} hr_clock_t;

/**
 * \enum hr_clock_time_unit_t
 * \brief Enumeration of time units for high-resolution clock measurements.
 *
 * This enum defines various time units that can be used to represent
 * elapsed time or durations, ranging from nanoseconds to days.
 */
typedef enum
{
    CLOCK_NANOSECONDS = 0, ///< Time unit in nanoseconds
    CLOCK_MICROSECONDS,    ///< Time unit in microseconds
    CLOCK_MILLISECONDS,    ///< Time unit in milliseconds
    CLOCK_SECONDS,         ///< Time unit in seconds
    CLOCK_MINUTES,         ///< Time unit in minutes
    CLOCK_HOURS,           ///< Time unit in hours
    CLOCK_DAYS,            ///< Time unit in days
} hr_clock_time_unit_t;

/**
 * \brief Sets the start time of the high-resolution clock to the current time.
 *
 * This function updates the \p start_time field of the provided \p hr_clock_t structure
 * to the current monotonic time in nanoseconds. If the provided pointer is NULL,
 * the function returns without making any changes.
 *
 * \param clock Pointer to an \p hr_clock_t structure to update.
 */
static inline void hr_clock_tick(hr_clock_t *const clock)
{
    if (clock == NULL)
    {
        return; // Handle null pointer
    }

    clock->start_time = get_nano_time(); // Set the start time to the current time
}

/**
 * \brief Converts a time duration in nanoseconds to the specified time unit.
 *
 * This function takes a duration in nanoseconds and converts it to the desired
 * time unit as specified by the \p unit parameter. Supported units include
 * nanoseconds, microseconds, milliseconds, seconds, minutes, hours, and days.
 *
 * \param nanos The duration in nanoseconds to convert.
 * \param unit The target time unit for conversion (see \p hr_clock_time_unit_t).
 * \return The converted duration in the specified unit, or -1 if the unit is invalid.
 */
static inline long long clock_nanos_to_unit(
    const long long nanos,
    const hr_clock_time_unit_t unit
)
{
    switch (unit)
    {
        case CLOCK_NANOSECONDS:
            return nanos;
        case CLOCK_MICROSECONDS:
            return nanos / 1000;
        case CLOCK_MILLISECONDS:
            return nanos / 1000000;
        case CLOCK_SECONDS:
            return nanos / 1000000000;
        case CLOCK_MINUTES:
            return nanos / 60000000000LL;
        case CLOCK_HOURS:
            return nanos / 3600000000000LL;
        case CLOCK_DAYS:
            return nanos / 86400000000000LL;
        default:
            return -1l; // Invalid unit
    }
}

/**
 * \brief Calculates the elapsed time between two high-resolution clock instances in the specified unit.
 *
 * This function computes the difference between the start times of two \p hr_clock_t structures,
 * returning the elapsed time converted to the desired time unit.
 *
 * \param clock Pointer to the starting \p hr_clock_t structure.
 * \param other Pointer to the ending \p hr_clock_t structure.
 * \param unit The time unit in which to return the elapsed time (see \p hr_clock_time_unit_t).
 * \return The elapsed time between \p clock and \p other in the specified unit, or -1 if any pointer is NULL or the unit is invalid.
 */
static inline long long hr_clock_distance(
    const hr_clock_t *const clock,
    const hr_clock_t *const other,
    const hr_clock_time_unit_t unit
)
{
    if (clock == NULL || other == NULL)
    {
        return -1l; // Handle null pointers
    }

    return clock_nanos_to_unit(other->start_time - clock->start_time, unit);
}

/**
 * \brief Calculates the elapsed time from the given high-resolution clock to now in the specified unit.
 *
 * This function computes the time difference between the current monotonic time and the
 * start time stored in the provided \p hr_clock_t structure, converting the result to the
 * specified time unit.
 *
 * \param clock Pointer to the \p hr_clock_t structure representing the start time.
 * \param unit The time unit in which to return the elapsed time (see \p hr_clock_time_unit_t).
 * \return The elapsed time from \p clock to now in the specified unit, or -1 if the pointer is NULL or the unit is invalid.
 */
static inline long long hr_clock_distance_from_now(
    const hr_clock_t *const clock,
    const hr_clock_time_unit_t unit
)
{
    if (clock == NULL)
    {
        return -1l; // Handle null pointer
    }

    // Calculate the elapsed time from the start time to now
    return clock_nanos_to_unit(
        get_nano_time() - clock->start_time,
        unit
    );
}

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif

#endif //FLUENT_LIBC_CLOCK_LIBRARY_H