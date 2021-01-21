#include <iostream>
#include <string>
#include <algorithm>
#include <bitset>
#include <map>
#include <vector>
#include <cctype>

#define NIBBLE_SIZE_in_BITS 4
#define WORD_SIZE_in_BITS 8
#define BLOCK_SIZE_in_BITS 16
#define WORD_SIZE_in_NIBBLE (WORD_SIZE_in_BITS / NIBBLE_SIZE_in_BITS)
#define BLOCK_SIZE_in_NIBBLE (BLOCK_SIZE_in_BITS / NIBBLE_SIZE_in_BITS)
#define BLOCK_SIZE_in_WORD (BLOCK_SIZE_in_BITS / WORD_SIZE_in_BITS)

using namespace std;

inline void remove_spaces(string &s) { s.erase(remove(s.begin(), s.end(), ' '), s.end()); }

#define NIB bitset<NIBBLE_SIZE_in_BITS>

map<char, NIB> HEX_to_NIBBLE({{'0', NIB(0)}, {'1', NIB(1)}, {'2', NIB(2)}, {'3', NIB(3)}, {'4', NIB(4)}, {'5', NIB(5)}, {'6', NIB(6)}, {'7', NIB(7)}, {'8', NIB(8)}, {'9', NIB(9)}, {'A', NIB(10)}, {'B', NIB(11)}, {'C', NIB(12)}, {'D', NIB(13)}, {'E', NIB(14)}, {'F', NIB(15)}});
vector<char> NIBBLE_to_HEX = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

struct WORD
{
    vector<NIB> N;
    WORD(string word)
    {
        for (auto nib : word)
        {
            N.emplace_back(HEX_to_NIBBLE[(char)(isalpha(nib) ? toupper(nib) : nib)]);
        }
    }
    WORD()
    {
        N.emplace_back(HEX_to_NIBBLE['0']);
        N.emplace_back(HEX_to_NIBBLE['0']);
    }
    WORD operator^(WORD const &word)
    {
        WORD result;
        for (unsigned nib_ind = 0; nib_ind < WORD_SIZE_in_NIBBLE; nib_ind++)
        {
            result.N[nib_ind] = N[nib_ind] ^ word.N[nib_ind];
        }
        return result;
    }
    string get_string()
    {
        string result;
        for (auto nib : N)
        {
            result += NIBBLE_to_HEX[nib.to_ulong()];
        }
        return result;
    }
};

struct BLOCK
{
    vector<WORD> W;
    BLOCK(string block)
    {
        for (unsigned word_ind = 0; word_ind < BLOCK_SIZE_in_WORD; word_ind++)
        {
            W.push_back(WORD(block.substr(word_ind * WORD_SIZE_in_NIBBLE, WORD_SIZE_in_NIBBLE)));
        }
    }
    BLOCK()
    {
        W.push_back(WORD());
        W.push_back(WORD());
    }
    string get_string()
    {
        string result;
        for (auto word : W)
        {
            result += word.get_string();
        }
        return result;
    }
};

struct STATE
{
    vector<vector<NIB>> N;
    STATE(BLOCK block)
    {
        N.resize(WORD_SIZE_in_NIBBLE, vector<NIB>(WORD_SIZE_in_NIBBLE));
        for (unsigned nibble_ind = 0; nibble_ind < BLOCK_SIZE_in_NIBBLE; nibble_ind++)
        {
            N[nibble_ind % 2][nibble_ind / 2] = block.W[nibble_ind / 2].N[nibble_ind % 2];
        }
    }
    BLOCK get_block()
    {
        BLOCK result;
        for (unsigned nibble_ind = 0; nibble_ind < BLOCK_SIZE_in_NIBBLE; nibble_ind++)
        {
            result.W[nibble_ind / 2].N[nibble_ind % 2] = N[nibble_ind % 2][nibble_ind / 2];
        }
        return result;
    }
    void print()
    {
        cout << "[\n";
        for (auto row : N)
        {
            cout << " [ ";
            for (auto cell : row)
            {
                cout << NIBBLE_to_HEX[cell.to_ulong()] << " ";
            }
            cout << "]\n";
        }
        cout << "]\n";
    }
};

void get_input(string &msg, string &key)
{
    cout << "\n----\nNOTE\n----\nBoth message and key must:\n* Be in hexadecimal format without the prefix '0x'.\n* Have equal size which should be in multiples of 16-bits (after the removal of all the space characters).\n\n";
    do
    {
        do
        {
            cout << "Message:\n";
            cin >> msg;

            remove_spaces(msg);

            cout << "Checking whether message size is appropriate ...\n";
        } while (msg.length() % BLOCK_SIZE_in_NIBBLE);

        do
        {
            cout << "Key:\n";
            cin >> key;

            remove_spaces(key);

            cout << "Checking if key size is appropriate ...\n";
        } while (key.length() % BLOCK_SIZE_in_NIBBLE);

        cout << "Checking if message and key are of equal size ...\n";
    } while (msg.length() != key.length());
}

void add_round_key(STATE &S, const BLOCK &K)
{
    for (unsigned key_word_ind = 0; key_word_ind < K.W.size(); key_word_ind++)
    {
        for (unsigned nib_ind = 0; nib_ind < K.W[key_word_ind].N.size(); nib_ind++)
        {
            // Addition in GF(2^4) which is equivalent to bitwise-XOR
            S.N[nib_ind][key_word_ind] ^= K.W[key_word_ind].N[nib_ind];
        }
    }
}

vector<vector<NIB>> sub_nibble_table = {
    {HEX_to_NIBBLE['9'], HEX_to_NIBBLE['4'], HEX_to_NIBBLE['A'], HEX_to_NIBBLE['B']},
    {HEX_to_NIBBLE['D'], HEX_to_NIBBLE['1'], HEX_to_NIBBLE['8'], HEX_to_NIBBLE['5']},
    {HEX_to_NIBBLE['6'], HEX_to_NIBBLE['2'], HEX_to_NIBBLE['0'], HEX_to_NIBBLE['3']},
    {HEX_to_NIBBLE['C'], HEX_to_NIBBLE['E'], HEX_to_NIBBLE['F'], HEX_to_NIBBLE['7']}};

void sub_nibble_transformation(STATE &S)
{
    for (auto &row : S.N)
    {
        for (auto &cell : row)
        {
            cell = sub_nibble_table[(cell >> 2).to_ulong()][((cell << 2) >> 2).to_ulong()];
        }
    }
}

inline void shift_rows(STATE &S) { swap(S.N[1][0], S.N[1][1]); }

unsigned GF(unsigned x)
{
    bitset<6> x_in_bin(x);
    if (x_in_bin[5] == 1)
    {
        x_in_bin[5] = 0;
        x_in_bin[2] = ((x_in_bin[2]) ? 0 : 1);
        x_in_bin[1] = ((x_in_bin[1]) ? 0 : 1);
    }
    if (x_in_bin[4] == 1)
    {
        x_in_bin[4] = 0;
        x_in_bin[1] = ((x_in_bin[1]) ? 0 : 1);
        x_in_bin[0] = ((x_in_bin[0]) ? 0 : 1);
    }
    return x_in_bin.to_ulong();
}

void mix_columns(STATE &S)
{
    for (unsigned col = 0; col < 2; col++)
    {
        auto temp = S.N[0][col] ^ NIB(GF(4 * S.N[1][col].to_ulong()));
        S.N[1][col] = NIB(GF(4 * S.N[0][col].to_ulong())) ^ S.N[1][col];
        S.N[0][col] = temp;
    }
}

vector<WORD> RCON = {WORD("80"), WORD("30")};

WORD sub_word(WORD word)
{
    for (auto &nib : word.N)
    {
        nib = sub_nibble_table[(nib >> 2).to_ulong()][((nib << 2) >> 2).to_ulong()];
    }
    return word;
}

WORD rot_word(WORD word) 
{ 
    swap(word.N[0], word.N[1]); 
    return word;
};

BLOCK key_expansion(BLOCK K, unsigned round_num)
{
    BLOCK W;
    W.W[0] = K.W[0] ^ (RCON[round_num - 1] ^ sub_word(rot_word(K.W[1])));
    W.W[1] = K.W[1] ^ W.W[0];
    return W;
}

string S_AES(const string &plain_text, const string &key)
{
    string cipher_text;
    for (uint_fast64_t block_ind = 0; block_ind < (plain_text.length() / BLOCK_SIZE_in_NIBBLE); block_ind++)
    {
        BLOCK M(plain_text.substr(block_ind * BLOCK_SIZE_in_NIBBLE, BLOCK_SIZE_in_NIBBLE));
        BLOCK K(key.substr(block_ind * BLOCK_SIZE_in_NIBBLE, BLOCK_SIZE_in_NIBBLE));

        cout << "Plain-text: " << M.get_string() << "\n";
        cout << "Key: " << K.get_string() << "\n";
        
        // Pre-round transformation
        STATE S(M);
        add_round_key(S, K);

        cout << "\n-------\nRound 0\n-------\n";
        cout << "State:\n";
        S.print();

        //Round-1
        sub_nibble_transformation(S);
        shift_rows(S);
        mix_columns(S);
        K = key_expansion(K, 1);
        add_round_key(S, K);

        cout << "\n-------\nRound 1\n-------\n";
        cout << "State:\n";
        S.print();

        //Round-2
        sub_nibble_transformation(S);
        shift_rows(S);
        K = key_expansion(K, 2);
        add_round_key(S, K);

        cout << "\n-------\nRound 2\n-------\n";
        cout << "State:\n";
        S.print();

        BLOCK C = S.get_block();
        cipher_text += C.get_string();
    }
    return cipher_text;
}

int main()
{
    cout << "\n----------------\nS-AES Encryption\n----------------\n";
    string plain_text, key;
    get_input(plain_text, key);
    string cipher_text = S_AES(plain_text, key);
    cout << "In hexadecimal without the prefix '0x', S-AES(" << plain_text << ", " << key << "):\n"
         << cipher_text << "\n";
}