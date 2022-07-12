#include "lexer.h"

bool is_space(char c) noexcept {
  switch (c) {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      return true;
    default:
      return false;
  }
}

bool is_digit(char c) noexcept {
  switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return true;
    default:
      return false;
  }
}

bool is_identifier_char(char c) noexcept {
  switch (c) {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '_':
    case '@':
      return true;
    default:
      return false;
  }
}

Token Lexer::atom(Token::Kind kind) noexcept { return Token(kind, m_beg++, 1); }

Token Lexer::next() noexcept {
  while (is_space(peek())) get();

  switch (peek()) {
    case '\0':
      return Token(Token::Kind::End, m_beg, 1);
    default:
      return atom(Token::Kind::Unexpected);
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '@':
    case '~':
      return identifier();
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return number();
    case '(':
      return atom(Token::Kind::LeftParen);
    case ')':
      return atom(Token::Kind::RightParen);
    case '[':
      return atom(Token::Kind::LeftSquare);
    case ']':
      return atom(Token::Kind::RightSquare);
    case '{':
      return atom(Token::Kind::LeftCurly);
    case '}':
      return atom(Token::Kind::RightCurly);
    case '=':
      return equal_or_equals();
    case '+':
      return atom(Token::Kind::Plus);
    case '-':
      return atom(Token::Kind::Minus);
    case '*':
      return atom(Token::Kind::Asterisk);
    case '/':
      return slash_or_comment();
    case '#':
      return atom(Token::Kind::Hash);
    case '.':
      return atom(Token::Kind::Dot);
    case ',':
      return atom(Token::Kind::Comma);
    case ':':
      return atom(Token::Kind::Colon);
    case ';':
      return atom(Token::Kind::Semicolon);
    case '\'':
      return atom(Token::Kind::SingleQuote);
    case '"':
      return atom(Token::Kind::DoubleQuote);
    case '<':
    case '>':
        return math_or_binary_operator();
    case '|':
    case '&':
      return logical_or_binary_operator();
  }
}
Token Lexer::identifier() noexcept {
  const char* start = m_beg;
  get();
  while (is_identifier_char(peek())) get();
  return Token(Token::Kind::Identifier, start, m_beg);
}

Token Lexer::number() noexcept {
  const char* start = m_beg;
  get();
  while (is_digit(peek())) get();
  return Token(Token::Kind::Number, start, m_beg);
}

Token Lexer::slash_or_comment() noexcept {
  const char* start = m_beg;
  get();
  if (peek() == '/') {
    get();
    start = m_beg;
    while (peek() != '\0') {
      if (get() == '\n') {
        return Token(Token::Kind::Comment, start,
                     std::distance(start, m_beg) - 1);
      }
    }
    return Token(Token::Kind::Unexpected, m_beg, 1);
  } else {
    return Token(Token::Kind::Slash, start, 1);
  }
}

Token Lexer::logical_or_binary_operator() noexcept {
    const char* start = m_beg;
    get();
    const char next = peek();
    switch(*start) {
        case '&':
            if(*start == next) {
                return Token(Token::Kind::And, start, 2);
            } else {
                return Token(Token::Kind::BinaryOperator, start, 1);
            }
        case '|':
            if(*start == next) {
                return Token(Token::Kind::Or, start, next);
            } else {
                return Token(Token::Kind::BinaryOperator, start, 1);
            }
        default:
        return Token(Token::Kind::Unexpected, m_beg, 1);
    }
    return Token(Token::Kind::Unexpected, m_beg, 1);
}

Token Lexer::math_or_binary_operator() noexcept {
    const char* start = m_beg;
    get();
    const char next = peek();
    switch(*start) {
        case '>':
            if(*start != next) {
                if(next == '=') { 
                    return Token(Token::Kind::LessOrEquals, start, 2); 
                } else { 
                    return Token(Token::Kind::LessThan, start, 1); 
                }
            } else {
                return Token(Token::Kind::BinaryOperator, start, 2);
            }
        case '<':
            if(*start != next) {
                if(next == '=') { 
                    return Token(Token::Kind::GreaterOrEquals, start, 2); 
                } else { 
                    return Token(Token::Kind::GreaterThan, start, 1); 
                }
            } else {
                return Token(Token::Kind::BinaryOperator, start, 2);
            }
        default:
        return Token(Token::Kind::Unexpected, m_beg, 1);
    }
    return Token(Token::Kind::Unexpected, m_beg, 1);
}

Token Lexer::equal_or_equals() noexcept {
    const char* start = m_beg;
    get();
    const char next = peek();
    if(*start == next) {
        return Token(Token::Kind::Equals, start, next);
    } else {
        return Token(Token::Kind::Equal, start, 1);
    }
}

#include <iomanip>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Token::Kind& kind) {
  static const char* const names[]{
      "Number",      "Identifier",  "LeftParen",  "RightParen", "LeftSquare",
      "RightSquare", "LeftCurly",   "RightCurly", "LessThan", "LessOrEquals", 
      "GreaterThan", "GreaterOrEquals",
      "Equal",       "Plus",        "Minus",      "Asterisk",   "Slash",
      "Hash",        "Dot",         "Comma",      "Colon",      "Semicolon",
      "SingleQuote", "DoubleQuote", "Comment",    "BinaryOperator", "And", "Or",
      "Equals",        "End",          "Unexpected",
  };
  return os << names[static_cast<int>(kind)];
}

void Lexer::test() {

    auto code =
    "x = 2\n"
    "// This is a comment.\n"
    "var x\n"
    "var y\n"
    "var f = function(x, y) { sin(x) * sin(y) + x * y; }\n"
    "der(f, x)\n"
    "var g = function(x, y) { 2 * (x + der(f, y)); } // der(f, y) is a "
    "matrix\n"
    "var r{3}; // Vector of three elements\n"
    "var J{12, 12}; // Matrix of 12x12 elements\n"
    "var dot = function(u{:}, v{:}) -> scalar {\n"
    "          return u[i] * v[i]; // Einstein notation\n"
    "}\n"
    "var norm = function(u{:}) -> scalar { return sqrt(dot(u, u)); }\n"
    "var norm = function(u{:}) -> scalar { return u >= u; }\n"
    "<end>";

    Lexer lex(code);
    for (auto token = lex.next(); !token.is_one_of(Token::Kind::End, Token::Kind::Unexpected); token = lex.next()) {
        std::cout << std::setw(12) << token.kind() << " |" << token.lexeme() << "|\n";
    }
}
