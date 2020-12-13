#include<iostream>
#include<fstream>
#include<string>
using namespace std;

bool IsVowel(char token);
bool IsConstantPairStarter(char token);
bool IsConstantPairStarterVariant(char token);
bool IsConstantNotPairStarter(char token);

/* Look for all **'s and complete them */

//=====================================================
// File scanner.cpp written by: Group Number: 10
//=====================================================

// --------- Two DFAs ---------------------------------

// WORD DFA 
// Done by: * Matthew Flavin *
// RE:   * (vowel | vowel n | consonant vowel | consonant vowel n | consonant-pair vowel | consonant-pair vowel n)^+ *
// outputs: bools
// This function is modeled after the DFA of the NFA given.
// It iterates throughout a word, following states based on the given character.
// An enum named wordType is created to keep track of the current states.
bool word (string word)
{
    // Check that string exists and is not empty.
    if (word.size() > 0)
    {
        // Create enum for states
        enum wordType { q0, q1, qy, qt, qc, qsa, qs, q0q1, q0qy, ERROR };

        // Create a char to hold the current index of string.
        char currentToken;

        // Create instance of enum to hold current state
        wordType currentState = q0;

        // Begin looping through the input word.
        for (size_t i = 0; i < word.size(); i++)
        {

            // Get the current toke at the loop index.
            currentToken = word[i];
 
            // Debug Line
            // cout << "Current state is: " << currentState << endl;
            // cout << "Current token is: " << currentToken << endl;

            // TRANSITION STATE: q0 ---------------------------------------
            if (currentState == q0)
            {
                if (currentToken == 'c')
                    currentState = qc;

                else if (currentToken == 't')
                    currentState = qt;

                else if (currentToken == 's')
                    currentState = qs;

                else if (IsVowel(currentToken))
                    currentState = q0q1;

                else if (IsConstantPairStarter(currentToken))
                    currentState = qy;
                
                else if (IsConstantNotPairStarter(currentToken))
                    currentState = qsa;

                // No found path, error.
                else
                {
                    currentState = ERROR;
                }
            }

            // TRANSITION STATE: q1 ---------------------------------------
            else if (currentState == q1)
            {
                if (currentToken == 'n')
                    currentState = q0;

                else
                    currentState = ERROR;
            }

            // TRANSITION STATE: qy ---------------------------------------
            else if (currentState == qy)
            {
                if (IsVowel(currentToken))
                {
                    currentState = q0q1;
                    //cout << "In Vowel" << endl;
                }


                else if (currentToken == 'y')
                {
                    currentState = qsa;
                    //cout << "In y" << endl;
                }


                else if (IsConstantNotPairStarter(currentToken))
                {
                    currentState = q1;
                    //cout << "CNPS" << endl;
                }


                else
                    currentState = ERROR;
            }

            // TRANSITION STATE: qt ---------------------------------------
            else if (currentState == qt)
            {
                if (IsVowel(currentToken))
                    currentState = q0q1;

                else if (currentToken == 's')
                    currentState = qsa;

                else
                    currentState = ERROR;
            }

            // TRANSITION STATE: qc ---------------------------------------
            else if (currentState == qc)
            {
                if (currentToken == 'h')
                    currentState = qsa;
                
                else
                    currentState = ERROR;
            }

            // TRANSITION STATE: qsa ---------------------------------------
            else if (currentState == qsa)
            {
                if (IsVowel(currentToken))
                    currentState = q0q1;

                else
                    currentState = ERROR;
            }

            // TRANSITION STATE: qs ---------------------------------------
            else if (currentState == qs)
            {
                if (currentToken == 'h')
                    currentState = qsa;

                else if (IsVowel(currentToken))
                    currentState = q0q1;

                else
                    currentState = ERROR;
            }

            // TRANSITION STATE: q0q1 ---------------------------------------
            else if (currentState == q0q1)
            {
                if (currentToken == 'c')
                    currentState = qc;

                else if (currentToken == 't')
                    currentState = qt;

                else if (IsVowel(currentToken))
                    currentState = q0q1;

                else if (IsConstantPairStarterVariant(currentToken))
                    currentState = qy;

                else if (IsConstantNotPairStarter(currentToken))
                    currentState = qsa;

                else if (currentToken == 's')
                    currentState = qs;

                else if (currentToken == 'n')
                    currentState = q0qy;

                else
                    currentState = ERROR;
            }

            // TRANSITION STATE: q0qy ---------------------------------------
            else if (currentState == q0qy)
            {
                if (currentToken == 'c')
                    currentState = qc;

                else if (currentToken == 't')
                    currentState = qt;

                else if (currentToken == 's')
                    currentState = qs;

                else if (IsVowel(currentToken))
                    currentState = q0q1;

                else if (IsConstantPairStarterVariant(currentToken))
                    currentState = qy;

                else if (IsConstantNotPairStarter(currentToken))
                    currentState = qsa;

                else if (currentToken == 'y')
                    currentState = qsa;

                else
                    currentState = ERROR;
            }

            // The DFA could not continue, lexical error. Break loop.
            else if (currentState == ERROR)
            {
                break;
            }

        } // END OF FOR LOOP.

        // Check that the loop ended in a final state. (q0, q0q1, q0qy)
        if (currentState == q0 || currentState == q0q1 || currentState == q0qy)
        {
            return true;
        }

        // Return Error Statement.
        else
        {
            return false;
        }
    }
}

// PERIOD DFA 
// Done by: * Matthew Flavin *
bool period (string word)
{
    // Check that word size is only 1.
  if (word.size() == 1)
  {
      // Check if the only token in the word is a period, if so, return true.
      if(word[0] == '.')
      {
          return true;
      }
  }

  // Not a period, return false.
  return false;
}

// ------ Three  Tables -------------------------------------

// TABLES Done by: Jackson and Jesse

// ** Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, EOFM, etc.
enum tokentype {ERROR, EOFM, PERIOD, WORD1, WORD2, VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR};

// ** For the display names of tokens - must be in the same order as the tokentype.
string tokenName[30] = {"ERROR", "EOFM", "PERIOD", "WORD1", "WORD2", "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", "IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR"}; 

// ** Need the reservedwords table to be set up here. 
// ** Do not require any file input for this. Hard code the table.
// ** a.out should work without any additional files.

string reservedWords[18] = {"masu", "masen", "mashita", "masendeshita", "desu", "deshita", "o", "wa", "ni", "watashi", "anata", "kare", "kanojo", "sore", "mata", "soshite", "shikashi", "dakara"};


// ------------ Scanner and Driver ----------------------- 

ifstream fin;  // global stream for reading from the input file

// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// ** Done by: Jackson and Jesse
int scanner(tokentype& tt, string& w)
{

  // ** Grab the next word from the file via fin
  // 1. If it is eofm, return right now.   
  bool valid = false; //a bool to check if w qualifies as a word
  fin >> w; // read in next word from file and set it to the string w
  if(w == "eofm"){ //if word is eofm set tt to EOFM and exit the function
    tt = EOFM;
    return 1;
  }
  /*  **
  2. Call the token functions (word and period) 
     one after another (if-then-else).
     Generate a lexical error message if both DFAs failed.
     Let the tokentype be ERROR in that case.
  */
  if(word(w) == true){ // if it is a word set valid to true
    valid = true;
  }
  else if(period(w) == true){// if it is a period set tt to period and exit the function
    tt = PERIOD;
    return 1;
  }
  else{ // if its not a word or a period it is an error
    cout << "ERROR: " << w << " was not recognized to be a word or a period!" << endl;
    tt = ERROR;
  }
	/*
  3. If it was a word,
     check against the reservedwords list.
     If not reserved, tokentype is WORD1 or WORD2
     decided based on the last character.
	*/
  
  int arrsize = sizeof(reservedWords)/sizeof(reservedWords[0]); // get the size of the array

  if(valid == true)
  {
    for(int x = 0; x < arrsize; x++)
    {// check to see if w matches a word from the reserved word list

        if(w == reservedWords[x])
        { // if it does match set tt to reserved and exit the function

            if(w == "masu")
                tt = VERB;

            else if(w == "masen")
                tt = VERBNEG;

            else if(w == "mashita")
                tt = VERBPAST;

            else if(w == "masendeshita")
                tt = VERBPASTNEG;

            else if(w == "desu")
                tt = IS;

            else if(w == "deshita")
                tt = WAS;

            else if(w == "o")
                tt = OBJECT;

            else if(w == "wa")
                tt = SUBJECT;

            else if(w == "ni")
                tt = DESTINATION;

            else if(w == "watashi" || w == "anata" || w == "kare" || w == "sore")
                tt = PRONOUN;

            else if(w == "mata" || w == "soshite" || w == "shikashi" || w == "dakara")
                tt = CONNECTOR;

            else
                cout << "ERROR: COULDNT FIND TT FOR RESERVED WORD" << endl;

	        return 1;
        }
    }
    
    char end = w[w.length() - 1]; // to get the last char in w
    
    if(end == 'a' || end == 'e' || end == 'i' || end == 'o' || end == 'u' || end == 'n') //if the last char in the word is a lowercase vowel
      tt = WORD1;
    else if(end == 'I' || end == 'E') //if the last character is I or E
      tt = WORD2;
    else // if neither its an error
      tt = ERROR;	
  }

  /*
  4. Return the token type & string  (pass by reference)
  */
  return 1; // exit the function
}//the end of scanner



// The temporary test driver to just call the scanner repeatedly  
// This will go away after this assignment
// DO NOT CHANGE THIS!!!!!! 
// Done by:  Louis
// int main()
// {
//   tokentype thetype;
//   string theword; 
//   string filename;

//   cout << "Enter the input file name: ";
//   cin >> filename;

//   fin.open(filename.c_str());

//   // the loop continues until eofm is returned.
//    while (true)
//     {
//        scanner(thetype, theword);  // call the scanner which sets
//                                    // the arguments  
//        if (theword == "eofm") break;  // stop now

//        cout << "Type is:" << tokenName[thetype] << endl;
//        cout << "Word is:" << theword << endl;   
//     }

//    cout << "End of file is encountered." << endl;
//    fin.close();

// }// end


// HELPER FUNCTIONS ---------------------------------

// inputs: a character.
// outputs: a bool value.
// A helper function to determine if the given token is a vowel. Returns true if a vowel.
// Used in the DFA function IsWordtype() for states: q0, qy, qt, qsa, qs, q0q1, q0qy.
bool IsVowel(char token)
{
    // All possible vowels in the language.
    string vowels = "aiueoIE";

    // Return true of false if exists in set of vowels.
    return vowels.find(token) != string::npos;
}

// Matthew - I was not necessarily sure what to call these functions.
// I am not super happy about their names, they are longer than I'd like.
// If someone has a better name, feel free to change it, or let me know and I can do it.

// inputs: a character.
// outputs: a bool value.
// A helper function to determine if the given token is part of the constant pair starter set.
// Used in the DFA function IsWordtype() for states: q0, q0qy.
bool IsConstantPairStarter(char token)
{
    string vowels = "bmknhrpg";
    return vowels.find(token) != string::npos;
}

// inputs: a character.
// outputs: a bool value.
// A helper function that is a variant of the above function, it includes no character 'n' in the matching search.
// Used in the DFA function IsWordtype() for states: q0q1.
bool IsConstantPairStarterVariant(char token)
{
    string vowels = "bmkhrpg";
    return vowels.find(token) != string::npos;
}

// inputs: a character.
// outputs: a bool value.
// A helper function to determine if the given token is part of the constant not pair starter set.
// Used in the DFA function IsWordtype() for states: q0, qy, q0q1, q0qy.
bool IsConstantNotPairStarter(char token)
{
    string vowels = "dwzyj";
    return vowels.find(token) != string::npos;
}
