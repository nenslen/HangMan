#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32 // For clearing stdout
    #include <windows.h>
#else
    #include <unistd.h>
#endif



/** Prototypes **/
void getRandomWord(int,char[]);     // Gets a random word for the user to guess
void strToLower(char[]);            // Converts a string to all lowercase characters
void fillUserString(char[],char[]); // Fills the user's guess string with underscores
void displayUserString(char[]);     // Displays the user's correct guesses and underscores
void displayGuessedLetters(int[]);  // Displays the letters the user has guessed already
void drawHangMan(int);              // Draws hangman
int validateLetter(char,int[]);     // Checks to see if a letter has been guessed already
char getGuess(int[]);               // Gets guess from the user
void saveGuess(char,int[]);         // Saves the guess the user enters in the guessed letters array
int checkGuess(char,char[],char[]); // Checks if guessed letter is part of word to be guessed
FILE* openFile(char*, char*);       // Opens a file



int main()
{
    char word[20] = {};          // Word to be guessed
    char userWord[20] = {};      // String to store user's guess (will be underscores and correctly guessed letters)
    char guess;                  // Holds user's current guess
    int guessedLetters[26] = {}; // Holds letters guessed by user (0 = not guessed, 1 = guessed)
    int guessesLeft = 6;         // How many guesses the user has left
    int wordIndex = 0;           // Index of the word
    int totalGuesses = 0;        // Keeps track of how many times the user has guessed
    int exitFlag = 0;            // Flag to exit the while loop

    // Gets random number, for index of word
    srand(time(NULL));
    rand();
    wordIndex = rand() % 124;

    // Gets random word to be guessed
    getRandomWord(wordIndex,word);

    // Fills the user's guess string with underscores
    fillUserString(userWord,word);

    while(1)
    {
        // Shows game information
        system("cls"); // Clears the command prompt
        printf("Hangman\n");
        printf("-------\n\n");
        drawHangMan(guessesLeft);              // Draws the hangman based on how many guess the user has left
        printf("Guesses Left: %d\n",guessesLeft);
        printf("Guessed Letters: ");
        displayGuessedLetters(guessedLetters); // Displays which letters the user has guessed
        printf("\nWord: ");
        displayUserString(userWord);           // Displays the user's word with underscores, their correct guesses, and spaces between each char

        // Allows the stats to be updates once more before exiting
        if(exitFlag == 1)
        { break; }

        guess = getGuess(guessedLetters);      // Gets a character, makes sure it hasn't already been guessed, and it's a-z
        saveGuess(guess,guessedLetters);       // Saves the valid guess in the guessed letters array
        totalGuesses++;                        // Keeps track of total guesses



        // Checks if user-guessed letter is part of the word to be guessed
        // Will update the userWord with the correct letters if it is
        if(checkGuess(guess,word,userWord) == 0)
        {
            guessesLeft--;
        }

        // Checks to see if user has any guesses left
        if(guessesLeft <= 0)
        {
            printf("You lost!\n");
            exitFlag = 1;
        }

        // Checks to see if user has won
        if(winCheck(word,userWord) == 0)
        {
            printf("You won!\n");
            exitFlag = 1;
        }
    }

    printf("\nGame stats:\n");
    printf("    Word: %s\n",word);
    printf("    Total guesses: %d\n",totalGuesses);
    printf("    Time elapsed: %lf\n",4.0);

    printf("Press enter to continue...\n");
    fflush(stdin);
    getchar();

    return 0;
}



/** Draws hangman **/
void drawHangMan(int guessesLeft) {
    switch(guessesLeft)
    {
        case 6:
            printf("  _______\n");
            printf(" |/     |\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|___\n");
            break;
        case 5:
            printf("  _______\n");
            printf(" |/     |\n");
            printf(" |     (_)\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|___\n");
            break;
        case 4:
            printf("  _______\n");
            printf(" |/     |\n");
            printf(" |     (_)\n");
            printf(" |     \\|\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|___\n");
            break;
        case 3:
            printf("  _______\n");
            printf(" |/     |\n");
            printf(" |     (_)\n");
            printf(" |     \\|/\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|___\n");
            break;
        case 2:
            printf("  _______\n");
            printf(" |/     |\n");
            printf(" |     (_)\n");
            printf(" |     \\|/\n");
            printf(" |      |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|___\n");
            break;
        case 1:
            printf("  _______\n");
            printf(" |/     |\n");
            printf(" |     (_)\n");
            printf(" |     \\|/\n");
            printf(" |      |\n");
            printf(" |     /\n");
            printf(" |\n");
            printf("_|___\n");
            break;
        case 0:
            printf("  _______\n");
            printf(" |/     |\n");
            printf(" |     (_)\n");
            printf(" |     \\|/\n");
            printf(" |      |\n");
            printf(" |     / \\\n");
            printf(" |\n");
            printf("_|___\n");
            break;
    }
    return;
}



/** Checks to see if user has guess the whole word **/
int winCheck(char word[],char userWord[]) {
    if(strcmp(word,userWord) == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}



/** Checks if guessed letter is part of word to be guessed, and fills in the blanks of the user string **/
int checkGuess(char guess,char word[],char userWord[]) {
    int i = 0;
    int returnVal = 0; // Returned to tell if letter was correct or not

    // Checks each letter of string against guessed letter
    for(i = 0; i <= strlen(word) - 1; i++)
    {
        if(guess == word[i])
        {
            userWord[i] = guess;
            returnVal = 1; // Letter was correct
        }
    }

    return returnVal; // User did not guess correct letter
}



/** Stores valid guessed letter in the array **/
void saveGuess(char guess,int guessedLetters[]) {
    // Finds out where to store it based on which letter it is
    guessedLetters[guess - 97] = 1;

    return;
}



/** Gets a character as a guess, makes sure it was not guessed first **/
char getGuess(int guessedLetters[]) {
    char c;

    while(1)
    {
        printf("\nEnter a letter: ");
        fflush(stdin); // Stops the "Letter must be a-z" message from showing unintentionally
        c = getchar();
        c = tolower(c);

        switch(validateLetter(c,guessedLetters))
        {
            case 0: // Char was not a letter
                printf("\nLetter must be a - z.");
                fflush(stdin); // Clears buffer to stop getchar() from repeating
                break;
            case 1: // Letter has already been guessed
                printf("\nLetter has already been guessed.");
                fflush(stdin); // Clears buffer to stop getchar() from repeating
                break;
            case 2: // Letter was valid
                return c;
        }
    }
    return c;
}



/** Makes sure a guess is valid (chars only) **/
int validateLetter(char c,int guessedLetters[]) {
    // Makes sure c is a char, not a number
    if(!(c >= 97 && c <= 122))
    {
        return 0; // Char is not a letter
    }

    // c - 97 will give the letter's location within the int array, if it's 1, then num has already been guessed
    if(guessedLetters[c - 97] == 1)
    {
        return 1; // Letter has been guessed
    }
    else
    {
        return 2; // Letter has not been guessed
    }

    return 2;
}



/** Gets a random word from textfile **/
void getRandomWord(int index,char word[]) {
    FILE* filePath = openFile("words.txt", "r");
    char line[20]; // Holds word to be guessed
    char ch;
    int lineCount = 0;
    int i = 0;

    // Loops through each line (word) until the desired word is reached
    while(!feof(filePath))
    {
        ch = fgetc(filePath); // Grabs char

        if(ch == '\n')        // End of current line
        {
            lineCount++;      // Counts how many words have been counted

            line[i] = '\0';   // Marks end of word, otherwise garbage chars appear after

            i = 0;            // Resets the char counter for the next word

            // This means the desired word index was reached
            if((lineCount - 1) == index)
            {
                break;
            }
        }
        else
        {
            // Puts current char into a string
            line[i] = ch;
            i++;
        }
    }

    fclose(filePath);

    // First makes the word all lowercase
    strToLower(line);

    // Copies the word at the index provided into the word to be guessed's placeholder
    strcpy(word,line);

    return;
}



/** Fills user's guess string with underscores **/
void fillUserString(char userWord[],char word[]) {
    int i = 0;

    for(i = 0; i <= strlen(word) - 1; i++)
    {
        userWord[i] = '_';
    }

    return;
}



/** Display's user's string, with spaces between chars **/
void displayUserString(char userWord[]) {
    int i = 0;
    int j = 0;

    // Adds spaces between characters
    for(i = 0; i <= (strlen(userWord) - 1) * 2; i++)
    {
        if(i % 2 == 1)
        {
            printf(" ");
        }
        else
        {
            printf("%c",userWord[j]);
            j++;
        }
    }
}



/** Displays's the user's guessed letters **/
void displayGuessedLetters(int guessedLetters[]) {
    int i = 0;
    char ch;

    // Goes through each letter, if it's a 1 it will display it
    for(i = 0; i <= 25; i++)
    {
        if(guessedLetters[i] == 1) // Letter has been guessed
        {
            // Displays the letter that corrosponds with i by adding 97 (a is 97) so b will be 1 + 97
            printf("%c ",i + 97);
        }
    }

    return;
}



/** Opens file or creates it if it doesn't exist **/
FILE* openFile(char* Name, char* Mode) {
    FILE* fp;
    char ModeName[15] = {};

    fp = fopen(Name, Mode);

    switch(Mode[0])
    {
        case 'a':
            strcpy(ModeName, "appending.\0");
            break;
        case 'w':
            strcpy(ModeName, "writing.\0");
            break;
        case 'r':
            strcpy(ModeName, "reading.\0");
            break;
        default:
            strcpy(ModeName, "unknown mode.\0");
            break;
    }

    if(fp == NULL)
    {
        printf("Error opening %s for %s\n", Name, ModeName);
    }
    else
    {
        //printf("File %s has been opened for %s\n", Name, ModeName);
    }

    return fp;
}



/** Converts a string to lowercase **/
void strToLower(char str[]) {
    int i = 0;

    for(i = 0; i <= strlen(str) - 1; i++)
    {
        str[i] = tolower(str[i]);
    }

    return;
}
