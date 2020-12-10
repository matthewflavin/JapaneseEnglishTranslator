#include<iostream>
#include<fstream>
#include<string>
#include <stdlib.h>
#include "group10scanner.cpp"
using namespace std;

// Function Prototypes
void S();
void AS();
void AN();
void AO();
void noun();
void verb();
void be();
void tense();
void trace(int trace_type, string name);

// Tracing Flag
bool tracingOn;

// ** Need the updated match and next_token with 2 global vars
// saved_token and saved_lexeme

// Global buffer for scanner return
tokentype saved_token;

// Flag indicating if token from scanner is saved
bool token_available = false;

// Save the lexeme globally
string saved_lexeme;

/* INSTRUCTION:  Complete all ** parts.
   You may use any method to connect this file to scanner.cpp
   that you had written.  
  e.g. You can copy scanner.cpp here by:
          cp ../ScannerFiles/scanner.cpp .  
       and then append the two files into one: 
          cat scanner.cpp parser.cpp > myparser.cpp
*/

//=================================================
// File parser.cpp written by Group Number: **
//=================================================

// ----- Four Utility Functions and Globals -----------------------------------

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.  

// Type of error: * Match Failure *
// Done by: * Jackson Koshley * 
void syntaxerror1(tokentype tt)
{
   cout << "SYNTAX ERROR: expected " << tokenName[tt] << " but found " << saved_lexeme << endl;
   exit(1); 
}

// Type of error: * Switch default, no matching token type. *
// Done by: * Jackson Koshley * 
void syntaxerror2(string pf)
{
   cout << "SYNTAX ERROR: unexpected " << saved_lexeme <<  " found in " << pf << endl; 
   exit(1);
}


// Purpose: *Looks ahead and saves the token in order to check for match()*
// Done by: *Matthew Flavin*
tokentype next_token()
{
   if (!token_available)
   {
      scanner(saved_token, saved_lexeme);
      cout << "Scanner Called Using Word: " << saved_lexeme << endl;
      token_available = true;

      if (saved_token == ERROR)
      {
         syntaxerror1(saved_token);
      }
   }

   return saved_token;
}

// Purpose: *Checks the current token against the expected and eats it if matched*
// Done by: *Matthew Flavin*
bool match(tokentype expected)
{
   if (next_token() != expected)
   {
      syntaxerror2(tokenName[expected]);
   }

   else
   {
      // Set flag
      token_available = false;

      // Print trace as required
      trace(0, tokenName[expected]);

      return true;
   }
}

// ----- RDP functions - one per non-term -------------------

// ** Make each non-terminal into a function here
// ** Be sure to put the corresponding grammar rule above each function
// ** Be sure to put the name of the programmer above each function

// Grammar: *<Story> ::= <S> [S]*
// Done by: * Everyone *
void Story()
{
   trace(1, "Story");

   S();

   while (true)
   {
      switch(next_token())
      {
         case CONNECTOR:
         case WORD1:
         case PRONOUN:
            S();
            break;

         default:
            return;
      }
   }
}

// Grammar: *<S> ::= [CONNECTOR] <noun> SUBJECT <AS>*
// Done by: * Everyone *
void S()
{
   // Print trace as required
   trace(1, "S");

   // Optional [CONNECTOR]
   if (next_token() == CONNECTOR)
   {
      match(CONNECTOR);
   }

   // <noun>
   noun();

   // SUBJECT
   match(SUBJECT);

   // After subject
   AS();
}

// After Subject
// Grammar: *<AS> ::= <verb> <tense> PERIOD | <noun> <AN>*
// Done by: * Everyone *
void AS()
{
   // Print trace as required
   trace(1, "AS");
   
   // Switch for |
   switch(next_token())
   {
      // Check if it starts <verb>
      case WORD2:
         cout << "ENTER WORD2" << endl;
         verb();
         tense();
         match(PERIOD);
         break;

      // Check if it starts <noun>
      case WORD1:
      case PRONOUN:
         cout << "ENTER WORD1/PRONOUN" << endl;
         noun();
         AN();
         break;

      // Error
      default:
         syntaxerror2("After Noun");
   }
}

// After Noun
// Grammar: <AN> ::= <be> PERIOD | DESTINATION <verb> <tense> PERIOD | OBJECT <AO>
// Done By: Everyone
void AN()
{
   // Print trace as required
   trace(1, "AN");

   switch(next_token())
   {
      case IS:
      case WAS:
         be();
         match(PERIOD);
         break;

      case DESTINATION:
         match(DESTINATION);
         verb();
         tense();
         match(PERIOD);
         break;

      case OBJECT:
         match(OBJECT);
         AO();
         break;

      default:
         syntaxerror2("After Noun");
   }
}

// After Object
// Grammar: <AO> ::= <verb> <tense> PERIOD | <noun> DESTINATION <verb> <tense> PERIOD
// Done By: Everyone
void AO()
{
   // Print trace as required
   trace(1, "AO");

   switch(next_token())
   {
      case WORD2:
         verb();
         tense();
         match(PERIOD);
         break;

      case WORD1:
      case PRONOUN:
         noun();
         match(DESTINATION);
         verb();
         tense();
         match(PERIOD);
         break;

      default:
         syntaxerror2("After Object");
   }
}

// Grammar: *<noun> ::= WORD1 | PRONOUN*
// Done by: * Everyone *
void noun()
{
   // Print trace as required
   trace(1, "noun");

   switch(next_token())
   {
      case WORD1:
         match(WORD1);
         break;

      case PRONOUN:
         match(PRONOUN);
         break;

      default:
         syntaxerror2("NOUN");
   }
}

// Grammar: *<verb> ::= WORD2*
// Done by: * Everyone *
void verb()
{
   // Print trace as required
   trace(1, "verb");

   switch(next_token())
   {
      case WORD2:
         match(WORD2);
         break;

      default:
         syntaxerror2("Verb");
   }
}

// Grammar: *<be> ::= IS | WAS*
// Done by: * Everyone *
void be()
{
   // Print trace as required
   trace(1, "be");

   switch(next_token())
   {
      case IS:
         match(IS);
         break;

      case WAS:
         match(WAS);
         break;

      default:
         syntaxerror2("Be");
   }
}

// Grammar: *<tense> ::= VERBPAST | VERBPASTNEG | VERB | VERBNEG*
// Done by: * Everyone *
void tense()
{
   // Print trace as required
   trace(1, "tense");

   switch(next_token())
   {
      case VERBPAST:
         match(VERBPAST);
         break;

      case VERBPASTNEG:
         match(VERBPASTNEG);
         break;

      case VERB:
         match(VERB);
         break;

      case VERBNEG:
         match(VERBNEG);
         break;

      default:
         syntaxerror2("Tense");
   }
}

string filename;

//----------- Driver ---------------------------

// The new test driver to start the parser
// Done by:  * Jesse Cox *
int main()
{
   // Tracing flag, set to false if you do not wish to see tracing in the program
   tracingOn = true;

   // Get input for file name
   cout << "Enter the input file name: ";
   cin >> filename;
   fin.open(filename.c_str());

   // check if EOFM
   if (saved_token == EOFM)
   {
      cout << "EXITING PROGRAM" << endl;
      return 0;
   }

   // Call Story
   Story();

   // Close File
   fin.close();
}

// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions

// Function to trace program
// trace_type == 0, then it is a match trace
// trace_type == 1, then it is a nonterminal process
// string name is what to print, e.g. matched token or processed nonterminal name
void trace(int trace_type, string name)
{
   // If flag is on
   if (tracingOn)
   {
      // match() trace
      if (trace_type == 0)
      {
         cout << "Matched " << name << endl;
      }

      // non_terminal() trace
      else if (trace_type == 1)
      {
         cout << "Processing <" << name << ">... " << endl;
      }

      // bad argument
      else
      {
         cout << "Invalid Call of trace()" << endl;
      }
   }
}