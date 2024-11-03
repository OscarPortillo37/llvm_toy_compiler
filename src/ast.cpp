// TODO: Move to header, add in Excel extensions, perform rigorous stress testing/error checking
#include <memory>
#include <string>
#include <vector>


/********************************************* Abstract Syntax Tree (AST) *********************************************/
// Base class Abstract Syntax Tree for all expression nodes
class ExprAST {
    public:
        virtual ~ExprAST() = default;
};

// Double Vals AST class
class DoubleValExprAST : public ExprAST {
    private:
        double val; // private by default, but made private for clarity

    public:
        DoubleValExprAST(double val) : val(val) {};
};

// Variable Names AST class
class VarNameExprAST : public ExprAST {
    private:
        std::string varName;  // private by default, but made private for clarity

    public:
        VarNameExprAST(const std::string& varName) : varName(varName) {};

};

// Binary Operator AST class
class BinOperAST : public ExprAST {
    private:
        char op; // operator
        std::unique_ptr<ExprAST> left_op, right_op;

    public:
        BinOperAST(char op, std::unique_ptr<ExprAST> left_op, std::unique_ptr<ExprAST>) :   // params
            op(op), left_op(std::move(left_op)), right_op(std::move(right_op)) {};          // args
};

// NOTE: Would, in particular, require significant additions for more data types
// Function Call AST class
class CallExprAST : public ExprAST {
    private:
        std::string callee;
        std::vector<std::unique_ptr<ExprAST>> args;

    public:
        CallExprAST(const std::string callee, std::vector<std::unique_ptr<ExprAST>> args) :   // params
            callee(callee), args(std::move(args)) {};                                       // args

};

// Function Prototype AST class (declaration)
class FuncDecAST : public ExprAST {
    private:
        std::string func_name;
        std::vector<std::unique_ptr<ExprAST>> args;

    public:
        FuncDecAST(const std::string func_name, std::vector<std::unique_ptr<ExprAST>> args) :   // params
            func_name(func_name), args(std::move(args)) {};                                 // args
        const std::string& getName() {return func_name;}
}; 

// Function Definition AST class (definition)
class FuncDefnAST : public ExprAST {
    private:
        std::unique_ptr<FuncDecAST> func_dec;
        std::unique_ptr<ExprAST> body;

    public:
        FuncDefnAST(std::unique_ptr<FuncDecAST> func_dec, std::unique_ptr<ExprAST> body) :   // params
            func_dec(std::move(func_dec)), body(std::move(body)) {};                         // args        
};