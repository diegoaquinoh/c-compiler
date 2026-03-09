#include "CodeGenVisitor.h"
using namespace std;

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    #ifdef __APPLE__
    cout << "    .globl _main\n";
    cout << "_main:\n";
    #else
    cout << "    .globl main\n";
    cout << "main:\n";
    #endif

    // Prologue
    cout << "    pushq %rbp\n";
    cout << "    movq %rsp, %rbp\n";

    // Initialize implicit return value slot at -4(%rbp)
    cout << "    movl $0, -4(%rbp)\n";

    // Visit all statements
    for (auto *stmt : ctx->stmt()) {
        this->visit(stmt);
    }

    // Visit return statement
    this->visit(ctx->return_stmt());

    // Epilogue
    cout << "    popq %rbp\n";
    cout << "    retq\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    for (auto *item : ctx->decl_item()) {
        this->visit(item);
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDecl_item(ifccParser::Decl_itemContext *ctx)
{
    string varName = ctx->VAR()->getText();
    int offset = symbolTable[varName];

    if (ctx->expr()) {
        if (ctx->expr()->CONST()) {
            int val = stoi(ctx->expr()->CONST()->getText());
            cout << "    movl $" << val << ", " << offset << "(%rbp)\n";
        } else {
            int offsetSrc = symbolTable[ctx->expr()->VAR()->getText()];
            cout << "    movl " << offsetSrc << "(%rbp), %eax\n";
            cout << "    movl %eax, " << offset << "(%rbp)\n";
        }
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAffect_stmt(ifccParser::Affect_stmtContext *ctx)
{
    string varName = ctx->VAR()->getText();
    int offset = symbolTable[varName];
    
    if (ctx->expr()) {
        int val = stoi(ctx->expr()->CONST()->getText());
        cout << "    movl $" << val << ", " << offset << "(%rbp)\n";
    } else {
        int offsetSrc = symbolTable[ctx->expr()->VAR()->getText()];
        cout << "    movl " << offsetSrc << "(%rbp), %eax\n";
        cout << "    movl %eax, " << offset << "(%rbp)\n";
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    if (ctx->expr()->CONST()) {
        int val = stoi(ctx->expr()->CONST()->getText());
        cout << "    movl $" << val << ", %eax\n";
    } else {
        int offset = symbolTable[ctx->expr()->VAR()->getText()];
        cout << "    movl " << offset << "(%rbp), %eax\n";
    }
    return 0;
}


// blabla jeMultiplie(ifccParser::Return_stmtContext *ctx)
// {
//     if (ctx->expr(0)->CONST()) {
//         if(ctx->expr(1)->CONST()) {
//             int val1 = stoi(ctx->expr(0)->CONST()->getText());
//             int val2 = stoi(ctx->expr(1)->CONST()->getText());
//             int result = val1 * val2;
//             cout << "    movl $" << result << ", %eax\n";
//             return 0;
//         }
//         if(ctx->expr(1)->VAR()) {
//             int val1 = stoi(ctx->expr(0)->CONST()->getText());
//             int offset2 = symbolTable[ctx->expr(1)->VAR()->getText()];
//             cout << "    movl " << offset2 << "(%rbp), %ebx\n";
//             cout << "    imull %ebx, $" << val1 << "\n";
//             cout << "    movl %eax, %eax\n"; // Move result to %eax
//             return 0;
//         }
//     }
//     else if(ctx->expr(0)->VAR()) {
//         if(ctx->expr(1)->CONST()) {
//             int offset1 = symbolTable[ctx->expr(0)->VAR()->getText()];
//             int val2 = stoi(ctx->expr(1)->CONST()->getText());
//             cout << "    movl " << offset1 << "(%rbp), %ebx\n";
//             cout << "    imull $" << val2 << ", %ebx\n";
//             cout << "    movl %ebx, %eax\n"; // Move result to %eax
//             return 0;
//         }
//         if(ctx->expr(1)->VAR()) {
//             int offset1 = symbolTable[ctx->expr(0)->VAR()->getText()];
//             int offset2 = symbolTable[ctx->expr(1)->VAR()->getText()];
//             cout << "    movl " << offset1 << "(%rbp), %ebx\n";
//             cout << "    movl " << offset2 << "(%rbp), %ecx\n";
//             cout << "    imull %ecx, %ebx\n";
//             cout << "    movl %ebx, %eax\n"; // Move result to %eax
//             return 0;
//         }
//     }


//     if (ctx->expr()->CONST()) {
//         int val = stoi(ctx->expr()->CONST()->getText());
//         cout << "    movl $" << val << ", %eax\n";
//     } else {
//         int offset = symbolTable[ctx->expr()->VAR()->getText()];
//         cout << "    movl " << offset << "(%rbp), %eax\n";
//     }
//     return 0;
// }