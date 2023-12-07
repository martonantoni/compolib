#include "aoc.h"
#include "bmp.h"

const char* main_delimeters = " ,";

bool isFirstPart;

string card_order;

struct cHand
{
    ll rank = 0;
    // high card: 0
    // one pair: 1
    // two pairs: 2
    // three: 3
    // full: 4
    // four: 5
    // five: 6
    ll bet;
    array<ll,5> cards;
    cHand(ll bet, string& hand): bet(bet)
    {
        FOR(i, 5)
        {
            cards[i] = card_order.find(hand[i]);
        }
        if (isFirstPart)
        {
            rank = calcRank(hand, 0);
        }
        else
        {
            rank = 0;
            for (int i = 0; i < 14; ++i)
            {
                rank = max(calcRank(hand, i), rank);
            }
        }
    }
    ll calcRank(string& hand, int joker_value)
    {
        unordered_map<ll, ll> card_counts;
        for (int i = 0; i < 5; ++i)
        {
            ++card_counts[(!isFirstPart && hand[i] == 'J') ? joker_value : cards[i]];
        }
        vector<ll> len_counts(6, 0);
        for (auto& [k, count] : card_counts)
            ++len_counts[count];

        if (len_counts[5] == 1)
            return 6;
        if (len_counts[4] == 1)
            return 5;
        if (len_counts[3] == 1 && len_counts[2] == 1)
            return 4;
        if (len_counts[3] == 1)
            return 3;
        if (len_counts[2] == 2)
            return 2;
        if (len_counts[2] == 1)
            return 1;
        return 0;
    }
    bool operator<(const cHand& other) const
    {
        return make_tuple(rank, cards) < make_tuple(other.rank, other.cards);
//        return make_tuple(rank, cards[0], cards[1], cards[2], cards[3], cards[4]) < make_tuple(other.rank, other.cards[0], other.cards[1], other.cards[2], other.cards[3], other.cards[4]);
    }
};

void solveFirst()
{
    card_order = "23456789TJQKA"s;

    ll res = 0;
    vector<cHand> hs; 
    for (auto& l : ls)
    {
        hs.emplace_back(l.i[1], l.s[0]);
    }
    sort(ALL(hs));
    ll i = 1;
    for (auto& h : hs)
    {
        res += i * h.bet;
        ++i;
    }

    P("----> Result: %lld\n", res);
}


void solveSecond()
{
    card_order = "J23456789TQKA"s;

    ll res = 0;
    vector<cHand> hs;
    for (auto& l : ls)
    {
        hs.emplace_back(l.i[1], l.s[0]);
    }
    sort(ALL(hs));
    ll i = 1;
    for (auto& h : hs)
    {
        res += i * h.bet;
        ++i;
    }

    P("----> Result: %lld\n", res);
}

void solve(bool first) { isFirstPart = first; first ? solveFirst() : solveSecond(); }
