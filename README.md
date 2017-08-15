README.md

**DNA Alignment**
=

About
-----

This program generates the best possible DNA alignment for two given DNA sequences. The alignment is scored based on matches, mismatches, and gaps between letters, given scores of +2, -1, and -5 respectively. Higher
scores are obviously better. When results are printed, matches are represented by a "|", mismatches by a "\*", and gaps by the label of the string which contains the character. Space characters within the original sequence are treated as mismatches.

The alignment algorithm works recursively with dynamic programming. Because we are comparing DNA sequences, identical comparisons of substrings of the sequences occur frequently. This is then used to our advantage. We save the best score of these comparisons within an unordered_map. We then recursively call our function three times, cutting out the first character of the first string, then the first character of the second string, and finally the first character of both strings. When this is finished, we compare the scores, save the score of the best comparison within our unordered_map, and return it.

**Note to reader:** This program was originally a project for one of my CS
classes, so some starter code was given. This included only the align_result
struct and the DNA_align print function. The algorithm, align function, and
other code was written by me.

Usage:
------

To compile the code, simply run the `make` command from the Makefile, and to run use `./align <sequence1> <sequence2>`. If either sequence has a space, simply precede it with a \\ Ex: `./align abracadabra avada\ kedavra`
