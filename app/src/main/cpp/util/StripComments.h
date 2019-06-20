#ifndef STRIPCOMMENTS_H
#define STRIPCOMMENTS_H

#include "defines.h"
#include "DMath.h"

namespace DCanvas
{

class StripComments
{
public:
    ~StripComments()
    {
        if(m_out)
        {
            delete[] m_out;
            m_out = NULL;
        }
    }

    StripComments(const std::string& str)
        : m_parseState(BeginningOfLine)
        , m_sourceString(str)
        , m_length(str.length())
        , m_position(0)
    {
        m_out = new char[str.length()+1];
        memset(m_out, 0, sizeof(char) * ( m_length + 1));
        m_pos = 0;
        parse();
    }

    char* result()
    {
        return m_out;
    }

private:
    bool hasMoreCharacters()
    {
        return (m_position < m_length);
    }

    void parse()
    {
        while (hasMoreCharacters())
        {
            process(current());
            // process() might advance the position.
            if (hasMoreCharacters())
                advance();
        }
    }

    void process(unsigned char);

    bool peek(unsigned char& character)
    {
        if (m_position + 1 >= m_length)
            return false;
        character = m_sourceString[m_position + 1];
        return true;
    }

    unsigned char current()
    {
        ASSERT(m_position < m_length);
        return m_sourceString[m_position];
    }

    void advance()
    {
        ++m_position;
    }

    bool isNewline(unsigned char character)
    {
        // Don't attempt to canonicalize newline related characters.
        return (character == '\n' || character == '\r');
    }

    void emit(unsigned char character)
    {
        if(m_pos >= m_length)
        {
            return;
        }
        m_out[m_pos++] = character;
        //m_builder.append(character);
    }

    enum ParseState
    {
        // Have not seen an ASCII non-whitespace character yet on
        // this line. Possible that we might see a preprocessor
        // directive.
        BeginningOfLine,

        // Have seen at least one ASCII non-whitespace character
        // on this line.
        MiddleOfLine,

        // Handling a preprocessor directive. Passes through all
        // characters up to the end of the line. Disables comment
        // processing.
        InPreprocessorDirective,

        // Handling a single-line comment. The comment text is
        // replaced with a single space.
        InSingleLineComment,

        // Handling a multi-line comment. Newlines are passed
        // through to preserve line numbers.
        InMultiLineComment
    };

    ParseState m_parseState;
    std::string m_sourceString;
    unsigned m_length;
    unsigned m_position;
    char *m_out;
    unsigned m_pos;
};

void StripComments::process(unsigned char c)
{
    if (isNewline(c))
    {
        // No matter what state we are in, pass through newlines
        // so we preserve line numbers.
        emit(c);

        if (m_parseState != InMultiLineComment)
            m_parseState = BeginningOfLine;

        return;
    }

    unsigned char temp = 0;
    switch (m_parseState)
    {
        case BeginningOfLine:
            if (isASCIISpace(c))
            {
                emit(c);
                break;
            }

            if (c == '#')
            {
                m_parseState = InPreprocessorDirective;
                emit(c);
                break;
            }

            // Transition to normal state and re-handle character.
            m_parseState = MiddleOfLine;
            process(c);
            break;

        case MiddleOfLine:
            if (c == '/' && peek(temp))
            {
                if (temp == '/') {
                    m_parseState = InSingleLineComment;
                    emit(' ');
                    advance();
                    break;
                }

                if (temp == '*')
                {
                    m_parseState = InMultiLineComment;
                    // Emit the comment start in case the user has
                    // an unclosed comment and we want to later
                    // signal an error.
                    emit('/');
                    emit('*');
                    advance();
                    break;
                }
            }

            emit(c);
            break;

        case InPreprocessorDirective:
            // No matter what the character is, just pass it
            // through. Do not parse comments in this state. This
            // might not be the right thing to do long term, but it
            // should handle the #error preprocessor directive.
            emit(c);
            break;

        case InSingleLineComment:
            // The newline code at the top of this function takes care
            // of resetting our state when we get out of the
            // single-line comment. Swallow all other characters.
            break;

        case InMultiLineComment:
            if (c == '*' && peek(temp) && temp == '/')
            {
                emit('*');
                emit('/');
                m_parseState = MiddleOfLine;
                advance();
                break;
            }

            // Swallow all other characters. Unclear whether we may
            // want or need to just emit a space per character to try
            // to preserve column numbers for debugging purposes.
            break;
    } // switch
}

} // namespace DCanvas

#endif // STRIPCOMMENTS_H
