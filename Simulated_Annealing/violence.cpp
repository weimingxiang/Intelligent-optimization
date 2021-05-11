int numSquares(int n) {
    vector<int> memo = vector<int>(n + 1, n);
    memo[0] = 0;
    memo[1] = 1;
    for (int i = 2; i <= n; ++i)
        for (int j = 1; j <= sqrt(i); ++j)
            memo[i] = min(memo[i], 1 + memo[i - j * j]);
    return memo[n];
}