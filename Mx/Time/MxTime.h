#pragma once

#ifndef MX_TIME_H_
#define MX_TIME_H_

#include <chrono>

namespace Mix {
    class Time final {
        friend class MixEngine;

    public:
        Time() = delete;

        /**
         *  @note Caution: At the very moment this class is reset, delta time equals 0.
         */
        static float DeltaTime() { return mDeltaTime; }

        /** @note Equivalent to Time.time of Unity5. */
        static float TotalTime() { return mTime; }

        /** @note Bullet3 uses an internal fixed time step of 60 Hertz. Thus the default value is 1/60. */
        static float FixedDeltaTime() { return mFixedDeltaTime; }

        static void FixedDeltaTime(const float _fixedDeltaTime) { mFixedDeltaTime = _fixedDeltaTime; }

        static float FixedTime() { return mFixedTime; }

        static float MaximumDeltaTime() { return mMaximumDeltaTime; }

        /**
         *  @brief Set the maximum time of a frame. If a frame takes longer than it, fixedUpdate() 
         *  will not be performed.
         *  @param _maximumDeltaTime The maximum time a frame can take.
         *  It is advisable to keep maximum time for frame between 1/10 and 1/3 of a second.
         */
        static void MaximumDeltaTime(const float _maximumDeltaTime) { mMaximumDeltaTime = _maximumDeltaTime; }

    private:
        using Clock = std::chrono::steady_clock;
        using Duration = Clock::duration;
        using TimePoint = std::chrono::time_point<Clock>;

        static float mDeltaTime,
                     mTime,
                     mFixedDeltaTime,
                     mFixedTime,
                     mMaximumDeltaTime;

        static TimePoint mStart, mPrev, mCurr;

        static float mSmoothing;
        static int mFixedClampedSteps;

        static void Init() {
            mDeltaTime = 0.0f;
            mTime = 0.0f;
            mFixedDeltaTime = 1.0f / 60;
            mFixedTime = 0.0f;
            mMaximumDeltaTime = 1.0f / 3;
            mFixedClampedSteps = 0;
            mStart = mPrev = mCurr = Clock::now();
            mSmoothing = 0.0f;
            mFixedClampedSteps = 0;
        }

        static void Tick();

        static float DurationToSecond(const Duration& _duration) {
            return static_cast<float>(_duration.count()) * Duration::period::num / Duration::period::den;
        }
    };
}

#endif
