n,m = map(int,input().split())
arr = list(map(int,input().split()))
trr = list(map(int,input().split()))
for t in trr:
    l,r,mid = 0,n,0
    while l < r:
        mid =( r + l) >> 1
        if arr[mid] < t : l = mid + 1
        else : r = mid 

    res = [-1,r+1][r >= 0 and r < n and arr[r] == t]
    print(res,end=' ')