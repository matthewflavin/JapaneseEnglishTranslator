// CS 421 Project Part A - The Scanner
// Group Members: Matthew Flavin, Jesse Cox, Jackson Koshley
// Members A and C: Jesse Cox and Jackson Koshley
// Member B: Matthew Flavin

// Include libraries
#include <iostream>
#include <string>

// Namespaces
using namespace std;

// Function Prototypes
string IsWordType(string word);
string IsReserved(string word);
bool IsPeriod(string word);
bool IsVowel(char token);
bool IsConstantPairStarter(char token);
bool IsConstantPairStarterVariant(char token);
bool IsConstantNotPairStarter(char token);

// MAIN()
// DRIVER CODE GOES HERE
int main()
{
    // Matthew -
    // This is the code I was using to test the functions.
    // It does not match his requirements, I don't think.
    // Need proper driver code. 

    string testInput = "";
    string wordType = "";

    cout << "DRIVER CODE TEST, ENTER stop TO STOP PROGRAM.";

    while (testInput != "stop")
    {
        cout << endl << "ENTER WORD: ";
        cin >> testInput;
        cout << endl;

        if (testInput == "stop")
        {
            break;
        }

        else
        {
            wordType = IsReserved(testInput);

            if (wordType == "NONE")
            {
                if (IsPeriod(testInput))
                {
                    wordType = "PERIOD";
                }

                else
                {
                    wordType = IsWordType(testInput);
                }     
            }

            cout << testInput << " : " << wordType << endl;
        }
    }

    return 0;
}

// inputs: a string.
// outputs: 3 possible strings, "WORD1", "WORD2", "LEXICAL ERROR".
// This function is modeled after the DFA of the NFA given.
// It iterates throughout a word, following states based on the given character.
// An enum named wordType is created to keep track of the current states.
string IsWordType(string word)
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
        for (int i = 0; i < word.size(); i++)
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
                
                else if (IsConstantPairStarter(currentToken))
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
                    currentState = q0q1;

                else if (currentToken == 'y')
                    currentState = qsa;

                else if (IsConstantNotPairStarter(currentToken))
                    currentState = q1;

                else
                    currentState = ERROR;
            }

            // TRANSITION STATE: qt ---------------------------------------
            else if (currentState == qt)
            {
                if (IsVowel(currentToken))
                    currentState = q0q1;

                else if (currentToken = 's')
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
            else if (currentState = q0q1)
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

                else if (IsConstantPairStarter(currentToken))
                    currentState = qy;

                else if (IsConstantNotPairStarter(currentToken))
                    currentState = qsa;

                else if (currentToken = 'y')
                    currentState = qsa;

                else
                    currentState = ERROR;
            }

            // The DFA could not continue, lexical error. Break loop.
            else if (currentState = ERROR)
            {
                break;
            }

        } // END OF FOR LOOP.

        // Check that the loop ended in a final state. (q0, q0q1, q0qy)
        if (currentState == q0 || currentState == q0q1 || currentState == q0qy)
        {
            // Get the last character of the input string and store it in finalChar.
            char finalChar = word[word.size() - 1];

            // If final character of string is capital I or E, it is type WORD2.
            if (finalChar == 'I' || finalChar == 'E')
                return "WORD2";

            // Else, it is type WORD1
            else
                return "WORD1";
        }

        // Return Error Statement.
        else
        {
            return "LEXICAL ERROR";
        }
    }
}

// inputs: a string.
// outputs: a bool value.
// This function checks whether the given string consists of only a period character. If it does, return true.
bool IsPeriod(string word)
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

// inputs: a string.
// outputs: a string.
// This function checks the input word against the reserved words of the language.
// If a match is found, the function returns the matching type as a string.
// If no match is found, "NONE" is returned.
string IsReserved(string word)
{

    // Verb Marker Checks -----
    if (word == "masu")
        return "VERB";

    else if (word == "masen")
        return "VERBNEG";

    else if (word == "mashita")
        return "VERBPAST";

    else if (word == "masendeshita")
        return "VERBPASTNEG";

    else if (word == "desu")
        return "IS";

    else if (word == "deshita")
        return "WAS";


    // Particles Checks -----
    else if (word == "o")
        return "OBJECT";

    else if (word == "wa")
        return "SUBJECT";

    else if (word == "ni")
        return "DESTINATION";


    // Prounouns and connectors check -----
    else if (word == "watashi" || word == "anata" || word == "kare" || word == "kanojo" || word == "sore")
        return "PRONOUN";

    else if (word == "mata" || word == "soshite" || word == "shikashi" || word == "dakara")
        return "CONNECTOR";


    // If none are matching, return none. -----
    else
        return "NONE";
}

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