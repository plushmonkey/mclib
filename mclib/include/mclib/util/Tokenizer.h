#ifndef MCLIB_UTIL_TOKENIZER_H_
#define MCLIB_UTIL_TOKENIZER_H_

#include <sstream>
#include <string>
#include <vector>

namespace mc {
namespace util {

class Tokenizer {
public:
    typedef std::vector<std::string> TokenList;
    typedef TokenList::const_iterator const_iterator;
    typedef TokenList::reference reference;
    typedef TokenList::const_reference const_reference;
    typedef TokenList::size_type size_type;

private:
    std::string m_String;
    TokenList m_Tokens;

public:
    Tokenizer(const std::string &str) : m_String(str) {}

    TokenList::size_type tokenize(char delim, TokenList::size_type max = 0) {
        std::stringstream ss(m_String);
        std::string token = "";
        TokenList::size_type cur = 0;

        m_Tokens.clear();

        if (m_String.length() == 0) return 0;

        while (true) {
            cur++;
            if (max > 0 && cur >= max) {
                std::getline(ss, token);
                m_Tokens.push_back(token);
                cur++;
                break;
            }

            if (std::getline(ss, token, delim)) {
                m_Tokens.push_back(token);
            } else {
                if (m_String.at(m_String.length() - 1) == delim) {
                    m_Tokens.push_back("");
                    cur++;
                }
                break;
            }
        }
        return cur - 1;
    }

    TokenList::size_type operator()(char delim, TokenList::size_type max = 0) {
        return tokenize(delim, max);
    }

    const_iterator begin() const { return m_Tokens.begin(); }
    const_iterator end() const { return m_Tokens.end(); }
    reference operator[](TokenList::size_type i) { return m_Tokens[i]; }
    const_reference operator[](TokenList::size_type i) const {
        return m_Tokens[i];
    }
    TokenList::size_type size() const { return m_Tokens.size(); }
};

}  // namespace util
}  // namespace mc

#endif
