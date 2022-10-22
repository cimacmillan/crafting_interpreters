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

bool CPPLox::TokenScanner::match(char c) {
    if (this->isAtEnd()) return false;
    if (this->source[this->current] != c) return false;
    this->current++;
    return true;
}

char CPPLox::TokenScanner::peek() {
    if (this->isAtEnd()) return '\0';
    return this->source[this->current];
}

char CPPLox::TokenScanner::peekNext() {
    if (this->current + 1 >= this->source.size()) return '\0';
    return this->source[this->current + 1];
}

void CPPLox::TokenScanner::advanceString() {
    char nextChar;
    while ((nextChar = this->peek()) != '"' && !this->isAtEnd()) {
        if (nextChar == '\n') this->line++;
        this->advance();
    }
    if (this->isAtEnd()) {
        report(this->line, "Unterminated string");
    }
    this->advance();
    this->lockToken(TokenType::STRING);
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isAlphaNumeric(char c) {
    return isDigit(c) || isAlpha(c);
}

void CPPLox::TokenScanner::advanceNumber() {
    while (isDigit(this->peek())) this->advance();
    if (this->peek() == '.' && isDigit(this->peekNext())) {
        advance();
        while (isDigit(this->peek())) this->advance();
    }
    this->lockToken(TokenType::NUMBER);
}

void CPPLox::TokenScanner::advanceIdentifier() {
    while (isAlphaNumeric(this->peek())) advance();

    std::string lexeme = this->source.substr(this->start, this->current - this->start);
    std::unordered_map<std::string, TokenType> keyword_map = {
        { "and", TokenType::AND },
        { "else", TokenType::ELSE },
        { "class", TokenType::CLASS },
        { "false", TokenType::FALSE },
        { "fun", TokenType::FUN },
        { "for", TokenType::FOR },
        { "if", TokenType::IF },
        { "nil", TokenType::NIL },
        { "or", TokenType::OR },
        { "print", TokenType::PRINT },
        { "return", TokenType::RETURN },
        { "super", TokenType::SUPER },
        { "this", TokenType::THIS },
        { "true", TokenType::TRUE },
        { "var", TokenType::VAR },
        { "while", TokenType::WHILE }
    };

    if (keyword_map.find(lexeme) != keyword_map.end()) {
        auto entry = keyword_map.find(lexeme);
        this->lockToken(entry->second);
    } else {
        this->lockToken(TokenType::IDENTIFIER);
    }
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
    // Get rid of brackets from string
    if (type == +TokenType::STRING) {
        lexeme = lexeme.substr(1, lexeme.size() - 2);
    }
    this->tokens.push_back((Token){ lexeme, type, this->line});
}

void CPPLox::TokenScanner::scanToken() {
    char symbol = this->advance();
    switch (symbol) {
        case ' ': 
        case '\t':
        case '\r':
            break;
        case '\n': this->line++; break;
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
        case '!': 
            this->match('=') ? this->lockToken(TokenType::BANG_EQUAL) : this->lockToken(TokenType::BANG); 
            break;
        case '=': 
            this->match('=') ? this->lockToken(TokenType::EQUAL_EQUAL) : this->lockToken(TokenType::EQUAL); 
            break;
        case '<': 
            this->match('=') ? this->lockToken(TokenType::LESS_EQUAL) : this->lockToken(TokenType::LESS); 
            break;
        case '>': 
            this->match('=') ? this->lockToken(TokenType::GREATER_EQUAL) : this->lockToken(TokenType::GREATER); 
            break;
        case '/':
            if (this->match('/')) {
                while(this->peek() != '\n' && !this->isAtEnd()) this->advance();
            } else {
                this->lockToken(TokenType::SLASH);
            }
            break;
        case '"': this->advanceString(); break;
        default:
            if (isDigit(symbol)) {
                this->advanceNumber();
                break;
            }

            if (isAlpha(symbol)) {
                this->advanceIdentifier();
                break;
            }
            std::string message = "Unknown symbol '";
            message.push_back(symbol);
            message.append("' when parsing '");
            message.append(this->source.substr(this->start, this->current - this->start));
            message.append("'");
            report(this->line, message);
        break;
    }
}