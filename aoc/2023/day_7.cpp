#include "aoc.h"
#include "bmp.h"

const char* main_delimeters = " ,";

bool isFirstPart;

string card_order;

struct cHand
{
    ll bet;
    array<ll, 5> cards;
    array<ll, 14> rank;
    cHand(ll bet, const string& hand): bet(bet)
    {
        FOR(i, 5)
            cards[i] = card_order.find(hand[i]);
        fill(ALL(rank), 0);
        for (auto c : cards)
            ++rank[c];
        auto jokers = rank[0];
        if (!isFirstPart)
            rank[0] = 0;
        sort(ALL(rank), greater<>());
        if (!isFirstPart)
            rank[0] += jokers;
    }
    bool operator<(const cHand& other) const
    {
        return make_tuple(rank, cards) < make_tuple(other.rank, other.cards);
    }
};

void solve(bool first)
{
    isFirstPart = first;
    if(first)
        card_order = "23456789TJQKA"s;
    else
        card_order = "J23456789TQKA"s;

    vector<cHand> hs;
    for (auto& l : ls)
    {
        hs.emplace_back(l.i[1], l.s[0]);
    }
    sort(ALL(hs));
    ll i = 1;
    ll res = 0;
    for (auto& h : hs)
    {
        res += i * h.bet;
        ++i;
    }

    P("----> Result: %lld\n", res);
}

/*
void solveFirst()
{
}

void solveSecond()
{
}

void solve(bool first) { isFirstPart = first; first ? solveFirst() : solveSecond(); }
*/