#include <iostream>
#include <bitset>
#include <map>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <iomanip>

#define NIBBLE_SIZE_in_BITS 4
#define BYTE_SIZE_in_BITS 8
#define BYTE_SIZE_in_NIBBLE BYTE_SIZE_in_BITS / NIBBLE_SIZE_in_BITS
#define WORD_SIZE_in_BITS 32
#define WORD_SIZE_in_NIBBLE WORD_SIZE_in_BITS / NIBBLE_SIZE_in_BITS
#define WORD_SIZE_in_BYTES WORD_SIZE_in_BITS / BYTE_SIZE_in_BITS
#define BLOCK_SIZE_in_BITS 128
#define BLOCK_SIZE_in_NIBBLE BLOCK_SIZE_in_BITS / NIBBLE_SIZE_in_BITS
#define BLOCK_SIZE_in_WORDS BLOCK_SIZE_in_BITS / WORD_SIZE_in_BITS

using namespace std;

#define NIB bitset<NIBBLE_SIZE_in_BITS>

map<char, NIB> HEX_to_NIBBLE({{'0', NIB(0)},
                              {'1', NIB(1)},
                              {'2', NIB(2)},
                              {'3', NIB(3)},
                              {'4', NIB(4)},
                              {'5', NIB(5)},
                              {'6', NIB(6)},
                              {'7', NIB(7)},
                              {'8', NIB(8)},
                              {'9', NIB(9)},
                              {'A', NIB(10)},
                              {'B', NIB(11)},
                              {'C', NIB(12)},
                              {'D', NIB(13)},
                              {'E', NIB(14)},
                              {'F', NIB(15)}});
struct BYTE
{
    vector<NIB> N;
    BYTE(string b)
    {
        for (unsigned i = 0; i < BYTE_SIZE_in_NIBBLE; i++)
            N.push_back(HEX_to_NIBBLE[(char)(isalpha(b[i]) ? toupper(b[i]) : b[i])]);
    }
    BYTE()
    {
        string zero;
        for (unsigned i = 0; i < BYTE_SIZE_in_NIBBLE; i++)
            zero += "0";
        this->N = BYTE(zero).N;
    }
    BYTE(vector<NIB> N) { this->N = N; }
    BYTE(unsigned long num)
    {
        bitset<BYTE_SIZE_in_BITS> B(num);
        N.push_back(NIB((B & bitset<BYTE_SIZE_in_BITS>(15)).to_ulong()));
        B = bitset<BYTE_SIZE_in_BITS>(num >> NIBBLE_SIZE_in_BITS);
        N.push_back(NIB(B.to_ulong()));
        reverse(N.begin(), N.end());
    }
    BYTE byte_xor(BYTE B)
    {
        BYTE ans;
        for (unsigned i = 0; i < BYTE_SIZE_in_NIBBLE; i++)
            ans.N[i] = this->N[i] ^ B.N[i];
        return ans;
    }
    void print()
    {
        for (auto n : N)
            cout << hex << uppercase << n.to_ullong();
    }
    unsigned char get_value()
    {
        unsigned char ans = N[0].to_ulong();
        ans <<= NIBBLE_SIZE_in_BITS;
        ans += N[1].to_ulong();
        return ans;
    }
};

map<unsigned, map<unsigned, BYTE>> AES_S_BOX({
    {0, {
            {0, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['3']})},
            {1, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['C']})},
            {2, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['7']})},
            {3, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['B']})},
            {4, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['2']})},
            {5, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['B']})},
            {6, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['F']})},
            {7, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['5']})},
            {8, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['0']})},
            {9, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['1']})},
            {10, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['7']})},
            {11, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['B']})},
            {12, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['E']})},
            {13, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['7']})},
            {14, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['B']})},
            {15, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['6']})},
        }},
    {1, {
            {0, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['A']})},
            {1, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['2']})},
            {2, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['9']})},
            {3, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['D']})},
            {4, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['A']})},
            {5, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['9']})},
            {6, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['7']})},
            {7, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['0']})},
            {8, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['D']})},
            {9, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['4']})},
            {10, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['2']})},
            {11, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['F']})},
            {12, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['C']})},
            {13, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['4']})},
            {14, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['2']})},
            {15, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['0']})},
        }},
    {2, {
            {0, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['7']})},
            {1, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['D']})},
            {2, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['3']})},
            {3, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['6']})},
            {4, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['6']})},
            {5, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['F']})},
            {6, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['7']})},
            {7, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['C']})},
            {8, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['4']})},
            {9, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['5']})},
            {10, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['5']})},
            {11, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['1']})},
            {12, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['1']})},
            {13, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['8']})},
            {14, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['1']})},
            {15, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['5']})},
        }},
    {3, {
            {0, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['4']})},
            {1, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['7']})},
            {2, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['3']})},
            {3, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['3']})},
            {4, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['8']})},
            {5, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['6']})},
            {6, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['5']})},
            {7, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['A']})},
            {8, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['7']})},
            {9, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['2']})},
            {10, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['0']})},
            {11, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['2']})},
            {12, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['B']})},
            {13, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['7']})},
            {14, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['2']})},
            {15, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['5']})},
        }},
    {4, {
            {0, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['9']})},
            {1, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['3']})},
            {2, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['C']})},
            {3, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['A']})},
            {4, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['B']})},
            {5, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['E']})},
            {6, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['A']})},
            {7, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['0']})},
            {8, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['2']})},
            {9, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['B']})},
            {10, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['6']})},
            {11, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['3']})},
            {12, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['9']})},
            {13, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['3']})},
            {14, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['F']})},
            {15, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['4']})},
        }},
    {5, {
            {0, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['3']})},
            {1, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['1']})},
            {2, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['0']})},
            {3, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['D']})},
            {4, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['0']})},
            {5, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['C']})},
            {6, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['1']})},
            {7, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['B']})},
            {8, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['A']})},
            {9, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['B']})},
            {10, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['E']})},
            {11, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['9']})},
            {12, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['A']})},
            {13, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['C']})},
            {14, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['8']})},
            {15, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['F']})},
        }},
    {6, {
            {0, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['0']})},
            {1, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['F']})},
            {2, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['A']})},
            {3, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['B']})},
            {4, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['3']})},
            {5, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['D']})},
            {6, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['3']})},
            {7, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['5']})},
            {8, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['5']})},
            {9, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['9']})},
            {10, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['2']})},
            {11, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['F']})},
            {12, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['0']})},
            {13, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['C']})},
            {14, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['F']})},
            {15, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['8']})},
        }},
    {7, {
            {0, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['1']})},
            {1, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['3']})},
            {2, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['0']})},
            {3, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['F']})},
            {4, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['2']})},
            {5, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['D']})},
            {6, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['8']})},
            {7, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['5']})},
            {8, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['C']})},
            {9, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['6']})},
            {10, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['A']})},
            {11, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['1']})},
            {12, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['0']})},
            {13, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['F']})},
            {14, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['3']})},
            {15, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['2']})},
        }},
    {8, {
            {0, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['D']})},
            {1, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['C']})},
            {2, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['3']})},
            {3, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['C']})},
            {4, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['F']})},
            {5, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['7']})},
            {6, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['4']})},
            {7, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['7']})},
            {8, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['4']})},
            {9, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['7']})},
            {10, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['E']})},
            {11, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['D']})},
            {12, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['4']})},
            {13, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['D']})},
            {14, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['9']})},
            {15, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['3']})},
        }},
    {9, {
            {0, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['0']})},
            {1, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['1']})},
            {2, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['F']})},
            {3, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['C']})},
            {4, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['2']})},
            {5, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['A']})},
            {6, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['0']})},
            {7, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['8']})},
            {8, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['6']})},
            {9, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['E']})},
            {10, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['8']})},
            {11, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['4']})},
            {12, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['E']})},
            {13, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['E']})},
            {14, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['B']})},
            {15, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['B']})},
        }},
    {10, {
             {0, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['0']})},
             {1, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['2']})},
             {2, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['A']})},
             {3, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['A']})},
             {4, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['9']})},
             {5, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['6']})},
             {6, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['4']})},
             {7, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['C']})},
             {8, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['2']})},
             {9, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['3']})},
             {10, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['C']})},
             {11, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['2']})},
             {12, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['1']})},
             {13, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['5']})},
             {14, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['4']})},
             {15, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['9']})},
         }},
    {11, {
             {0, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['7']})},
             {1, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['8']})},
             {2, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['7']})},
             {3, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['D']})},
             {4, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['D']})},
             {5, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['5']})},
             {6, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['E']})},
             {7, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['9']})},
             {8, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['C']})},
             {9, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['6']})},
             {10, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['4']})},
             {11, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['A']})},
             {12, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['5']})},
             {13, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['A']})},
             {14, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['E']})},
             {15, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['8']})},
         }},
    {12, {
             {0, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['A']})},
             {1, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['8']})},
             {2, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['5']})},
             {3, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['E']})},
             {4, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['C']})},
             {5, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['6']})},
             {6, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['4']})},
             {7, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['6']})},
             {8, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['8']})},
             {9, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['D']})},
             {10, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['4']})},
             {11, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['F']})},
             {12, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['B']})},
             {13, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['D']})},
             {14, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['B']})},
             {15, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['A']})},
         }},
    {13, {
             {0, BYTE(vector<NIB>{HEX_to_NIBBLE['7'], HEX_to_NIBBLE['0']})},
             {1, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['E']})},
             {2, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['5']})},
             {3, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['6']})},
             {4, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['8']})},
             {5, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['3']})},
             {6, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['6']})},
             {7, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['E']})},
             {8, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['1']})},
             {9, BYTE(vector<NIB>{HEX_to_NIBBLE['3'], HEX_to_NIBBLE['5']})},
             {10, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['7']})},
             {11, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['9']})},
             {12, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['6']})},
             {13, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['1']})},
             {14, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['D']})},
             {15, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['E']})},
         }},
    {14, {
             {0, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['1']})},
             {1, BYTE(vector<NIB>{HEX_to_NIBBLE['F'], HEX_to_NIBBLE['8']})},
             {2, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['8']})},
             {3, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['1']})},
             {4, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['9']})},
             {5, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['9']})},
             {6, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['E']})},
             {7, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['4']})},
             {8, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['B']})},
             {9, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['E']})},
             {10, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['7']})},
             {11, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['9']})},
             {12, BYTE(vector<NIB>{HEX_to_NIBBLE['C'], HEX_to_NIBBLE['E']})},
             {13, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['5']})},
             {14, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['8']})},
             {15, BYTE(vector<NIB>{HEX_to_NIBBLE['D'], HEX_to_NIBBLE['F']})},
         }},
    {15, {
             {0, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['C']})},
             {1, BYTE(vector<NIB>{HEX_to_NIBBLE['A'], HEX_to_NIBBLE['1']})},
             {2, BYTE(vector<NIB>{HEX_to_NIBBLE['8'], HEX_to_NIBBLE['9']})},
             {3, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['D']})},
             {4, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['F']})},
             {5, BYTE(vector<NIB>{HEX_to_NIBBLE['E'], HEX_to_NIBBLE['6']})},
             {6, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['2']})},
             {7, BYTE(vector<NIB>{HEX_to_NIBBLE['6'], HEX_to_NIBBLE['8']})},
             {8, BYTE(vector<NIB>{HEX_to_NIBBLE['4'], HEX_to_NIBBLE['1']})},
             {9, BYTE(vector<NIB>{HEX_to_NIBBLE['9'], HEX_to_NIBBLE['9']})},
             {10, BYTE(vector<NIB>{HEX_to_NIBBLE['2'], HEX_to_NIBBLE['D']})},
             {11, BYTE(vector<NIB>{HEX_to_NIBBLE['0'], HEX_to_NIBBLE['F']})},
             {12, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['0']})},
             {13, BYTE(vector<NIB>{HEX_to_NIBBLE['5'], HEX_to_NIBBLE['4']})},
             {14, BYTE(vector<NIB>{HEX_to_NIBBLE['B'], HEX_to_NIBBLE['B']})},
             {15, BYTE(vector<NIB>{HEX_to_NIBBLE['1'], HEX_to_NIBBLE['6']})},
         }},
});

struct WORD
{
    vector<BYTE> B;
    WORD(string word)
    {
        for (unsigned i = 0; i < WORD_SIZE_in_BYTES; i++)
            B.push_back(BYTE(word.substr(i * BYTE_SIZE_in_NIBBLE, BYTE_SIZE_in_NIBBLE)));
    }
    WORD()
    {
        string zero;
        for (unsigned i = 0; i < WORD_SIZE_in_NIBBLE; i++)
            zero += "0";
        this->B = WORD(zero).B;
    }
    WORD(vector<BYTE> B) { this->B = B; }
    WORD word_xor(WORD W)
    {
        WORD ans;
        for (unsigned i = 0; i < WORD_SIZE_in_BYTES; i++)
            ans.B[i] = this->B[i].byte_xor(W.B[i]);
        return ans;
    }
    void print()
    {
        cout << "[";
        for (unsigned i = 0; i < WORD_SIZE_in_BYTES; i++)
        {
            B[i].print();
            if (i + 1 != WORD_SIZE_in_BYTES)
                cout << " ";
        }
        cout << "]";
    }
};

WORD left_rotate(WORD word, unsigned num_rotations = 1)
{
    rotate(word.B.begin(), word.B.begin() + num_rotations, word.B.end());
    return word;
}

WORD substitute(WORD word)
{
    for (auto &b : word.B)
        b = AES_S_BOX[b.N[0].to_ulong()][b.N[1].to_ulong()];
    return word;
}

struct BLOCK
{
    vector<WORD> W;
    BLOCK(string block)
    {
        for (unsigned i = 0; i < BLOCK_SIZE_in_WORDS; i++)
            W.push_back(WORD(block.substr(i * WORD_SIZE_in_NIBBLE, WORD_SIZE_in_NIBBLE)));
    }
    BLOCK()
    {
        string zero;
        for (unsigned i = 0; i < BLOCK_SIZE_in_NIBBLE; i++)
            zero += "0";
        this->W = BLOCK(zero).W;
    }
    BLOCK(vector<WORD> W) { this->W = W; }
    BLOCK block_xor(BLOCK block)
    {
        BLOCK ans;
        ans.W = this->W;
        for (unsigned i = 0; i < BLOCK_SIZE_in_WORDS; i++)
            ans.W[i] = ans.W[i].word_xor(block.W[i]);
        return ans;
    }
    void print()
    {
        for (auto word : W)
        {
            word.print();
            cout << " ";
        }
    }
};

BLOCK substitute(BLOCK block)
{
    for (auto &w : block.W)
        w = substitute(w);
    return block;
}

const unsigned NUM_ROUNDS = 10;
map<int, string> RC({
    {1, "01"},
    {2, "02"},
    {3, "04"},
    {4, "08"},
    {5, "10"},
    {6, "20"},
    {7, "40"},
    {8, "80"},
    {9, "1B"},
    {10, "36"},
});

vector<BLOCK> AES_key_schedule(BLOCK K)
{
    cout << "----------------\nAES-Key Schedule\n----------------\n\n";
    cout << "AES-128 Key: \nK = ";
    K.print();
    cout << "\n\n";

    map<int, WORD> RCON;
    for (unsigned i = 1; i <= 10; i++)
        RCON[i] = WORD(vector<BYTE>{BYTE(RC[i]), BYTE(), BYTE(), BYTE()});

    vector<BLOCK> expanded_key(NUM_ROUNDS + 1);
    expanded_key[0] = K;

    cout << "AES-128 Expanded Key Calculation: \n";
    cout << "W[00] = ";
    expanded_key[0].print();
    cout << "\n";

    for (unsigned i = 1; i <= NUM_ROUNDS; i++)
    {
        expanded_key[i] = BLOCK();
        expanded_key[i].W[0] = RCON[i].word_xor(expanded_key[i - 1].W[0].word_xor(substitute(left_rotate(expanded_key[i - 1].W[3]))));
        expanded_key[i].W[1] = expanded_key[i - 1].W[1].word_xor(expanded_key[i].W[0]);
        expanded_key[i].W[2] = expanded_key[i - 1].W[2].word_xor(expanded_key[i].W[1]);
        expanded_key[i].W[3] = expanded_key[i - 1].W[3].word_xor(expanded_key[i].W[2]);

        cout << "--------\nRound " << setfill('0') << setw(2) << dec << i << "\n--------\n";
        cout << "RCON[" << setfill('0') << setw(2) << dec << i << "] = ";
        RCON[i].print();
        cout << "\n";
        cout << "W[" << setfill('0') << setw(2) << i - 1 << "][0] = ";
        expanded_key[i - 1].W[0].print();
        cout << "\n";
        cout << "W[" << setfill('0') << setw(2) << i - 1 << "][3] = ";
        expanded_key[i - 1].W[3].print();
        cout << "\n";
        cout << "LEFT_ROTATE(W[" << setfill('0') << setw(2) << i - 1 << "][3]) = ";
        left_rotate(expanded_key[i - 1].W[0]).print();
        cout << "\n";
        cout << "AES_S-BOX(LEFT_ROTATE(W[" << setfill('0') << setw(2) << i - 1 << "][3])) = ";
        substitute(left_rotate(expanded_key[i - 1].W[0])).print();
        cout << "\n";
        cout << "\nW[" << setfill('0') << setw(2) << dec << i << "][0] = RCON[" << setfill('0') << setw(2) << dec << i << "] XOR W[" << setfill('0') << setw(2) << i - 1 << "][0] XOR AES_S-BOX(LEFT_ROTATE(W[" << setfill('0') << setw(2) << i - 1 << "][3])) = ";
        expanded_key[i].W[0].print();
        cout << "\n";
        cout << "W[" << setfill('0') << setw(2) << dec << i << "][1] = W[" << setfill('0') << setw(2) << i - 1 << "][1] XOR W[" << setfill('0') << setw(2) << i - 1 << "][0] =  ";
        expanded_key[i].W[1].print();
        cout << "\n";
        cout << "W[" << setfill('0') << setw(2) << dec << i << "][2] = W[" << setfill('0') << setw(2) << i - 1 << "][2] XOR W[" << setfill('0') << setw(2) << i - 1 << "][1] =  ";
        expanded_key[i].W[2].print();
        cout << "\n";
        cout << "W[" << setfill('0') << setw(2) << dec << i << "][3] = W[" << setfill('0') << setw(2) << i - 1 << "][3] XOR W[" << setfill('0') << setw(2) << i - 1 << "][2] =  ";
        expanded_key[i].W[3].print();
        cout << "\n";
        cout << "\nW[" << setfill('0') << setw(2) << dec << i << "] = ";
        expanded_key[i].print();
        cout << "\n";
    }

    cout << "\nAES-128 Expanded Key: \n";
    for (auto W : expanded_key)
    {
        W.print();
        cout << "\n";
    }

    return expanded_key;
}

BLOCK shift_rows(BLOCK M)
{
    vector<BYTE> row = {M.W[0].B[1], M.W[1].B[1], M.W[2].B[1], M.W[3].B[1]};
    M.W[0].B[1] = row[1];
    M.W[1].B[1] = row[2];
    M.W[2].B[1] = row[3];
    M.W[3].B[1] = row[0];
    row = {M.W[0].B[2], M.W[1].B[2], M.W[2].B[2], M.W[3].B[2]};
    M.W[0].B[2] = row[2];
    M.W[1].B[2] = row[3];
    M.W[2].B[2] = row[0];
    M.W[3].B[2] = row[1];
    row = {M.W[0].B[3], M.W[1].B[3], M.W[2].B[3], M.W[3].B[3]};
    M.W[0].B[3] = row[3];
    M.W[1].B[3] = row[0];
    M.W[2].B[3] = row[1];
    M.W[3].B[3] = row[2];
    return M;
}

BLOCK mix_col(BLOCK M)
{
    for (unsigned i = 0; i < BLOCK_SIZE_in_WORDS; i++)
    {
        vector<unsigned char> a(4), b(4), r(4);
        for (unsigned j = 0; j < WORD_SIZE_in_BYTES; j++)
        {
            r[j] = M.W[i].B[j].get_value();
            a[j] = r[j];
            unsigned char h = (unsigned char)((signed char)r[j] >> 7);
            b[j] = r[j] << 1;
            b[j] ^= 0x1B & h;
        }
        M.W[i].B[0] = BYTE(b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]);
        M.W[i].B[1] = BYTE(b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]);
        M.W[i].B[2] = BYTE(b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]);
        M.W[i].B[3] = BYTE(b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]);
    }
    return M;
}

BLOCK AES(BLOCK M, BLOCK K)
{
    vector<BLOCK> W = AES_key_schedule(K);

    cout << "-------\nAES-128\n-------\n";
    cout << "Message: \nM = ";
    M.print();
    cout << "\n";

    M = M.block_xor(W[0]);

    cout << "--------\nRound 00\n--------\n";
    cout << "Hash = M XOR W[0] = ";
    M.print();
    cout << "\n";

    for (unsigned round = 1; round <= NUM_ROUNDS; round++)
    {
        M = substitute(M);

        cout << "--------\nRound " << setfill('0') << setw(2) << dec << round << "\n--------\n";
        cout << "Hash = AES_S-BOX(Hash) = ";
        M.print();
        cout << "\n";
        
        M = shift_rows(M);
        
        cout << "Hash = ShiftRows(Hash) = ";
        M.print();
        cout << "\n";
        
        if (round < NUM_ROUNDS)
        {
            M = mix_col(M);
        
            cout << "Hash = MixColumns(Hash) = ";
            M.print();
            cout << "\n";
        }
        M = M.block_xor(W[round]);
        
        cout << "Hash = Hash XOR W[" << setfill('0') << setw(2) << dec << round << "] = ";
        M.print();
        cout << "\n";
    }
    
    cout << "\nMessage AES-128 Encryption: \nE = ";
    M.print();
    cout << "\n";
    
    return M;
}

int main()
{
    /* [Assumption] Input message will be of 128-bits exactly. Therefore, no padding will be needed. */
    string key, msg;
    
    cout << "AES-128 Key: ";
    cin >> key;
    cout << "128-bit Message: ";
    cin >> msg;

    AES(BLOCK(msg), BLOCK(key));
}