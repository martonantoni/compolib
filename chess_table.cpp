class cChessTableCalc
{
public:
    cChessTableCalc(int rows, int coloumns) :
        rows(rows), coloumns(coloumns)
    {
        white_in_odd_row = (coloumns + 1) / 2;
        white_in_even_row = coloumns / 2;
        black_in_odd_row = coloumns - white_in_odd_row;
        black_in_even_row = coloumns - white_in_even_row;
        even_rows = rows / 2;
        odd_rows = rows - even_rows;
    }
    int whiteCount() const
    {
        return white_in_odd_row * odd_rows + white_in_even_row * even_rows;
    }
    int blackCount() const
    {
        return black_in_odd_row * odd_rows + black_in_even_row * even_rows;
    }

    pair<int, int> nth_black(int n)  //  n is zero based,  returns:  row, col   
    {
        int nth_in_two_rows = n % coloumns;
        if (nth_in_two_rows < black_in_odd_row)
        {
            return { 2 * (n / coloumns), nth_in_two_rows * 2 + 1 };
        }
        return { 2 * (n / coloumns) + 1, (nth_in_two_rows - black_in_odd_row) * 2 };
    }

    pair<int, int> nth_white(int n) // n is zero based,   returns: row, col
    {
        int nth_in_two_rows = n % coloumns;
        if (nth_in_two_rows < white_in_odd_row)
        {
            return { 2 * (n / coloumns), nth_in_two_rows * 2 };
        }
        return { 2 * (n / coloumns) + 1, (nth_in_two_rows - white_in_odd_row) * 2 + 1 };
    }
private:
    int rows, coloumns;
// carefull: naming is messed up, first row is odd, but that's row indoex 0  (same with coloumns)
    int white_in_odd_row;
    int white_in_even_row;
    int black_in_odd_row;
    int black_in_even_row;
    int even_rows;
    int odd_rows;
};


void test()
{
    RI(n);
    cChessTableCalc table(n, n);
    int remainingWhites = table.whiteCount();
    int remainingBlacks = table.blackCount();

    printf("white count: %d, black count: %d\n", remainingWhites, remainingBlacks);
    for (int i = 0; i < remainingWhites; ++i)
    {
        printf("white #%d: %d, %d\n", i, table.nth_white(i).first, table.nth_white(i).second);
    }
    for (int i = 0; i < remainingWhites; ++i)
    {
        printf("black #%d: %d, %d\n", i, table.nth_black(i).first, table.nth_black(i).second);
    }
}