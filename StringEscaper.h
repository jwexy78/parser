#pragma once

class StringEscaper
{
  public:
    /**
     * Escape a string
     * @param toEscape the character to escape
     * @param escapeWith the character to replace the escaped char with
     * @param escapeChar the char to prepend an escaped char with
     */
    static std::string escape(const std::string_view& str,
                              char toEscape,
                              char escapeWith,
                              char escapeChar = '\\')
    {
        std::string output;
        for (const auto& c : str) {
            if (c == toEscape) {
                output += escapeChar;
                output += escapeWith;
            } else if (c == escapeChar) {
                output += escapeChar;
                output += escapeChar;
            } else {
                output += c;
            }
        }
        return output;
    }

    static std::string deescape(const std::string_view& str,
                                char toEscape,
                                char escapeWith,
                                char escapeChar = '\\')
    {
        std::string output;
        for (const auto& c : str) {
            if (c == toEscape) {
                output += escapeChar + escapeWith;
            } else if (c == escapeChar) {
                output += escapeChar + escapeChar;
            } else {
                output += c;
            }
        }
        return output;
    }
};