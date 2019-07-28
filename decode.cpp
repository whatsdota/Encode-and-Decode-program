 /* -----------------------------------------------------------
 Encode and Decoding program
 Class: CS 141, Summer 2019. Mon/Wed 10 A.M.
 System: Windows 10
 Author: Jonathon Heikkinen
 Description of Program: A program that can encode and decode text
 -----------------------------------------------------------
 */
#include<iostream>
#include<cstring>
#include<fstream>
#include<cstdlib>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;
#define DICTSIZE 21876 //amount of words in the dictionary.txt file
string dictArray[DICTSIZE];
int max = 0;
int userInput;//initialize global variable for user's input
string originalText;

void loadDictArray(){//loads all the words from dictionary.txt into an array
    ifstream file("dictionary.txt");
    string str; 
    int counter = 0;
    while (getline(file, str)){//while loop to go through each line in the document and ass the line to the dictionary array
        dictArray[counter] = str;
        counter++;//counter to keep track of the line and add it to that index of dictArray
    }
}//end loadDictArray

bool lookUpWord(string word){//function to look up a word in the dictionary array 
    int i;
    int j;
    bool wordFound = false;
    for(i = 0; i < word.length(); i++){//for loop to change every char in word to lowercase
        word[i] = tolower(word[i]);
    }
    for(j = 0; j < DICTSIZE; j++){//for loop to iterate through dict array and compare each index to word
        if(word == dictArray[j]){//if word is found, break loop and wordFound = true
            wordFound = true;
            break;
        }
        else{
            continue;
        }
    }
    return wordFound;//returns true or false
}//end lookUpWord
void printHeader(){//function to display the info of the program
    cout<<"Author: Jonathon Heikkinen"<<endl;
    cout<<"Program 4: Decode"<<endl;
    cout<<"TA: Hasti Sharifi"<<endl;
    cout<<"JUL 26, 2019"<<endl;
    cout<<endl<<"Choose from the following options:"<<endl;
    cout<<"1. Encode some text"<<endl;
    cout<<"2. Decode using user-entered values"<<endl;
    cout<<"3. Decode the ciphertext given with the assignment"<<endl;
    cout<<"4. Exit program"<<endl;
   
}//end printHeader

int getUserInput(){//function to get the users input for which operation they'd like to use
    cout<<"Your choice: ";
    cin>>userInput;
    cout<<endl;
    return userInput;   
}//end getUserInput
string transposeString(string text, int transNum){//function that accepts a char array and int as parameters to transpose the string
    for(int i = 0; i < text.length(); i++){//for loop to iterate through character array and add transNum to ascii value of each character
        if(isalpha(text[i])){
            if((text[i] + transNum) >= 123){//if statement for if the ascii value is greater than 123(end of alphabet)
                text[i] = text[i] - (26 - transNum);                
            }
            else{
            text[i] = text[i] + transNum;
            }
        }
        else{
            continue;
        }
    }    
    return text;
}//end transposeString

string reverseString(string text){//function to reverse the given string
    string word; 
    string reversedString;
    stringstream iss(text); 
    while (iss >> word){ //while loop to take each word, reverse it, and append it to reversedString
        reverse(word.begin(),word.end()); 
        reversedString += word + ' ';
    } 
    return reversedString;
}//end of reverseString 

string vigenereEncoding(string text, string keyword){ //function to encode text using vigenere cipher
    string cipherText;  
            for (int i = 0, j = 0; i < text.length(); ++i)//for loop to encode for each character in user input
            {
                char c = text[i];
                if(isalpha(c)){//encode alphabet characters using vigenere cipher               
                    cipherText += (c + keyword[j] - 2 * 'a') % 26 + 'a';
                    j = (j + 1) % keyword.length();
                }
                else{//non alphabet characters are still kept in the string, and keyword continues
                    cipherText += c;
                    j = (j + 1) % keyword.length();                   
                }
            }
    return cipherText;
}//end vigenereEncoding
    
string vigenereDecoding(string text, string keyword){//function to decode text using vigenere cipher
    string origText;//string to store deciphered text to be returned 
    for (int i = 0, j = 0; i < text.length(); ++i)
            {
                char c = text[i];
                if(isalpha(c)){//if statement to run if char c is in the alphabet, and deciphering if so               
                origText += (c - keyword[j] + 26) % 26 + 'a';
                j = (j + 1) % keyword.length();
                }
                else{//else statement to print char c and continue moving along the keyword, but not deciphering
                    origText += c;
                    j = (j+1) % keyword.length();
                }
            }
    return origText;
}

void findTransposition(string text, string keyword){//function to test every possible transposition
    string vigenereDecoded = vigenereDecoding(text, keyword);//initializes string as the decoded version of the given text
    string transposedText;//holds the decoded and transposed string
    string reversedText;//holds the decodedString in reverse
    string word;
    string origText;//holds the potential original text
    int transposeNum;//holds the number to represent the transposition value
    int counter;
    string direction;//holds the direction the string was flipped
    for(int i = 0; i < 26; i++){
        counter = 0;
        for(int j = 0; j < 2; j++){
            if(j == 0){//j determines if the string should be reversed or not
                transposedText = transposeString(vigenereDecoded, i);
                stringstream iss(transposedText); 
                while (iss >> word){
                    if(lookUpWord(word) == true){
                        counter++;//adds to counter if a word is found in the dictionary
                    }
                }
                if(counter > ::max){//if this particular transposition has more words than the previous best case, it becomes the new best case
                    ::max = counter;
                    transposeNum = i;
                    origText = transposedText;
                    direction = "Forwards";
                    cout<<"\nNew best case. Dictionary hits: "<<::max<<", keyword: "<<keyword<<", transposition: "<<transposeNum<<','<<endl;
                    cout<<"direction: "<<direction<<' '<<endl;
                    cout<<origText<<endl;
                    ::originalText = origText;
                }                
            }
            else if(j == 1){//same as above but with added reversal of string
                reversedText = reverseString(vigenereDecoded);
                transposedText = transposeString(reversedText, i);
                stringstream iss(transposedText);
                while(iss >> word){
                    if(lookUpWord(word) == true){
                        counter++;
                    }
                }
                if(counter > ::max){
                    ::max = counter;
                    transposeNum = i;
                    origText = transposedText;
                    direction = "Reverse";
                    cout<<"\nNew best case. Dictionary hits: "<<::max<<", keyword: "<<keyword<<", transposition: "<<transposeNum<<','<<endl;
                    cout<<"direction: "<<direction<<' '<<endl;
                    cout<<origText<<endl;
                    ::originalText = origText;
                }
            }
        }
    }
}

void encodeText(){//function for if user would like to encode text
    char userText[100];//char array to store user input
    string fwdRvrs;//string to determine which way user wants characters shifted
    int transpositionValue;//int value to determine how far to move characters
    string keyword;//char array to store vigenere encryption word
    string origText; //used to store the original user input
    string cipherText;//string to hold the cipherText
    string transposedText;//string to hold the transposedText
    string reversedText;//string to hold the reversedText
    cout<<"Enter the text to be encoded: ";
    cin.ignore();
    cin.getline(userText, 100);//get text until user hits enter 
    origText = userText;
    cout<<"Enter direction(forward or reverse): ";
    cin>>fwdRvrs;//get userinput for forward or reverse
    cout<<"Enter transposition value(0..25): ";
    cin>>transpositionValue;//get user input for transposition value
    cout<<"Enter a keyword for Vigenere encryption: ";
    cin>>keyword;//get user input for a vigenere keyword   
    cout<<"Keyword is: "<<keyword<<endl<<endl;
    if(fwdRvrs == "reverse"){//if statement to determine if string should be reversed or not
        reversedText = reverseString(userText);   
        cout<<"reversed text:" <<reversedText<<endl;
        transposedText = transposeString(reversedText, transpositionValue);
        cipherText = vigenereEncoding(transposedText, keyword);
        cout<<"\nKeyword, plainText and cipher text are:"<<endl;
        for(int i = 0, j = 0; i < strlen(userText);i++){
            cout<<keyword[j];
            j = (j + 1) % keyword.length();        
        }
    }
    else if(fwdRvrs == "forward"){//if statement to determine if string should be reversed or not
        transposedText = transposeString(userText, transpositionValue);
        cipherText = vigenereEncoding(transposedText, keyword);
        cout<<"\nKeyword, plainText and cipher text are:"<<endl;
        for(int i = 0, j = 0; i < strlen(userText);i++){
            cout<<keyword[j];
            j = (j + 1) % keyword.length();
        }        
    }
    cout<<endl;
    cout<<origText<<endl;    
    cout<<cipherText<<endl;
}//end encodeText

void decodeText(){//function for if user would like to decode text
    string vigenereDecoded;
    char userText[100];
    char keyword[20];
    char keyword2[20];
    char keyword3[20];
    int counter;
    int max;
    cout<<"Enter the cipherText to be decoded: ";
    cin.ignore();
    cin.getline(userText, 100);
    cout<<"\nEnter the three Vigenere keywords to be tried:";
    cin>>keyword>>keyword2>>keyword3; 
    cout<<"\ncipher text is: ";
    for(int i = 0; i < strlen(userText); i++){
        cout<<userText[i];
    }
    cout<<"\nString of possible keywords is: "<<keyword<<' '<<keyword2<<' '<<keyword3;
    cout<<endl; 
    //find best transposition for each keyword
    findTransposition(userText, keyword);
    findTransposition(userText, keyword2);
    findTransposition(userText, keyword3);
    cout<<"\nDecrypted text is: "<<endl;
    cout<<::originalText<<endl;

}//end decodeText

void decodeCipherText(){//function to decode cipher text given with assignment
    ifstream file("analysis.txt");
    int counter = 0;//counter to hold how many words are in analysis.txt
    string wordArray[300];
    string str;//string to hold each line in analysis.txt
    string text;//string to hold analysis.txt contents
    string word;//string to hold each individual word in text    
    string decipherText = "IQHN OH XALM EJG JYOQ NBGNRV KMBQJ ZTNZZV";//string to be deciphered
    for(int i = 0; i < decipherText.length(); i++){//for loop to convert decipherText to all lowercase
        decipherText[i] = tolower(decipherText[i]);
    }   
    while (getline(file, str)){//while loop to add each line to a string text
        text += str;
    }
    stringstream iss(text); 
    while (iss >> word){//while loop to grab each word in analysis.txt and add 1 to counter for each 
        counter++;
        wordArray[counter] = word;
    }
    cout<<"ciphertext is: "<<decipherText<<endl;
    cout<<"String of possible keywords taken from file: analysis.txt"<<endl;
    for(int i = 1; i < counter + 1; i++){
        findTransposition(decipherText, wordArray[i]);
    }
    cout<<"\nDecrypted text is:"<<endl;
    cout<<::originalText<<endl;
    cout<<"Exiting program..."<<endl;
    exit(0);
}

int main(){
    printHeader();
    loadDictArray();
    /*if(lookUpWord("coconut") == true){
        cout<<"The requested word has been found your highness"<<endl;
    }*/
    userInput = getUserInput();
    
    if(userInput == 1){
        encodeText();   
    }
    else if(userInput == 2){
        decodeText();
    }   
    else if(userInput == 3){
        decodeCipherText();
    }
    else if(userInput == 4){
        cout<<"Exiting program..."<<endl;
        exit(0);
    }
}//end main
