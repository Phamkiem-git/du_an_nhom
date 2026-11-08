// ============================================================
// File      : src/core/Timer.h
// Phụ trách : THÀNH VIÊN A (dùng chung cho cả hai khi thực nghiệm)
// Vai trò   : Đo thời gian chạy bằng <chrono>. Cả file là header, không có .cpp.
// Cách dùng : Timer t; t.start(); ...làm việc...; double ms = t.elapsedMs();
// ============================================================
#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    void start() { begin = std::chrono::steady_clock::now(); }

    // Thời gian đã trôi qua kể từ start(), tính bằng mili-giây.
    double elapsedMs() const {
        std::chrono::duration<double, std::milli> d = std::chrono::steady_clock::now() - begin;
        return d.count();
    }

private:
    std::chrono::steady_clock::time_point begin;
};

#endif
