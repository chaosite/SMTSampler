#ifndef MEGASAMPLER_INTERVAL_H
#define MEGASAMPLER_INTERVAL_H


#include <cstdint>
#include <string>
#include <iostream>

class Interval {
    int64_t low;
    int64_t high;
public:
    Interval() : low(INT64_MIN), high(INT64_MAX) {};
    Interval(int64_t l, int64_t h) : low(l), high(h) {};
    void set_upper_bound(int64_t u_bound);
    void set_lower_bound(int64_t l_bound);
    [[nodiscard]] int64_t get_low() const {return low;};
    [[nodiscard]] int64_t get_high() const {return high;};
    [[nodiscard]] bool is_high_inf() const;
    [[nodiscard]] bool is_low_minf() const;
    [[nodiscard]] bool is_bottom() const;
    [[nodiscard]] bool is_top() const;
    [[nodiscard]] bool is_infinite() const;
    friend std::ostream& operator<<(std::ostream& os, const Interval& interval);
    [[nodiscard]] bool is_in_range(int64_t value) const;
    [[nodiscard]] int64_t random_in_range() const;
};


#endif //MEGASAMPLER_INTERVAL_H
