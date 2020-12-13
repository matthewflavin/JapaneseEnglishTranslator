#include<iostream>
#include<fstream>
#include<string>
#include <stdlib.h>
#include <vector>
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
void ClearFile(string file_name);

// Tracing Flag
bool tracingOn;

// Global buffer for scanner return
tokentype saved_token;

// Flag indicating if token from scanner is saved
bool token_available = false;

// Save the lexeme globally
string saved_lexeme = "";

// Save the english word globally
string saved_E_word = "";

//=================================================
// File translator.cpp written by Group Number: * 10 *
//=================================================

// ----- Additions to the parser.cpp ---------------------

// ** Declare Lexicon (i.e. dictionary) that will hold the content of lexicon.txt
// Make sure it is easy and fast to look up the translation.
// Do not change the format or content of lexicon.txt 
//  Done by: * Jesse Cox * 

// Structure of Japanese Word and its english version
struct word_pair
{
   string english_word;
   string japanese_word;
};

// Vector to hold word pairs
vector<word_pair> dictionary;

// ** Additions to parser.cpp here:
//    getEword() - using the current saved_lexeme, look up the English word
//                 in Lexicon if it is there -- save the result   
//                 in saved_E_word
//  Done by: * Jackson Koshley * 
void getEword()
{
   // loop through dictionary in search of saved lexeme
   for (size_t i = 0; i < dictionary.size(); i++)
   {
      // if found, set saved_E_word to the pair english word
      if(dictionary[i].japanese_word == saved_lexeme)
      {
         saved_E_word = dictionary[i].english_word;
         return;
      }    
   }

   // else, no english equivalent, just set as japanese word
   saved_E_word = saved_lexeme;
}

//    gen(line_type) - using the line type,
//                     sends a line of an IR to translated.txt
//                     (saved_E_word or saved_token is used)
//  Done by: * Matthew Flavin*
void gen(string line_type)
{
   // file stream objects
   ofstream file_output;
   ifstream file_input;

   string output_line = "";

   // open input and output files
   file_input.open("translated.txt");
   file_output.open("translated.txt", ios::app);

   // if input is open
   if (file_input.is_open())
   {
      // if given line type is tense
      if (line_type == "TENSE")
      {

         // string to hold the type of tense
         tokentype tense_type_token;
         string tense_type_string;

         // if cases to figure out tense type based off saved e word
         // tense japanese is not translated as far as I am aware
         if(saved_lexeme == "masu")
               tense_type_string = "VERB";

         else if(saved_lexeme == "masen")
               tense_type_string = "VERBNEG";

         else if(saved_lexeme == "mashita")
               tense_type_string = "VERBPAST";

         else if(saved_lexeme == "masendeshita")
               tense_type_string = "VERBPASTNEG";

         else if(saved_lexeme == "desu")
               tense_type_string = "IS";

         else if(saved_lexeme == "deshita")
               tense_type_string = "WAS";

         else
            tense_type_string = "COULD NOT FIND TENSE TYPE";

         // output tense
         output_line = line_type + ": " + tense_type_string + "\n";
      }

      // else if not tense
      else
      {
         // output type and translated word
         output_line = line_type + ": " + saved_E_word + "\n";
      }
   }  

   // Print output
   // cout << output_line;

   // Output file
   file_output << output_line;

   // close file stream objects
   file_input.close();
   file_output.close();
}



// ----- Four Utility Functions and Globals -----------------------------------

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.  


// PARSER ==============

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
      ofstream file_output;
      file_output.open("translated.txt", ios::app);
      file_output << "\n";
      file_output.close();

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
      getEword();
      gen("CONNECTOR");
   }

   // <noun>
   noun();

   getEword();

   // SUBJECT
   match(SUBJECT);

   gen("ACTOR");

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
         verb();

         getEword();
         gen("ACTION");

         tense();
         gen("TENSE");

         match(PERIOD);
         break;

      // Check if it starts <noun>
      case WORD1:
      case PRONOUN:
         noun();
         getEword();
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
         gen("DESCRIPTION");
         gen("TENSE");
         // cout << "\n " << saved_lexeme << "\n";
         match(PERIOD);
         break;

      case DESTINATION:
         match(DESTINATION);
         gen("TO");
         verb();
         getEword();
         gen("ACTION");
         tense();
         gen("TENSE");
         match(PERIOD);
         break;

      case OBJECT:
         match(OBJECT);
         gen("OBJECT");
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
         getEword();
         gen("ACTION");
         tense();
         gen("TENSE");
         match(PERIOD);
         break;

      case WORD1:
      case PRONOUN:
         noun();
         getEword();
         match(DESTINATION);
         gen("TO");
         verb();
         getEword();
         gen("ACTION");
         tense();
         gen("TENSE");
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

// ---------------- Driver ---------------------------

// compiled with
// g++ group10translator.cpp -W -std=c++11

// The final test driver to start the translator
// Done by:  * Jackson Koshley*
int main()
{
   // Tracing flag, set to false if you do not wish to see tracing in the program
   tracingOn = true;

   // clear translator.txt on start of new instance
   ClearFile("translated.txt");

   // Strings to hold the japanese and english words while storing
   string j_word, e_word;

   //** opens the lexicon.txt file and reads it into Lexicon
   ifstream lexicon_file;
   lexicon_file.open("lexicon.txt");

   while (lexicon_file >> j_word)
   {
      // Get E word
      lexicon_file >> e_word;

      // Create struct tmp
      word_pair tmp;

      // Set variables
      tmp.english_word = e_word;
      tmp.japanese_word = j_word;

      // Add to dictionary
      dictionary.push_back(tmp);
   }

   //** closes lexicon.txt 
   lexicon_file.close();

   // Get input for file name
   cout << "Enter the input file name: ";
   cin >> filename;
   fin.open(filename.c_str());

   // Call Story
   Story();

   // check if EOFM
   if (saved_token == EOFM)
   {
      cout << "EXITING PROGRAM" << endl;
      return 0;
   }

   // Close File
   fin.close();
}

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

// https://stackoverflow.com/a/17033060
// function to clear file
void ClearFile(string file_name)
{
   ofstream ofs;
   ofs.open(file_name, std::ofstream::out | std::ofstream::trunc);
   ofs.close();
}