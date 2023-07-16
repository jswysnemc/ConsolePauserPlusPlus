const { log } = require('console')
const fs = require('fs')
const data = fs.readFileSync('/dev/stdin')
// const data = fs.readFileSync('./input.txt')

const nums = data.toString('ascii').trim().split(/\s+/).map(x => parseInt(x))
var [n,m] = nums.slice(0,2)

var arr = nums.slice(2,n+2)
var trr = nums.slice(n+2)

var str = ''
for(var t of trr){
    var [l,r,mid] = [0,n]
    while(l < r){
        mid = (l + r) >> 1;
        if(arr[mid] < t) l = mid + 1;
        else r = mid;
    }

    var res = r >= 0 && r < n && arr[r] == t ? r + 1 : -1;
    str = str + res + ' ';
}

console.log(str);
process.exit() // 请注意必须在出口点处加入此行
