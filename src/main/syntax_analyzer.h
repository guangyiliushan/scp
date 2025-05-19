// Header file for SCP Language Syntax Analyzer

#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

// Define the SyntaxAnalyzer structure
typedef struct {
    char* analyzed_syntax;
} SyntaxAnalyzer;

// Function prototypes
SyntaxAnalyzer* create_syntax_analyzer();
void analyze_syntax(SyntaxAnalyzer* analyzer, const char* tokens);
void destroy_syntax_analyzer(SyntaxAnalyzer* analyzer);

#endif // SYNTAX_ANALYZER_H