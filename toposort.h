// Sorting based on partial ordering (https://en.wikipedia.org/wiki/Partially_ordered_set),
// adapted from: https://stackoverflow.com/a/45306500

#include <algorithm>

namespace
{
    template <typename Iterator, typename Compare, typename Callback>
    void toposort_implementation(Iterator begin, Iterator end, Compare compare, Callback callback)
    {
        auto const predicate = [&](auto const& a) {
            return std::none_of(begin, end,
                [&](auto const& b) {
                    return compare(b, a);
                }
            );
        };

        while (begin != end) {
            auto const new_begin = callback(begin, end, predicate);
            if (new_begin == begin) {
                throw std::logic_error("toposort: not a partial ordering");
            }
            begin = new_begin;
        }
    }
}

template <typename Iterator, typename Compare>
void toposort(Iterator begin, Iterator end, Compare compare)
{
    auto const callback = [](Iterator begin, Iterator end, auto predicate) {
        return std::partition(begin, end, predicate);
    };
    toposort_implementation(begin, end, compare, callback);
}

template <typename Iterator, typename Compare>
void stable_toposort(Iterator begin, Iterator end, Compare compare)
{
    auto const callback = [](Iterator begin, Iterator end, auto predicate) {
        return std::stable_partition(begin, end, predicate);
    };
    toposort_implementation(begin, end, compare, callback);
}