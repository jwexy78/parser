#define CATCH_CONFIG_MAIN

#include "catch.h"

#include "StringEscaper.h"
#include "Tokenizer.h"

TEST_CASE("Escaper")
{
    std::string escaped = StringEscaper::escape("a,b,c", ',', 'c');
    REQUIRE(escaped == "a\\cb\\cc");
}

TEST_CASE("Tokenizer")
{
    SECTION("One Operator")
    {
        std::vector<Tokenizer::Token> tokens = Tokenizer::tokenize("+");
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0] == Tokenizer::Token::Type::OPERATOR);
        REQUIRE(tokens[0] == "+");
    }
    SECTION("One Operator - Preceding Whitespace")
    {
        std::vector<Tokenizer::Token> tokens = Tokenizer::tokenize(" \t\n+");
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0] == Tokenizer::Token::Type::OPERATOR);
        REQUIRE(tokens[0] == "+");
        REQUIRE(tokens[0].raw == " \t\n+");
    }
    SECTION("One Operator - Surrounding Whitespace")
    {
        std::vector<Tokenizer::Token> tokens = Tokenizer::tokenize(" \t\n+ \n");
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0] == Tokenizer::Token::Type::OPERATOR);
        REQUIRE(tokens[0] == "+");
        REQUIRE(tokens[0].raw == " \t\n+ \n");
    }
    SECTION("Multiple Operators")
    {
        std::vector<Tokenizer::Token> tokens = Tokenizer::tokenize(" *-(% ");
        REQUIRE(tokens.size() == 4);
        REQUIRE(tokens[0] == Tokenizer::Token::Type::OPERATOR);
        REQUIRE(tokens[0] == "*");
        REQUIRE(tokens[0].raw == " *");
        REQUIRE(tokens[1] == Tokenizer::Token::Type::OPERATOR);
        REQUIRE(tokens[1] == "-");
        REQUIRE(tokens[1].raw == "-");
        REQUIRE(tokens[2] == Tokenizer::Token::Type::OPERATOR);
        REQUIRE(tokens[2] == "(");
        REQUIRE(tokens[2].raw == "(");
        REQUIRE(tokens[3] == Tokenizer::Token::Type::OPERATOR);
        REQUIRE(tokens[3] == "%");
        REQUIRE(tokens[3].raw == "% ");
    }
}

TEST_CASE("String and Number Tokenization")
{
    std::vector<Tokenizer::Token> tokens = Tokenizer::tokenize(
            "Str a1 def0 1234.56a");
    REQUIRE(tokens.size() == 7);
    REQUIRE(tokens[0] == "Str");
    REQUIRE(tokens[0] == Tokenizer::Token::Type::STRING);
    REQUIRE(tokens[1] == "a1");
    REQUIRE(tokens[1] == Tokenizer::Token::Type::STRING);
    REQUIRE(tokens[1].raw == " a1");
    REQUIRE(tokens[2] == "def0");
    REQUIRE(tokens[2] == Tokenizer::Token::Type::STRING);
    REQUIRE(tokens[3] == "1234");
    REQUIRE(tokens[3] == Tokenizer::Token::Type::NUMERAL);
    REQUIRE(tokens[3].raw == " 1234");
    REQUIRE(tokens[4] == ".");
    REQUIRE(tokens[5] == "56");
    REQUIRE(tokens[5] == Tokenizer::Token::Type::NUMERAL);
    REQUIRE(tokens[6] == "a");
    REQUIRE(tokens[6] == Tokenizer::Token::Type::STRING);
}

