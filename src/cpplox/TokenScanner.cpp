#include <vector>
#include <string>

#include "CPPLox.h"
#include "TokenScanner.h"


bool CPPLox::TokenScanner::isAtEnd() {
    return this->current >= this->source.size();
}

char CPPLox::TokenScanner::advance() {
    char value = this->source[this->current];
    this->current++;
    return value;
}

std::vector<Token> CPPLox::TokenScanner::scanTokens() {
    while(!this->isAtEnd()) {
        this->start = this->current;
        this->scanToken();
    }

    this->tokens.push_back((Token){ "", TokenType::FILE_END, this->line});
    return tokens;
}

void CPPLox::TokenScanner::lockToken(TokenType type) {
    std::string lexeme = this->source.substr(this->start, this->current - this->start);
    this->tokens.push_back((Token){ lexeme, type, this->line});
}

void CPPLox::TokenScanner::scanToken() {
    char symbol = this->advance();
    switch (symbol) {
        case '\n': this->line++; break;
        case ' ': break;
        case '(': this->lockToken(TokenType::LEFT_PAREN); break;
        case ')': this->lockToken(TokenType::RIGHT_PAREN); break;
        case '{': this->lockToken(TokenType::LEFT_BRACE); break;
        case '}': this->lockToken(TokenType::RIGHT_BRACE); break;
        case ',': this->lockToken(TokenType::COMMA); break;
        case '.': this->lockToken(TokenType::DOT); break;
        case '-': this->lockToken(TokenType::MINUS); break;
        case '+': this->lockToken(TokenType::PLUS); break;
        case ';': this->lockToken(TokenType::SEMICOLON); break;
        case '*': this->lockToken(TokenType::STAR); break; 
        default:
            std::string message = "Unknown symbol '";
            message.push_back(symbol);
            message.append("' when parsing '");
            message.append(this->source.substr(this->start, this->current - this->start));
            message.append("'");
            report(this->line, message);
        break;
    }
}