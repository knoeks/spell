// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;
// To check efficiency of the code I could check

int dict_length;

// TODO: Choose number of buckets in hash table
const unsigned int N = 7001;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    unsigned int index = hash(word);
    char lower_word[LENGTH + 1];
    int i = 0;

    while (word[i] != '\0')
    {
        lower_word[i] = tolower(word[i]);
        i++;
    }

    lower_word[i] = '\0';

    node *ptr = table[index];
    while(ptr != NULL)
    {
        if (strcmp(ptr->word, lower_word) == 0)
        {
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function


    if (word[1] == '\0')
    {
       return toupper(word[0]) - 'A';
    }
    else
    {
        // two word pointers:
        // i front {i, i + 1}
        // j back {j - 2, j - 1}

        //find j
        int j = 0;
        int a, b, x, y;
        while (word[j] !='\0')
        {
            j++;
        }

        a = toupper(word[0]) - '@';
        if (word[1] == 'n' && word[2] != '\0')
        {
            b = (toupper(word[2]) - '@') * 16;
        }
        else
        {
            b = (toupper(word[2]) - '@') * 16;
        }

        x = (toupper(word[j - 2]) - '@') * 256;
        y = (toupper(word[j - 1]) - '@') * 4096;

        return (a + b + x + y) % N;
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        fprintf(stderr, "Error opening dictionary file: %s\n", dictionary);
        return false;
    }

    // Scan dictionary words
    int letter = 0;
    char c, word[LENGTH + 1];

    // scans until file stream EOF
    while(fread(&c, sizeof(char), 1, dict))
    {
        // scans word letters, saves them
        if (c !='\n')
        {
            word[letter] = c;
            letter++;
        }
        else
        {
            // reset char counter and set it for new word
            word[letter] = '\0';

            // Get hash number
            unsigned int index = hash(word);

            node *n = malloc(sizeof(node));
            n->next = table[index]; // n is single node that is assigned to bucket table[N]
            strcpy(n->word, word);
            table[index] = n;
            letter = 0;
            dict_length++;
        }
    }

    //prints each chain length
    int empty = 0, longest = 0, identifier;
    for (int i = 0; i < N; i++)
    {
        int chain_len = 0;
        node *ptr = table[i];

        while (ptr != NULL)
        {
            ptr = ptr->next;
            chain_len++;
        }
        if (chain_len == 0)
        {
            printf("%i\n", i);
            empty++;
        }
        //printf("%i chain length: %i\n", i, chain_len);

        if (chain_len > longest)
        {
            identifier = i;
            longest = chain_len;
        }
    }
    printf("empty chains: %i\nlongest chain %i: %i\n", empty, identifier, longest);

    printf("Dictionary file opened successfully: %s\n", dictionary); // temporary

    fclose(dict);
    return true; // >>> IS IT GOOD PRACTICE FOR FUNCTION TO DEFAULT TO TRUE??
    // Change to true later
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO

    return dict_length;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for(int i = 0; i < N; i++)
    {
        node *ptr = table[i]; //pointer to node
        node *temp = NULL;

        while (ptr != NULL)
        {
            temp = ptr->next;
            free(ptr);
            ptr = temp;
        }

        if (ptr != NULL)
        {
            fprintf(stderr, "Error: node %i was not deallocated properly\n", i);
            return false;
        }
    }
    return true;
}
