//
//  main.cpp
//  Algo-Max-Number
//
//  Created by Chaojun Xue on 1/14/16.
//  Copyright © 2016 Chaojun Xue. All rights reserved.
//
/*
 https://leetcode.com/problems/create-maximum-number/
 
 Given two arrays of length m and n with digits 0-9 representing two numbers. Create the maximum number of length k <= m + n from digits of the two. The relative order of the digits from the same array must be preserved. Return an array of the k digits. You should try to optimize your time and space complexity.
 
 Example 1:
 nums1 = [3, 4, 6, 5]
 nums2 = [9, 1, 2, 5, 8, 3]
 k = 5
 return [9, 8, 6, 5, 3]
 
 Example 2:
 nums1 = [6, 7]
 nums2 = [6, 0, 4]
 k = 5
 return [6, 7, 6, 0, 4]
 
 Example 3:
 nums1 = [3, 9]
 nums2 = [8, 9]
 k = 3
 return [9, 8, 9]
 
 */

#include <iostream>
#include <vector>

using namespace std;

class list1
{
    vector<int> a;
    vector<vector<int>> f;
public:
    list1() = delete;
    inline int size() {return a.size();}
    inline int next(int x,int d) {return f[x][d];}
    list1(vector<int>& a0)
    {
        a = a0;
        f = vector<vector<int>>(a0.size() + 1,vector<int>(10,INT_MAX));
        for (int i = 0;i<a0.size();i++)
        {
            f[i][a[i]] = i;
            for (int j = i-1;j>=0;j--)
            {
                if (a[j] == a[i]) break;
                f[j][a[i]] = i;
            }
        }
    }
};

//dmd for detect_max_digit
// dmd(a,x,rem) -> (max_digit, pos) , where a[pos-1] == max_digit
// list a , from x, need rem numbers, x not included.

pair<int,int> dmd(list1& a,int x,int rem)
{
    for (int d = 9;d >= 0;d--)
    {
        int pos = a.next(x,d);
        if (pos == INT_MAX) continue;
        if (a.size() - (pos + 1) >= rem)
            return make_pair(d,pos + 1);
    }
    return make_pair(-1, -1);;
}


class Solution {
public:
    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
        list1 a1 = list1(nums1);
        int N1 = nums1.size();
        list1 a2 = list1(nums2);
        int N2 = nums2.size();
        
        auto ret = vector<int>(k,0);
        auto f = vector<int>(N1 + 1,0);
        //f[i] denote a1[0..i-1] need a2[0..f[i]-1] to reach the current maximal number, and can expand to length k
        // in other words, the state is : the number is current maximal and can be expanded, list1 begin with a1[i] and list2 with a2[f[i]]
        for (int d = 1;d <= k;d++)
        {
            int maxDigit = -1;
            auto tmpf = vector<int>(N1 + 1,INT_MAX);
            for (int x = 0;x<=N1;x++)
            {
                int y = f[x];
                if (y == INT_MAX) continue;
                auto m1 = dmd(a1,x,k-d-(N2-y));
                auto m2 = dmd(a2,y,k-d-(N1-x));
                maxDigit = max(maxDigit,m1.first);
                maxDigit = max(maxDigit,m2.first);
            }
            ret[d-1] = maxDigit;

            for (int x = 0;x<=N1;x++)
            {
                int y = f[x];
                if (y == INT_MAX) continue;
                auto m1 = dmd(a1,x,k-d-(N2-y));
                if (m1.first == maxDigit)
                    tmpf[m1.second] = min(tmpf[m1.second],y);
                auto m2 = dmd(a2,y,k-d-(N1-x));
                if (m2.first == maxDigit)
                    tmpf[x] = min(tmpf[x],m2.second);
            }
            f = tmpf;
        }
        return ret;
    }
};

class Solution2{
private:
    vector<vector<int>> f1=vector<vector<int>>(10, vector<int>());
    vector<vector<int>> f2=vector<vector<int>>(10, vector<int>());
    int len1, len2;
    vector<int> result;
    vector<int> innums1, innums2;
    
public:
    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
        len1 = (int)nums1.size();
        len2 = (int)nums2.size();
        innums1 = nums1;
        innums2 = nums2;
        
        int s0, s1;
        
        vector< pair<int, int>> candidates;
        vector< pair<int, int>> newCandidates;
        
        prepare(nums1, f1);
        prepare(nums2, f2);
        
        candidates.push_back(make_pair(0, 0));
        
        while(result.size()<k){
            
            int maxDig = -1;
            int dnum = (int)result.size();
            
            for(auto way : candidates){
                
                s0 = way.first;
                s1 = way.second;
                
                pair<int, int> p0, p1;
                p0 = getMaxDigital(f1, len1, s0, k - dnum - (len2-s1));
                p1 = getMaxDigital(f2, len2, s1, k - dnum - (len1-s0));
                
                if(p0.first > maxDig){
                    maxDig = p0.first;
                }
                if(p1.first > maxDig){
                    maxDig = p1.first;
                }
                
            }
            
            result.push_back(maxDig);
            
            for(auto way : candidates){
                s0 = way.first;
                s1 = way.second;
                
                pair<int, int> p0, p1;
                p0 = getMaxDigital(f1, len1, s0, k - dnum  - (len2-s1));
                p1 = getMaxDigital(f2, len2, s1, k - dnum  - (len1-s0));
                
                if(maxDig == p0.first){
                    auto p = make_pair(p0.second + 1, s1);
                    add2Candidates(newCandidates, p);
                    
                }
                if(p1.first == maxDig){
                    auto p = make_pair(s0, p1.second + 1);
                    add2Candidates(newCandidates, p);
                    
                }
            }
            
            candidates = newCandidates;
            //printf("newCandidates size=%d\n", newCandidates.size());
            newCandidates.clear();
        }
        
        return result;
    }
    
private:
    pair<int, int> getMaxDigital(vector<vector<int>> ft, int len, int s0, int remain){
        
        if(s0 < len){
            for(int i=9; i>=0; i--){
                vector<int> &f=ft[i];
                for(int j: f){
                    if(j >= s0 && len - j>= remain){
                        return make_pair(i, j);
                    }
                }
            }
        }
        
        return make_pair(-1, -1);
    }
    
    void add2Candidates(vector< pair<int, int>> &pool, pair<int, int> &newpair){
        for(auto l : pool){
            if(l.first == newpair.first && l.second == newpair.second){
                return;
            }
            
            if(l.second == newpair.second){
                if(l.first > newpair.first){
                    l.first = newpair.first;
                    return;
                }
            }
            
            if(l.first == newpair.first){
                if(l.second > newpair.second){
                    l.second = newpair.second;
                    return;
                }
            }
        }
        
        pool.push_back(newpair);
    }
    
    bool isDupOne(vector< pair<int, int>> &pool, pair<int, int> &newpair){
        for(auto l : pool){
            if(l.first == newpair.first && l.second == newpair.second){
                return true;
            }
        }
        return false;
    }
    
    void prepare(vector<int>& nums, vector<vector<int>> &f){
        for(int i = 0; i<nums.size(); i++){
            f[nums[i]].push_back(i);
        }
    }
    
};

int main(int argc, const char * argv[]) {
 
    
    vector<int> nums1 = {8,9,7,3,5,9,1,0,8,5,3,0,9,2,7,4,8,9,8,1,0,2,0,2,7,2,3,5,4,7,4,1,4,0,1,4,2,1,3,1,5,3,9,3,9,0,1,7,0,6,1,8,5,6,6,5,0,4,7,2,9,2,2,7,6,2,9,2,3,5,7,4,7,0,1,8,3,6,6,3,0,8,5,3,0,3,7,3,0,9,8,5,1,9,5,0,7,9,6,8,5,1,9,6,5,8,2,3,7,1,0,1,4,3,4,4,2,4,0,8,4,6,5,5,7,6,9,0,8,4,6,1,6,7,2,0,1,1,8,2,6,4,0,5,5,2,6,1,6,4,7,1,7,2,2,9,8,9,1,0,5,5,9,7,7,8,8,3,3,8,9,3,7,5,3,6,1,0,1,0,9,3,7,8,4,0,3,5,8,1,0,5,7,2,8,4,9,5,6,8,1,1,8,7,3,2,3,4,8,7,9,9,7,8,5,2,2,7,1,9,1,5,5,1,3,5,9,0,5,2,9,4,2,8,7,3,9,4,7,4,8,7,5,0,9,9,7,9,3,8,0,9,5,3,0,0,3,0,4,9,0,9,1,6,0,2,0,5,2,2,6,0,0,9,6,3,4,1,2,0,8,3,6,6,9,0,2,1,6,9,2,4,9,0,8,3,9,0,5,4,5,4,6,1,2,5,2,2,1,7,3,8,1,1,6,8,8,1,8,5,6,1,3,0,1,3,5,6,5,0,6,4,2,8,6,0,3,7,9,5,5,9,8,0,4,8,6,0,8,6,6,1,6,2,7,1,0,2,2,4,0,0,0,4,6,5,5,4,0,1,5,8,3,2,0,9,7,6,2,6,9,9,9,7,1,4,6,2,8,2,5,3,4,5,2,4,4,4,7,2,2,5,3,2,8,2,2,4,9,8,0,9,8,7,6,2,6,7,5,4,7,5,1,0,5,7,8,7,7,8,9,7,0,3,7,7,4,7,2,0,4,1,1,9,1,7,5,0,5,6,6,1,0,6,9,4,2,8,0,5,1,9,8,4,0,3,1,2,4,2,1,8,9,5,9,6,5,3,1,8,9,0,9,8,3,0,9,4,1,1,6,0,5,9,0,8,3,7,8,5};//{1,5,8,1,4,0,8,5,0,7,0,5,7,6,0,5,5,2,4,3,6,4,6,6,3,8,1,1,3,1,3,5,4,3,9,5,0,3,8,1,4,9,8,8,3,4,6,2,5,4,1,1,4,6,5,2,3,6,3,5,4,3,0,7,2,5,1,5,3,3,8,2,2,7,6,7,5,9,1,2};
    
    vector<int> nums2 = {7,8,4,1,9,4,2,6,5,2,1,2,8,9,3,9,9,5,4,4,2,9,2,0,5,9,4,2,1,7,2,5,1,2,0,0,5,3,1,1,7,2,3,3,2,8,2,0,1,4,5,1,0,0,7,7,9,6,3,8,0,1,5,8,3,2,3,6,4,2,6,3,6,7,6,6,9,5,4,3,2,7,6,3,1,8,7,5,7,8,1,6,0,7,3,0,4,4,4,9,6,3,1,0,3,7,3,6,1,0,0,2,5,7,2,9,6,6,2,6,8,1,9,7,8,8,9,5,1,1,4,2,0,1,3,6,7,8,7,0,5,6,0,1,7,9,6,4,8,6,7,0,2,3,2,7,6,0,5,0,9,0,3,3,8,5,0,9,3,8,0,1,3,1,8,1,8,1,1,7,5,7,4,1,0,0,0,8,9,5,7,8,9,2,8,3,0,3,4,9,8,1,7,2,3,8,3,5,3,1,4,7,7,5,4,9,2,6,2,6,4,0,0,2,8,3,3,0,9,1,6,8,3,1,7,0,7,1,5,8,3,2,5,1,1,0,3,1,4,6,3,6,2,8,6,7,2,9,5,9,1,6,0,5,4,8,6,6,9,4,0,5,8,7,0,8,9,7,3,9,0,1,0,6,2,7,3,3,2,3,3,6,3,0,8,0,0,5,2,1,0,7,5,0,3,2,6,0,5,4,9,6,7,1,0,4,0,9,6,8,3,1,2,5,0,1,0,6,8,6,6,8,8,2,4,5,0,0,8,0,5,6,2,2,5,6,3,7,7,8,4,8,4,8,9,1,6,8,9,9,0,4,0,5,5,4,9,6,7,7,9,0,5,0,9,2,5,2,9,8,9,7,6,8,6,9,2,9,1,6,0,2,7,4,4,5,3,4,5,5,5,0,8,1,3,8,3,0,8,5,7,6,8,7,8,9,7,0,8,4,0,7,0,9,5,8,2,0,8,7,0,3,1,8,1,7,1,6,9,7,9,7,2,6,3,0,5,3,6,0,5,9,3,9,1,1,0,0,8,1,4,3,0,4,3,7,7,7,4,6,4,0,0,5,7,3,2,8,5,1,4,5,8,5,6,7,5,7,3,3,9,6,8,1,5,1,1,1,0,3};//{7,8,5,8,0,1,1,6,1,7,6,9,6,6,0,8,5,8,6,3,4,0,4,6,7,8,7,7,7,5,7,2,5,2,1,9,5,9,3,7,3,9,9,3,1,4,3,3,9,7,1,4,4,1,4,0,2,3,1,3,2,0,2,4,0,9,2,0,1,3,9,1,2,2,6,6,9,3,6,0};
    

    int k = 500;
    
    Solution2 sol2;
    
    vector<int> v2 = sol2.maxNumber(nums1, nums2, k);
    
    for(auto i:v2){
        printf("%d ", i);
    }
    
    std::cout << "\nHello, World!\n";
    
    Solution sol;
    
    vector<int> v = sol.maxNumber(nums1, nums2, k);
    
    for(auto i:v){
        printf("%d ", i);
    }
    
    return 0;
}
