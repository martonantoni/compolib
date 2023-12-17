#include "aoc.h"

struct cPosition
{
    int row, col;

    cPosition() = default;
    constexpr cPosition(int r, int c) : row(r), col(c) {}
    constexpr cPosition(const cPosition& src) : row(src.row), col(src.col) {}
    auto operator<=>(const cPosition&) const = default;
    cPosition& operator+=(const cPosition& p)
    {
        row += p.row;
        col += p.col;
        return *this;
    }
    bool isWithinBounds(int w, int h) const
    {
        return row >= 0 && row < h&& col >= 0 && col < w;
    }
    const int& operator[](int i) const { return i == 0 ? row : col; }
    int& operator[](int i) { return i == 0 ? row : col; }
    cPosition operator*(int m) const { return cPosition(row * m, col * m); }
    cPosition operator-() const { return cPosition(-row, -col); }
};

template<> struct hash<cPosition>
{
    size_t operator()(const cPosition& p) const { return (p.row << 16) ^ p.col; };
};

constexpr cPosition operator+(const cPosition& l, const cPosition& r)
{
    return { l.row + r.row, l.col + r.col };
}

constexpr static cPosition direction_N = { -1, 0 };                     //         N
constexpr static cPosition direction_S = { 1, 0 };                      //         ^
constexpr static cPosition direction_E = { 0, 1 };                      //         |
constexpr static cPosition direction_W = { 0, -1 };                     //     W <-+-> E
constexpr static cPosition direction_NE = direction_N + direction_E;    //         |
constexpr static cPosition direction_NW = direction_N + direction_W;    //         V
constexpr static cPosition direction_SE = direction_S + direction_E;    //         S
constexpr static cPosition direction_SW = direction_S + direction_W;

constexpr static array<cPosition, 4> neighbour4Positions =  // in clockwise order
{ {
        //     ^  0          >   1       V   2        <  3
            direction_N, direction_E, direction_S, direction_W
} };

constexpr static array<cPosition, 8> neighbour8Positions = // in clockwise order
{ {
    direction_N, direction_NE, direction_E, direction_SE, direction_S, direction_SW, direction_W, direction_NW
} };


template<class DATA_TYPE>
struct cImage
{
    int w = 0, h = 0;
    vector<DATA_TYPE> cells;

    cImage() = default;
    template<class U> cImage(const cImage<U>& base, const DATA_TYPE& init_value)
    {
        cells.resize(base.cells.size(), init_value);
        w = base.w;
        h = base.h;
    }
    template<class U> cImage(const cImage<U>& base)
    {
        cells.resize(base.cells.size());
        w = base.w;
        h = base.h;
    }


    DATA_TYPE& at(const cPosition& pos)
    {
        return cells[w * pos.row + pos.col];
    }
    DATA_TYPE at(const cPosition& pos, const DATA_TYPE& default_value)
    {
        return isValidPos(pos) ? at(pos) : default_value;
    }
    DATA_TYPE& operator[](const cPosition& pos) { return at(pos); }
    cPosition bottomRight() const { return cPosition{ h - 1, w - 1 }; }

    bool isValidPos(const cPosition& pos);
    // [&](cPosition pos)
    template<class T> void forEach4Neighbour(cPosition pos, const T& f)
    {
        for (auto offset : neighbour4Positions)
        {
            cPosition neighbourPos = pos + offset;
            if (isValidPos(neighbourPos))
            {
                f(neighbourPos);
            }
        }
    }
    // [&](cPosition pos)
    template<class T, class C> void forOffsets(cPosition pos, const C& offsets, const T& f)
    {
        for (auto offset : offsets)
        {
            cPosition neighbourPos = pos + offset;
            if (isValidPos(neighbourPos))
            {
                f(neighbourPos);
            }
        }
    }
    // [&](cPosition pos)
    template<class T> void forAll(const T& f)
    {
        for (int r = 0; r < h; ++r)
        {
            for (int c = 0; c < w; ++c)
            {
                f(cPosition(r, c));
            }
        }
    }
    template<class U>
    void initSize(const cImage<U>& other)
    {
        cells.resize(other.cells.size());
        w = other.w;
        h = other.h;
    }

    template<class U> cImage& operator-=(const U& x) 
    {
        for (auto& c : cells) c -= x;
        return *this;
    }
    template<class U> cImage& operator+=(const U& x)
    {
        for (auto& c : cells) c += x;
        return *this;
    }
};

template<class DATA_TYPE>
inline bool cImage<DATA_TYPE>::isValidPos(const cPosition& pos)
{
    return pos.row >= 0 && pos.row < h&& pos.col >= 0 && pos.col < w;
}

inline cImage<char> loadImage(auto lines)
{
    cImage<char> image;
    if (lines.empty())
    {
        image.w = 0;
        image.h = 0;
    }
    else
    {
        image.w = static_cast<int>(lines[0].txt.size());
        image.h = static_cast<int>(lines.size());
    }
    int size = image.w * image.h;
    image.cells.resize(size);
    int p = 0;
    for (auto& line : lines)
    {
        memcpy(image.cells.data() + p, line.txt.c_str(), image.w);
        p += image.w;
    }
    return image;
}

void printImage(cImage<char>& img);

inline int _loop_row_helper(cPosition& pos, const pair<int, int>& p) { pos.row = p.first; return p.second; }
inline int _loop_row_helper(cPosition& pos, int from, int to) { pos.row = from; return to; }
template<class T> inline int _loop_row_helper(cPosition& pos, const cImage<T>& i) { pos.row = 0; return i.h; }
inline int _loop_col_helper(cPosition& pos, const pair<int, int>& p) { pos.col = p.first;  return p.second; }
inline int _loop_col_helper(cPosition& pos, int from, int to) { pos.col = from; return to; }
template<class T> inline int _loop_col_helper(cPosition& pos, const cImage<T>& i) { pos.col = 0;  return i.w; }

#define loop_row(p, ...) for(int row_end = _loop_row_helper(p, __VA_ARGS__);p.row<row_end;++p.row)
#define loop_col(p, ...) for(int col_end = _loop_col_helper(p, __VA_ARGS__);p.col<col_end;++p.col)
