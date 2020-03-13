'use strict';

function fib_slow(n) {
    if (n == 1 || n == 2) 
        return 1;
    return fib_slow(n - 1) + fib_slow(n - 2);
}

function fib_helper(arr, n) {
    if (n == 1 || n == 2)
        return 1;
    if (arr[n] > 0)
        return arr[n];
    arr[n] = fib_helper(arr, n-1) + fib_helper(arr, n-2);
    return arr[n];
}

function fib_dp(n) {
    let dp = new Array(n + 1);
    dp[1] = 1;
    dp[2] = 1;
    for (let i = 3; i <= n; i++) {
        dp[i] = dp[i-1] + dp[i-2];
    }
    return dp[n];
}

function fib_fast(n) {
    if (n == 1 || n == 2) 
        return 1;
    let n1 = 1;
    let n2 = 1;
    for (let i = 3; i <= n; i++) {
        let sum = n2 + n1;
        n2 = n1;
        n1 = sum;
    }
    return n1;
}

function test_fib() {
    let N = 40;
    let t1 = new Date();
    let n1 = fib_slow(N);
    let t2 = new Date();
    let n2 = fib_dp(N);
    let t3 = new Date();
    let n3 = fib_fast(N);
    let t4 = new Date();
    console.log('fib recursive', n1, t2 - t1);
    console.log('fib dp', n2, t3 - t2);
    console.log('fib for', n3, t4 - t3);
}

test_fib();
