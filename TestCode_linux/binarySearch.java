/*
 * @LastEditTime: 2023-06-17 19:46:59
 * @题目链接:  https://www.luogu.com.cn/problem/P2249
 */

import java.io.*;


//https://www.luogu.com.cn/problem/P2249

public class binarySearch {
    static StreamTokenizer st = new StreamTokenizer(new BufferedReader(new InputStreamReader(System.in)));
    static int nextInt() throws IOException {
        st.nextToken();
        return (int) st.nval;
    }
    static long nextLong() throws IOException{
        st.nextToken();
        return (long) st.nval;
    }
    static PrintWriter pw = new PrintWriter(new OutputStreamWriter(System.out));
    static int[] arr;
    public static void main(String[] args) throws IOException {
        int n,m;
        n = nextInt();
        m = nextInt();
        arr = new int[n];
        for(int i = 0; i < n; i++){
            arr[i] = nextInt();
        }

        for(int i = 0; i < m; i++){
            int q = nextInt();
            pw.print(search(arr,q) + " ");
            pw.flush();
        }
    }

    public static int search(int[] nums, int target) {
        int l = -1; int r = nums.length - 1;
        while(l + 1 != r){
            int m = (l + r) / 2;
            if(nums[m] < target){
                l = m;
            }else {
                r = m;
            }
        }
        return r >= 0 && r < nums.length &&  nums[r] == target ? r + 1 : -1;
    }
}
