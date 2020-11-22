#include<iostream>
#include<fstream>
#include<string>
#include "FinalScanner.cpp"
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

// Type of error: **
// Done by: ** 
void syntaxerror1()
{
   // Placeholder
   cout << "Error 1 " << endl;
}
// Type of error: **
// Done by: ** 
void syntaxerror2()
{
   // Placeholder
   cout << "Error 2" << endl;
}

// ** Need the updated match and next_token with 2 global vars
// saved_token and saved_lexeme

// Global buffer for scanner return
tokentype saved_token;

// Flag indicating if token from scanner is saved
bool token_available;

// Save the lexeme globally
string saved_lexeme;

// Purpose: *Looks ahead and saves the token in order to check for match()*
// Done by: *Matthew Flavin*
tokentype next_token()
{

   if (!token_available)
   {
      scanner(saved_token, saved_lexeme);
      token_available = true;

      if (saved_token == ERROR)
      {
         // Remove comment when syntaxerror is implemented
         //syntaxerror1(saved_lexeme, saved_token);
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
      // Figure out error thing here
      //syntaxerror2(saved_lexeme)
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

// Grammar: *<S> ::= [CONNECTOR] <noun> SUBJECT <AS>*
// Done by: * Matthew Flavin*
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
// Done by: * Matthew Flavin*
void AS()
{
   // Print trace as required
   trace(1, "AS");
   

   // Switch for |
   switch(next_token())
   {
      // Check if it starts <verb>
      case WORD2:
         verb();
         tense();
         match(PERIOD);
         break;

      // Check if it starts <noun>
      case WORD1:
      case PRONOUN:
         noun();
         AN();

      // Error
      default:
         syntaxerror1();
   }
}

// After Noun
// Grammar: <AN> ::= <be> PERIOD | DESTINATION <verb> <tense> PERIOD | OBJECT <AO>
// Done By: 
void AN()
{
   // Print trace as required
   trace(1, "AN");
}

// After Object
// Grammar: <AO> ::= <verb> <tense> PERIOD | <noun> DESTINATION <verb> <tense> PERIOD
// Done By: 
void AO()
{
   // Print trace as required
   trace(1, "AO");
}

// Grammar: *<noun> ::= WORD1 | PRONOUN*
// Done by: * Matthew Flavin*
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
         syntaxerror1();
   }
}

// Grammar: *<verb> ::= WORD2*
// Done by: *Matthew Flavin*
void verb()
{
   // Print trace as required
   trace(1, "verb");
}

// Grammar: *<be> ::= IS | WAS*
// Done by: **
void be()
{
   // Print trace as required
   trace(1, "be");
}

// Grammar: *<tense> ::= VERBPAST | VERBPASTNEG | VERB | VERBNEG*
// Done by: **
void tense()
{
   // Print trace as required
   trace(1, "tense");
}


string filename;

//----------- Driver ---------------------------

// The new test driver to start the parser
// Done by:  **
int main()
{
   // Tracing flag, set to false if you do not wish to see tracing in the program
   tracingOn = true;

   cout << "Enter the input file name: ";
   cin >> filename;
   fin.open(filename.c_str());

   // Placeholder test for "watashi"
   next_token();
   cout << "WORD TYPE: " << tokenName[saved_token] << endl;
   noun();

   //** calls the <story> to start parsing
   //** closes the input file 

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