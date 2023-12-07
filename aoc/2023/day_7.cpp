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
        {
            cards[i] = card_order.find(hand[i]);
        }
        calcRank(0);
        if(!isFirstPart)
        {
            auto bestRank = rank;
            for (int i = 0; i < 14; ++i)
            {
                calcRank(i);
                bestRank = max(rank, bestRank);
            }
            rank = bestRank;
        }
    }
    void calcRank(int jokerIndex)
    {
        fill(ALL(rank), 0);
        for (auto c : cards)
            ++rank[!isFirstPart && c == 0 ? jokerIndex : c];
        sort(ALL(rank), greater<>());
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