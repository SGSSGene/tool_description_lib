#pragma once

#include <numeric>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <variant>
#include <vector>


template <typename T>
struct is_vector : std::false_type {};

template <typename T>
struct is_vector<std::vector<T>> : std::true_type {};

template <typename T>
inline static constexpr bool is_vector_v = is_vector<T>::value;

/*!\brief Stores a value with optional limits
 */
template <typename T, typename ListType=T>
struct TValue {
    ListType value{};
    std::optional<T> minLimit{};
    std::optional<T> maxLimit{};

    enum class State {Ok, LimitsInvalid, ValueToLow, ValueToHigh};

    auto state() const {
        if (minLimit and maxLimit and *minLimit > *maxLimit) {
            return State::LimitsInvalid;
        }
        if constexpr (is_vector_v<T>) {
            for (auto e : value) {
                if (minLimit and *minLimit > e) {
                    return State::ValueToLow;
                }
                if (maxLimit and *maxLimit < e) {
                    return State::ValueToHigh;
                }
            }
        } else {
            if (minLimit and *minLimit > value) {
                return State::ValueToLow;
            }
            if (maxLimit and *maxLimit < value) {
                return State::ValueToHigh;
            }
        }
        return State::Ok;
    }
};

/*!\brief Stores a string with optional list of valid strings.
 */
template <typename T, typename ListType=T>
struct TStringValue {
    ListType value;
    std::optional<std::vector<T>> validValues{};

    enum class State {Valid, Invalid};
    auto state() const {
        if (validValues) {
            auto checkSingleValue = [this](T const& value) {
                return std::accumulate(begin(*validValues), end(*validValues), false, [&](auto acc, auto const& pattern) {
                    if (value == pattern) {
                        return true;
                    }
                    return acc;
                });
            };

            if constexpr (is_vector_v<T>) {
                for (auto const& e : value) {
                    auto isValid = checkSingleValue(e);
                    if (not isValid) {
                        return State::Invalid;;
                    }
                }
            } else {
                auto isValid = checkSingleValue(value);
                if (not isValid) {
                    return State::Invalid;;
                }
            }
        }
        return State::Valid;
    }
};

using IntValue        = TValue<int>;
using DoubleValue     = TValue<double>;
using StringValue     = TStringValue<std::string>;
using IntValueList    = TValue<int, std::vector<int>>;
using DoubleValueList = TValue<double, std::vector<double>>;
using StringValueList = TStringValue<std::string, std::vector<std::string>>;

/*!\brief
 */
struct ParamTree {
    using Children = std::vector<ParamTree>;
    using Value = std::variant<IntValue,           // single int, double or string value
                               DoubleValue,
                               StringValue,
                               IntValueList,       // list of int, double or string values
                               DoubleValueList,
                               StringValueList,
                               Children>;          // not a value, but a node with children

    std::string name{};           //!< Name of the entry.
    std::string description{};    //!< Entry description.
    std::set<std::string> tags{}; //!< List of tags, e.g.: advanced parameter tag.
    Value value;                  //!< Current value of this entry
};

struct CLIMapping {
    std::string optionIdentifier;
    std::string referenceName;
};
struct ParamDocument {
    ParamTree::Children root;
    std::vector<CLIMapping> cliMapping{};
};
