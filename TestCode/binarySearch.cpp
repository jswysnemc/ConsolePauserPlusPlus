/*
 * @LastEditTime: 2023-06-17 19:46:59
 * @题目链接:  https://www.luogu.com.cn/problem/P2249
 */

#include<bits/stdc++.h>

using namespace std;
int n,m;

int main() 
{
    cin >> n >> m;
    vector<int> arr(n);
    for(int i = 0; i < n; i++) cin >> arr[i];

    int l,r,mid,q;

    while(m--){
        l = 0,r = n; cin >> q;
        while(l  < r){
            mid = (r + l) / 2;
            if(arr[mid] < q) l = mid + 1;
            else r = mid;
        }

        int res = r >= 0 && r < n && arr[r] == q ? r + 1  : -1;
        cout << res << " ";

    }
    
    return 0;
}