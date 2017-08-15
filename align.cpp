//
//  align.cpp
//  dna_alignment
//
//  Base code created by Erika DeBenedictis on 1/27/13.
//  Alignment algorithm written by Zafir Abou-Zamzam.
//
//  Copyright (c) 2014 California Institute of Technology. All rights reserved.
//
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// scoring values
#define GAP_SCORE -5
#define MISMATCH -1
#define MATCHING 2

/**
 * @brief Packages the score and instruction string the align function returns
 */
struct align_result {
    int score;      // score of this alignment
    string inst;    // instruction on how to align the inputs

    align_result(int s, string i) {
        this->score = s;
        this->inst = i;
    }
    align_result() {
        this->score = 0;
        this->inst = "";
    }
};

// memo_type will allow us to hash the string input to align
// with its output for memoization
typedef unordered_map<string, align_result> memo_type;

/**
 * @brief Function takes two strings, s and t, and produces an align_result
 * of the highest alignment score and its corresponding instruction str.
 *
 */
align_result align(string s, string t, memo_type &memo) {

    if(s.empty() || t.empty())
    {
        // return empty align result
        if(s.empty() && t.empty())
        {
            return align_result();
        }

        // return gaps for the remainder of the other strings
        else if(s.empty())
        {
            string result = "";
            int i;
            for(i = 0; i < t.length(); i++)
            {
                result += "t";
            }
            return align_result(-5 * (i), result);
        }
        else
        {
            string result = "";
            int i;
            for(i = 0; i < s.length(); i++)
            {
                result += "s";
            }
            return align_result(-5 * (i), result);
        }
    }
    // if this result is memoized, use recorded result
    string key = s + "," + t;
    if (memo.count(key) > 0){
        return memo[key];
    }


    /*
      calculate the highest score for an alignment of s and t
      - Base cases: s or t is empty
      - Recursive calls
     */

    // break up the string into multiple parts

    // take the first character off the second string and first string
    string t2 = t.substr(1, t.length() - 1);
    string s2 = s.substr(1, s.length() - 1);

    // comparison when chopping off 2nd string
    align_result a1 = align(s, t2, memo);
    // comparison result when chopping off both
    align_result a2 = align(s2, t2, memo);
    // result when cutting off 1st
    align_result a3 = align(s2, t, memo);

    align_result answer;

    // update the scores before comparing
    a1.score -= 5;
    a1.inst = 't' + a1.inst;

    a3.score -= 5;
    a3.inst = 's' + a3.inst;

    if(s.at(0) == t.at(0))
    {
        a2.score += 2;
        a2.inst = "|" + a2.inst;
    }
    else
    {
        a2.score -= 1;
        a2.inst = "*" + a2.inst;
    }


    if(a2.score >= a1.score && a2.score >= a3.score)
    {
        answer = a2;

    }
    // this means a gap is given with gap string in s
    else if(a1.score > a2.score && a1.score >= a3.score)
    {
        answer = a1;
    }

    else
    {
        answer = a3;
    }

    // save the alignment score before returning
    memo[key] = answer;
    return answer;
}

/**
 * @brief Wrapper function to print the results of align
 */
void DNA_align(string s, string t) {
    cout << endl << "Calling DNA align on strings " << s << ", " << t << endl;

    // create the memoization system
    memo_type memo;

    align_result answer = align(s, t, memo);
    string ans = answer.inst;
    int score = answer.score;

    // Printing section
    // line where string s will be printed, spaces inserted
    string line1 = "";
    // line where string t will be printed, spaces inserted
    string line2 = "";
    // description of the relationship between s and t here (* | s t)
    string line3 = "";

    int j = 0;      // running index in s
    int k = 0;      // running index in t

    for (unsigned int m = 0; m < ans.length(); m++) {
        // i is the next element in our instruction string ans
        string i = ans.substr(m, 1);

        // only in s
        if(i.compare("s") == 0){
            line1 += s[j]; j++;
            line2 += " ";
            line3 += "s";
        }

        // only in t
        else if (i.compare("t") == 0){
            line1 += " ";
            line2 += t[k]; k++;
            line3 += "t";
        }

        // mismatch
        else if (i.compare("*") == 0){
            line1 += s[j]; j++;
            line2 += t[k]; k++;
            line3 += "*";
        }

        // match
        else {
            line1 += s[j]; j++;
            line2 += t[k]; k++;
            line3 += "|";
        }
    }
    cout << line1 << endl << line2 << endl << line3 << endl;
    cout << "Score for this alignment: " << score << endl;
}

int main(int argc, char** argv){

    if(argc != 3)
    {
        cout << "Usage: ./align <Sequence 1> <Sequence 2>" << endl;
        return 1;
    }

    DNA_align(argv[1], argv[2]);

    // uncomment these to see the alignment in action with some test cases
    // DNA_align("",   "a");
    // DNA_align("b",  "");
    // DNA_align("a", "a");
    // DNA_align("b",  "a");
    // DNA_align("b",  "ba");
    // DNA_align("ab", "ba");
    // DNA_align("ab", "b");
    // DNA_align("ACTGGCCGT", "TGACGTAA");
    // DNA_align("abracadabra", "avada kedavra");
    // DNA_align("avada kedavra", "abracadabra");

    // uncomment for real world examples of dna alignment
    // see some_alignments.txt for a description of the following sequences

    // DNA_align("GCAGACTACACTCTCGACGAGGAAAGCAGGGCTAGGATTAAGACCAGGCTATTTACCATA
    // AGACAAGAAATGGCCAACAGAGGCCTCTGGGATTCCTTTCGTCAGTCCGAAAGAGGCGAAGAAACAATTGA
    // AGAAAAATTTGAAATCTCAGGAACTATGCGCAGGCTTGCCGACCAAAGTCTCCCGCCGAACTTCTCCTGCC
    // TTGAGAATTTTAGAGCCTATGTGGATGGATTCGAACCGAACGGCTGCATTGAGGGCAAGCTTTCACAAATG
    // TCCATAGAAGTGAATGCCAAAATTGAACCTTTTTTGAAGACAACAC", "CACTCTGCTGCATTTGAAGAT
    // TTAAGATTGTTAAGCTTCATCAGAGGGACCAAAGTATCTCCGCGGGGGAAACTTTCAACTAGAGGAGTACA
    // AATTGCTTCAAATGAGAACATGGATAATATGGGATCGAGTACTCTTGAACTGAGAAGCGGGTACTGGGCCA
    // TAAGGACCAGGAGTGGAGGAAACACTAATCAACAGAGGGCCTCCGCAGGCCAAATCAGTGTGCAACCTACG
    // TTTTCTGTACAAAGAAATCTCCCATTTGAAAAGTCAACCGTCATGGCAGCATTCACTGGAAATACGGAGGG
    // AAGAACCTCAGACATGAGGGCAGAAATCATAAGAATGATGGAAGGTGCAAAACCAGAAGAAGTGTCGTTCC
    // GGGGGAGG");

    return 0;
}
