
#include <bits/stdc++.h>

using namespace std;
int main()
{
    int input[4] = {0, 1, 2, 3};
    int inv[4];
    while(next_permutation(input, input + 4))
    {
        for (int i = 0; i < 4; i++)
        {
            inv[i] = 0;
            int m = 0;
            while (input[m] != i)
            {
                if (input[m] > i)
                {
                    inv[i]++;
                }
                m++;
            }
            cout << inv[i];
        }
        cout << "\t" << input[0] << input[1] << input[2] << input[3] << endl;
    }
    return 0;
}
/*
0000    0123
0010    0132
0100    0213
0200    0231
0110    0312
0210    0321
1000    1023
1010    1032
2000    1203
3000    1230
2010    1302
3010    1320
1100    2013
1200    2031
2100    2103
3100    2130
2200    2301
3200    2310
1110    3012
1210    3021
2110    3102
3110    3120
2210    3201
3210    3210
*/