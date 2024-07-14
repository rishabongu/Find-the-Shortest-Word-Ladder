/*-------------------------------------------
Program 5: Find the Shortest Word Ladder
Course: CS 211, Fall 2023, UIC
System: Advanced zyLab
Author: Risha Bongu 
------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

//This function opens a file with name <filename> and counts the number 
//of words in the file that are exactly <wordSize> letters long, where 
//a "word" is ANY set of characters that falls between two whitespaces 
//return the count, if filename is valid return -1 if the file cannot be 
// opened
int countWordsOfLength(char* filename, int wordSize) { 
    //opens the file
    FILE *myFile = fopen(filename, "r");
    //checks if the file cannot be opened 
    if(myFile == NULL){
        return -1;
    }

    int count = 0;
    char words[90];
    //reads the words from the file to the end of the file and checks if the length of the current word is equal to the word size
    while(fscanf(myFile, "%s", words) == 1){
        if(strlen(words) == wordSize){
            count++;
        }
    }
    //closes the file
    fclose(myFile);
    //returns the count of the words
    return count;
}
  
//opens a file with name <filename> and fills the 
//pre-allocated word array <words> with only words that are exactly <wordSize> letters long;
//the file contains exactly <numWords> words that are the correct number of letters
//<words> is pre-allocated as <numWords> char* ptrs, each pointing to a C-string of legnth <wordSize>+1;
//returns true if the file is opened successfully and the file contains exactly <numWords> words of exactly 
//<wordSize> letters, and those words are stored in the <words> array 
//return false otherwise
bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
    //opens the file
    FILE* myFile = fopen(filename, "r");
    //checks if the file cannot be opened 
    if (myFile == NULL) {
        return false;
    }
    //temp array that stores the words
    char maxWordSize[100];
    int count = 0;
    while(fscanf(myFile, "%s", maxWordSize) == 1){
        if(strlen(maxWordSize)== wordSize){
            //copies word into array
             strcpy(words[count], maxWordSize);
             count++;
        }
    }
    //closes the file
    fclose(myFile);
    //checks if valid words match the count 
    bool found = (count == numWords);
    //returns the result of the bool 
    return found;

}

//This function uses binary search for string <aWord> in an
// alphabetically sorted array of strings <words>, only between <loInd> 
// & <hiInd> return index of <aWord> if found return -1 if not found b/w
//loInd & hiInd
int findWord(char** words, char* aWord, int loInd, int hiInd) { 
    while(loInd <= hiInd){
        int middle;
        //finds the middle index
        middle = loInd + (hiInd - loInd) / 2;
        int compare;
        //compares the middle word with the search word
        compare = strcmp(words[middle], aWord);
        //checks if the middle word is less than the search word 
        if(compare < 0){
            loInd = middle + 1;
        }
        //checks if the middle word is equal to the search word
        else if(compare == 0){
            return middle;
        }
        //checks if the middle word is greater than the search word
        else{
            hiInd = middle - 1;
        }
    }
    //returns -1 if the word was not found in the range 
    return -1; 
    
}
//This function frees up all heap-allocated space for <words>, which is 
// an array of <numWords> C-strings 
// frees the space allocated for each C-string 
// frees the space allocated for the array  of pointers, <words>, itself
void freeWords(char** words, int numWords) {
    for(int i = 0; i < numWords; i++){
        free(words[i]);
    }
    free(words);
}

// This fucntion allocates space for a new [WordNode], set its[myWord] 
// subitem using <newWord> and insert it to the front of <ladder>, which 
// is a pointer-passed-by-pointer as the head node of ladder changes 
// inside this function; <newWord> is a pointer to a C-string from the 
// full word array, already heap-allocated
void insertWordAtFront(WordNode** ladder, char* newWord) {
    // allocates soace for the new word node
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
    //checks if allocation failed 
    if(newNode == NULL){
        return;
    }
    //sets the subitem of the new node
    newNode->myWord = newWord;
    //sets the next subitem 
    newNode->next = *ladder;
    //updates the head of the ladder 
    *ladder = newNode;

}
  
//finds and returns number of words in <ladder> list
int getLadderHeight(WordNode* ladder) {
    int height = 0;
    WordNode* curr = ladder;
    while(curr != NULL){
        height++;
        curr = curr->next;
    }
    return height;
}

//makes a complete copy of <ladder> and return it
//the copy ladder has new space allocated or each [WordNode] in 
//<ladder>, but the C-string pointers to elements of the full word array can be reused
WordNode* copyLadder(WordNode* ladder) {

    WordNode* copyLadder = NULL;
    WordNode* lastNode = NULL;
    WordNode* tempLadder = ladder;
    //allocates memory for a new node in the copy ladder
    while (tempLadder != NULL) {
        WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
        //checks if allocated correctly
        if (newNode == NULL) {
            return NULL;
        }
        //copies c-string from orginal ladder to the new node
        newNode->myWord = tempLadder->myWord;
        newNode->next = NULL;
        //uisng the position of the new node, copy ladder updates 
        if (lastNode == NULL) {
            copyLadder = newNode;
        } 
        else {
            lastNode->next = newNode;
        }
        //updates the previous node to the new node
        lastNode = newNode;
        //moves to the next node in the orginal ladder
        tempLadder = tempLadder->next;
    }
    //returns the copy ladder
    return copyLadder;

}

//frees up all heap-allocated space for <ladder>;
//free up the space that was allocated for each [WordNode]
void freeLadder(WordNode* ladder) {
    WordNode* curr = ladder;
    while (curr != NULL) {
        WordNode* next = curr->next;
        free(curr);
        curr = next;
    }
   
}

//allocates space for a new [LadderNode], set its [topWord] subitem using <newLadder>
//finds the back of <list> and append the newly created [LadderNode] to the back
// <list> is a pointer-passed-by-pointer 
//since function handle the edge case where <list> is empty and the new [LadderNode] becomes the head node
void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
   //allocates memory for a ladder node
    LadderNode* newLadderNode = (LadderNode*)malloc(sizeof(LadderNode));
    //sets the top word of the new ladder node
    newLadderNode->topWord = newLadder;
    newLadderNode->next = NULL;
    //checks if the list is empty
    //if list empty, updates the new ladder to the head node
    if(*list == NULL){
        *list = newLadderNode;
    }
    //if list not empty, traverses to the back by adding the new ladder node
    else{
        LadderNode* curr = *list;
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = newLadderNode;
    }
}

//pops the first Ladder from the front of the list by returning the 
//pointer to the head node of <list> AND updating the head node of <list> to the next [LadderNode]
//<list> is apointer-passed-by-pointer, since this functionupdates the head node to be one down the list;
//the [LadderNode] popped off the front must have its memory freed in this function, 
//since it will go out of scope, but the ladder itself
WordNode* popLadderFromFront(LadderNode** list) {
    //gets the top word from the head of the list
    WordNode* topWord = (*list)->topWord;
    //stores the head 
    LadderNode* currHead = *list;
    //updates the head to the next ladder node
    *list = currHead->next;
    //frees the memory of the popped ladder node
    free(currHead);
    //returns the top word of the popped ladder node
    return topWord; 
}

//frees up all heap-allocated space for <myList>;
//for each ladder in <myList>:
//frees the space allocated for each [WordNode] in the ladder using freeLadder()
//frees the space allocated for the [LadderNode] itself 
void freeLadderList(LadderNode* myList) {
    LadderNode* currLadder = myList;
    LadderNode* nextNode = NULL;
    while(currLadder != NULL){
        nextNode = currLadder->next;
        freeLadder(currLadder->topWord);
        free(currLadder);
        currLadder = nextNode;
    }
}

//helper function that checks if there is exactly one character difference between two words
//returns true if there is exactly one difference and retruns false otherwise
//uses word1, which is a pointer to the first character array of a word
//uses word2, whihc is a pointer to the second character array of a word
//uses wordSize, which is the size of each word
bool checkDifference(char* word1, char* word2, int wordSize) {
    int diffCount = 0;
    for (int i = 0; i < wordSize; ++i) {
        if (word1[i] != word2[i]) {
            diffCount++;
            if (diffCount > 1) {
                return false;  
            }
        }
    }
    return diffCount == 1;
}

//runs algorithm to find the shortest word ladder from <startWord> to <finalWord> in the <words>
//word array, where each word is <wordSize> longand there are <numWords> total words
//<usedWord> also has size <numWords>, such that usedWord[i] is only true if words[i] has 
//previously be entered into a ladder, and should therefore not be added to any other ladders 
//the algorithm creates partial word ladders, which are [WordNode] linked lists, and stores them in a [LadderNode] linked list
//this function return a pointer to the shortest ladder and returns NULL if no ladder is possible
//frees all heap-allocated memory that is created here that is not the returned ladder
WordNode* findShortestWordLadder(   char** words, 
                                    bool* usedWord, 
                                    int numWords, 
                                    int wordSize, 
                                    char* startWord, 
                                    char* finalWord ) {
    
    LadderNode* wordLadderList = NULL;
    //creates an initial ladder with the start word
    WordNode* beginningLadder = (WordNode*)malloc(sizeof(WordNode));
    beginningLadder->myWord = startWord;
    beginningLadder->next = NULL;
    //start word is used
    usedWord[findWord(words, startWord, 0, numWords - 1)] = true;
    //inserts the initial ladder at the back of the ladder word list
    insertLadderAtBack(&wordLadderList, beginningLadder);

    while (wordLadderList != NULL) {
        //pops the front of the ladder 
        WordNode* currLadder = popLadderFromFront(&wordLadderList);
        char* currWord = currLadder->myWord;
        //checks if the ladder gets up to the final word
        if (strcmp(currWord, finalWord) == 0) {
            freeLadderList(wordLadderList);
            return currLadder;
        }
        for (int i = 0; i < numWords; ++i) {
            if (!usedWord[i] && checkDifference(currWord, words[i], wordSize)) {
                //creates a new ladder that inserts the word at front, uses the word, and inserts the ladder at the back 
                WordNode* newLadder = copyLadder(currLadder);
                insertWordAtFront(&newLadder, words[i]);
                usedWord[i] = true;
                insertLadderAtBack(&wordLadderList, newLadder);
            }
        }
        //frees the memory of the current ladder
        freeLadder(currLadder);
    }
    //returns null if no ladder is found
    return NULL;
}

// interactive user-input to set a word; 
//  ensures sure the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize); 
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}


//-----------------------------------------------------
// The primary application is fully provided in main(); 
//  no changes should be made to main()
//-----------------------------------------------------
int main() {
    srand((int)time(0));
    
    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n"); 
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever 
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i] 
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize); 
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n"); 

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array, 
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);    
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));
    
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    return 0;
}
