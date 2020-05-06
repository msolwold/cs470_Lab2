/*
 * Michael Solwold
 * CS470
 * Lab 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>

/*
 * Attempts to open file
 *
 * @param char*
 * @returns FILE*
 */
FILE *openFile(char *filename)
{
    FILE *file;
    file = fopen(filename, "r");

    if (!file)
    {
        printf("Error opening file!\n");
        return NULL;
    }
    return file;
}

/*
 * Formatted output of statistics
 *
 * @param int*
 */
void outputResults(int *charCount)
{
    long numLetters = 0;
    long totalChars = 0;
    for (int i = 32; i < 128; i++)
    {
        totalChars += charCount[i];
        if (i >= 97 && i <= 122)
            numLetters += charCount[i];
    }

    printf("\n Letter Frequencies\n\n");
    printf(" Char   |  Count\t[%%]\n");
    printf("------- | -----------------------\n");

    for (int i = 97; i < 123; i++)
    {
        printf("   %c    |  %d ", i, charCount[i]);
        printf(" \t[%.2f%%]\n", ((double)charCount[i] / numLetters) * 100);
    }

    printf("----------------------------------\n");
    printf("\n File Statistics\n\n");
    printf(" Char Type |  Count\t[%%]\n");
    printf("---------- | ---------------------\n");

    printf("  Letters  |  %li", numLetters);
    printf(" \t[%.2f%%]\n", ((double)numLetters / totalChars) * 100);
    printf("  Other    |  %li", totalChars - numLetters);
    printf(" \t[%.2f%%]\n", ((double)(totalChars - numLetters) / totalChars) * 100);
    printf("  Total    |  %li\n\n", totalChars);
}

/*
 * Creates a process for each letter of the alphabet
 * Opens the file and counts the number of occurrences of a specific letter
 * Uses ASCII codes to keep track of count
 *
 * @param char* filename to parse
 * @returns int* array of frequency of characters
 */
int *countLetters(char *filename)
{
    int *charCount;
    FILE *file;
    charCount = mmap(NULL, 128 * sizeof(*charCount), PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    for (int i = 0; i < 27; i++)
    {
        int c;

        if ((file = openFile(filename)) == NULL)
        {
            printf("Error opening file in child process %d!\n", getpid());
            exit(1);
        }

        pid_t pID = fork();

        if (pID == -1)
        {
            printf("Error forking process!\n");
            exit(1);
        }
        else if (pID == 0)
        {
            while ((c = tolower(fgetc(file))) != EOF)
            {
                if (i == 26 && (c < 97 || c > 122))
                    charCount[c]++;                     /* count other char */
                else if (c == i + 97)
                    charCount[i + 97] += 1;             /*  count letters   */
            }

            fclose(file);
            exit(0);
        }
        else rewind(file);
    }
    for (int i = 0; i < 27; i++)
        wait(NULL);

    return charCount;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("File Name not provided. Please provide a file name as a command line argument for this program to run.\n");
        return 1;
    }

    char *filename = argv[1];
    FILE *file;

    if ((file = openFile(filename)) == NULL)
        return 1;

    while (1)
    {
        char uResponse[3];
        printf("Would you like to parse the file '%s'? [y/n]: ", filename);
        scanf("%s", uResponse);
        if (strcmp(uResponse, "y") == 0)
            break;
        else if (strcmp(uResponse, "n") == 0)
            return 0;
        else
            printf("Invalid Response!\n");
    }

    outputResults(countLetters(filename));

    if (fclose(file) != 0)
    {
        printf("Error closing file!\n");
        return 1;
    }

    return 0;
}