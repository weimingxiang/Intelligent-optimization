
#include <bits/stdc++.h>

using namespace std;
int main()
{
    vector<int> city1(7);
    vector<int> pos1(7);
    int inv[7] = {2, 5, 2, 0, 2, 0, 0};
    vector<int> inv1(inv, inv + 7);

    for (int i = inv1.size() - 1; i >= 0; i--)
    {
        for (int m = i + 1; m < pos1.size(); m++)
        {
            if (pos1[m] >= inv1[i])
                pos1[m]++;
        }
        pos1[i] = inv1[i];
    }
    for (int i = 0; i < city1.size(); i++)
        city1[pos1[i]] = i;

    for (auto &it : city1)
        cout << it << ", ";

    return 0;
}
