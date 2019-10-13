#pragma once

#include <vector>
#include <unordered_set>
#include "StringEscaper.h"

/// A class that Tokenizes a string into a series of tokens
/// This class is lossless - that is, all characters that
/// are in the input end up, somehow, in the output
class Tokenizer
{
private:
    static bool isWhitespaceChar(char c)
    {
        return c == '\t' || c == '\n' || c == ' ';
    }

    // If the character is 0 through 9
    static bool isNumeralChar(char c)
    {
        return c >= '0' && c <= '9';
    }

    // If the character is in the alphabet
    static bool isCharacterChar(char c)
    {
        return (c >= 'a' && c < 'z') || (c >= 'A' && c <= 'Z');
    }

    // If the character is a non-numeral non-alphabet
    // but acceptable character
    static bool isOperatorChar(char c)
    {
        return (c >= '!' && c <= '/') ||
               (c >= ':' && c <= '@') ||
               (c >= '[' && c <= '`') ||
               (c >= '{' && c <= '~');
    }

public:
    struct Token
    {
        enum class Type : char
        {
            NUMERAL = 'n',
            STRING = 's',
            OPERATOR = 'o',
            INVALID = 'I'
        };

        Token(Type _type, const std::string& _content, const std::string& _raw)
          : type(_type),
            content(_content),
            raw(_raw)
        {
        }

        Token(Type _type, const char* rawStart, const char* contentStart, const char*end)
          : type(_type),
            content(contentStart, end - contentStart),
            raw(rawStart, end - rawStart)
        {
        }

        bool operator==(const Token::Type& _type) const
        {
            return type == _type;
        }

        bool operator==(const std::string_view& _content) const
        {
            return content == _content;
        }

        Type type;
        std::string content;
        std::string raw;
    };

    static std::string toString(const std::vector<Token> tokens)
    {
        auto escape = [](const std::string_view& str) -> std::string {
            return StringEscaper::escape(
                    StringEscaper::escape(str, '\n', 'n'), '\t', 't');
        };
        std::string output;
        for (const auto& token : tokens) {
             std::ostringstream stringStream;
             stringStream << (char)token.type << '\t'
                          << escape(token.content) << '\t'
                          << escape(token.raw) << '\n';
            output += stringStream.str();
        }
        return output;
    }

    static std::vector<Token> fromString(const std::string& input)
    {
        std::vector<Token> tokens;
        std::istringstream lineStream(input);
        std::string tokenLine;
        while(std::getline(lineStream, tokenLine, '\n')) {
            std::istringstream tokenStream(tokenLine);
            std::string type;
            std::string content;
            std::string raw;
            tokenStream >> type >> content >> raw;
            tokens.emplace_back((Token::Type)type[0], content, raw);
        }
        return tokens;
    }

    static std::vector<Token> tokenize(const char* input)
    {
        std::vector<Token> output;
        const char* start = input;
        while (*start) {
            // Set content_start to the first non-whitespace char
            const char* content_start = start;
            while (*content_start && isWhitespaceChar(*content_start)) {
                content_start++;
            }
            const char* end = content_start + 1;
            Token::Type type;
            if (!*content_start) {
                // Reached the end of the string while
                // parsing whitespace.
                if (output.size()) {
                    // Append to the end of the last token's raw
                    Token& lastToken = output[output.size() - 1];
                    lastToken.raw = std::string_view(lastToken.raw.data(), lastToken.raw.size() + content_start - start);
                } else {
                    output.emplace_back(Token::Type::INVALID, start, content_start, content_start);
                }
                break;
            }
            if (isOperatorChar(*content_start)) {
                type = Token::Type::OPERATOR;
            } else if (isCharacterChar(*content_start)) {
                while (*end && (isCharacterChar(*end) || isNumeralChar(*end))) {
                    ++end;
                }
                type = Token::Type::STRING;
            } else if (isNumeralChar(*content_start)) {
                while (*end && isNumeralChar(*end)) {
                    ++end;
                }
                type = Token::Type::NUMERAL;
            } else {
                type = Token::Type::INVALID;
            }
            output.emplace_back(type, start, content_start, end); 
            start = end;
        }
        return output;
    }
};