#pragma once

#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <ranges>
#include <algorithm>
#include <numeric>

using cIntVector = std::vector<ll>;

// Can store either:
// - std::string, the name of the type is then cStringVector
// - std::string_view, the name of the type is then cStringViewVector

// Can be constructed from:
// - std::string
// - std::string_view
// - const char*
// - from a container of strings (e.g. std::vector<std::string>)

template<class T> concept cStringVectorSupportedStoredTypes =
std::same_as<T, std::string> ||
std::same_as<T, std::string_view>;

template<class T> concept cStringVectorSupportedSource =
std::same_as<std::decay_t<T>, std::string> ||
std::same_as<std::decay_t<T>, std::string_view> ||
std::is_convertible_v<T, const char*>;

template<class T> concept cStringVectorDelimeter =
    requires(const std::string_view sv, const T & t)
{
    { sv.find_first_of(t) } -> std::convertible_to<std::size_t>;
};

template<class T> concept cStringContainer = requires(T t)
{
    t.begin();
    t.end();
    *t.begin();
    std::string(*t.begin());
};

template<cStringVectorSupportedStoredTypes StoredType>
class tStringVector : public std::vector<StoredType>
{
    using base = std::vector<StoredType>;
    void addFields(
        const cStringVectorSupportedSource auto& source,
        const cStringVectorDelimeter auto& delimeters,
        bool emptyFieldsAllowed);
    static void trim(StoredType& s);
public:
    tStringVector() {}
    tStringVector(const cStringContainer auto& source);
    tStringVector(
        const cStringVectorSupportedSource auto& source,
        const cStringVectorDelimeter auto& delimeters,
        bool emptyFieldsAllowed = true);
    void fromString(
        const cStringVectorSupportedSource auto& source,
        const cStringVectorDelimeter auto& delimeters,
        bool emptyFieldsAllowed = true);
    void trimAll();
    std::string toString(const std::string& separator) const;
    void fromIntVector(const cIntVector& intVector) requires std::same_as<StoredType, std::string>;
    cIntVector toIntVector() const;
};

using cStringVector = tStringVector<std::string>;
using cStringViewVector = tStringVector<std::string_view>;

template<cStringVectorSupportedStoredTypes StoredType>
void tStringVector<StoredType>::addFields(
    const cStringVectorSupportedSource auto& source,
    const cStringVectorDelimeter auto& delimeters,
    bool emptyFieldsAllowed)
{
    std::string_view sourceView(source);
    std::string::size_type start = 0;
    std::string::size_type end = sourceView.find_first_of(delimeters);
    while (end != std::string::npos)
    {
        if (emptyFieldsAllowed || end > start)
            this->emplace_back(sourceView.substr(start, end - start));
        start = end + 1;
        end = sourceView.find_first_of(delimeters, start);
    }
    if (emptyFieldsAllowed || start < sourceView.size())
        this->emplace_back(sourceView.substr(start));
}

template<cStringVectorSupportedStoredTypes StoredType>
tStringVector<StoredType>::tStringVector(
    const cStringVectorSupportedSource auto& source,
    const cStringVectorDelimeter auto& delimeters,
    bool emptyFieldsAllowed)
{
    this->reserve(4);
    addFields(source, delimeters, emptyFieldsAllowed);
}

template<cStringVectorSupportedStoredTypes StoredType>
void tStringVector<StoredType>::fromString(
    const cStringVectorSupportedSource auto& source,
    const cStringVectorDelimeter auto& delimeters,
    bool emptyFieldsAllowed)
{
    this->clear(); this->reserve(4);
    addFields(source, delimeters, emptyFieldsAllowed);
}

template<cStringVectorSupportedStoredTypes StoredType>
std::string tStringVector<StoredType>::toString(const std::string& separator) const
{
    return std::accumulate(
        this->begin(),
        this->end(),
        std::string(),
        [&separator](const std::string& a, const StoredType& b)
        {
            if constexpr (std::same_as<StoredType, std::string>)
                return a.empty() ? b : a + separator + b;
            else
            {
                if (a.empty())
                    return std::string(b);
                std::string result = a + separator;
                result.reserve(result.size() + b.size());
                result.insert(result.end(), b.begin(), b.end());
                return result;
            }
        });
}

template<cStringVectorSupportedStoredTypes StoredType>
void tStringVector<StoredType>::fromIntVector(const cIntVector& intVector) requires std::same_as<StoredType, std::string>
{
    this->resize(intVector.size());
    for (auto&& [idx, value] : std::views::enumerate(intVector))
        (*this)[idx] = std::to_string(value);
}

template<cStringVectorSupportedStoredTypes StoredType>
cIntVector tStringVector<StoredType>::toIntVector() const
{
    cIntVector intVector;
    intVector.resize(this->size());
    for (auto&& [idx, s] : std::views::enumerate(*this))
    {
        if constexpr (std::is_same_v<StoredType, std::string>)
        {
            intVector[idx] = std::stoi(s);
        }
        else
        {
            intVector[idx] = 0;
            std::string_view sv(s);
            trim(sv);
            if (sv.empty())
                continue;
            int isNegative = sv[0] == '-';
            if (isNegative)
                sv.remove_prefix(1);
            if (sv.empty())
                continue;
            if (sv[0] < '0' || sv[0] > '9')
            {
#ifndef STRING_VECTOR_NO_EXCEPTIONS
                throw std::runtime_error("Invalid number format");
#else
                continue;
#endif
            }
            for (auto c : sv)
            {
                if (c < '0' || c > '9')
                {
                    break;
                }
                intVector[idx] = intVector[idx] * 10 + c - '0';
            }
            if (isNegative)
                intVector[idx] = -intVector[idx];
        }
    }
    return intVector;
}

template<cStringVectorSupportedStoredTypes StoredType>
void tStringVector<StoredType>::trim(StoredType& s)
{
    auto left = std::find_if_not(s.begin(), s.end(), [](int c) { return std::isspace(c); });
    auto right = std::find_if_not(s.rbegin(), s.rend(), [](int c) { return std::isspace(c); });
    s = s.substr(left - s.begin(), right.base() - left);
}

template<cStringVectorSupportedStoredTypes StoredType>
void tStringVector<StoredType>::trimAll()
{
    for (auto& s : *this)
        trim(s);
}

template<cStringVectorSupportedStoredTypes StoredType>
template<cStringContainer T> tStringVector<StoredType>::tStringVector(const T& source)
{
    for (auto& s : source)
        this->emplace_back(s);
}