// TODO: Move to header, add in Excel extensions, perform rigorous stress testing/error checking
#include <memory>
#include "lexer.cpp"
#include "ast.cpp"
/************************************************** Parser Helpers **************************************************/

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
static int CurTok;
static int getNextTok() { return CurTok = gettok(); }

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<FuncDefnAST> LogError(const char* error_log) {
    fprintf(stderr, "Error Msg: %s", error_log);
    return nullptr;
}
std::unique_ptr<FuncDecAST> LogErrorDec(const char* error_log) {
    LogError(error_log);
    return nullptr;
}


/************************************************** Parser Code **************************************************/
/*****************************************************************************************************************/
/*****************************************************************************************************************/

                   /************************* Basic Expression Parsing ***************************/

// double vals --> double
static std::unique_ptr<ExprAST> ParseDoubleExpr() {
    auto DoubleValASTptr = std::make_unique<DoubleValExprAST>(NumVal);
    getNextTok();   // consume the double 
    return DoubleValASTptr;
}

// parentheses --> ()
static std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextTok(); // consume '('
    auto V = ParseExpression();

    // errors
    if(!V) return nullptr;
    if(CurTok != ')') {
        return LogError("expected ')'");
    }

    getNextTok(); // consume ')'
    return V;
}

// identifiers --> varName, funcCall
static std::unique_ptr<ExprAST> ParseIdentExpr() {
    std::string ident = IdentifierStr;
    getNextTok();
    
    // Normal identifier
    if(CurTok != '(') {
        return std::make_unique<VarNameExprAST>(ident);
    }

    getNextTok; // consume '('

    // Function Call
    if(CurTok == '(') {
        std::vector<std::unique_ptr<ExprAST>> args;
        while(true) {
            if(auto arg = ParseExpression()) {
                args.push_back(arg);
            } else {
                return nullptr; // If nullptr (0), propagate the error with the expression
            }

            // If end of functional call
            if(CurTok == ')') {
                break;
            }
            
            // If comma, do nothing (getNextTok() & skip ahead)
            if(CurTok == ',') { /*skip*/ }
            else { // Otherwise, an error has occurred
                return LogError("Expected ')' or ',' in argument list"); 
            }
            getNextTok(); // Consume ',' 
        }
        getNextTok(); // consume ')'

        return std::make_unique<CallExprAST>(ident, std::move(args));
    }
}

// primary 
//          ->  double val    
//          ->  parentheses
//          ->  identifiers (varName & funcCall) 
static std::unique_ptr<ExprAST> ParsePrimary() {
    switch(CurTok) {
        case tok_number:
            return ParseDoubleExpr();
        case tok_identifier:
            return ParseIdentExpr();
        case '{':
            return ParseParenExpr();
        default:
            return LogError("Unknown token when expecting expression");
    }
}


                   /************************* Binary Expression Parsing ***************************/
