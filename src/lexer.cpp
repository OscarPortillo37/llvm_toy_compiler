// TODO: Move to header, add in Excel extensions, perform rigorous stress testing/error checking
#include <string>
#include <algorithm>

// The lexer returns tokens [0-255] if it's an unknown chararcter 
// (e.g. operators), otherwise one of the below.
enum Token {
    tok_eof = -1,
    
    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5,
};



// Local file variables used in gettok()
static std::string IdentifierStr;
static double NumVal;

// Lexing function -- gettok (returns tokens)
static int gettok() {
    static int LastChar = ' ';

    // whitespace
    while(std::isspace(LastChar)) LastChar = getchar();

    // identifier, 
    // EXTENSION: Added in the ability to have vars w/ _ (not just alphanumeric labels)
    if(std::isalpha(LastChar) || LastChar == '.') {
        IdentifierStr = LastChar;
        while(std::isalnum(LastChar = std::getchar()) || LastChar == '_') {
            IdentifierStr += LastChar;
        }

        if(IdentifierStr == "def") return tok_def;
        if(IdentifierStr == "extern") return tok_extern;
        return tok_identifier;
    }

    // EXTENSION: Added in checks for only one decimal point
    if(std::isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        NumStr += LastChar;
        while(std::isdigit(LastChar = std::getchar()) || LastChar == '.') {
            NumStr += LastChar;
        }
        
        // ERROR: Too many "." in the decimal
        if(std::count(NumStr.begin(), NumStr.end(), '.') > 1) return 0;

        NumVal = std::stod(NumStr);
        return tok_number;
    }

    // Comments
    if(LastChar == '#') {
        LastChar = std::getchar();
        while( !(LastChar == '\n' || LastChar == '\r' || LastChar == EOF) ) LastChar = std::getchar();
        if(LastChar != EOF) return gettok();
    }

    // End of File
    if(LastChar == EOF) return tok_eof;

    // Otherwise, just return the character as an ASCII value & move to the next char
    int ThisChar = LastChar;
    LastChar = std::getchar();
    return ThisChar;
}
