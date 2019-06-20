#ifndef BASE64_H
#define BASE64_H

#include <vector>

namespace DCanvas
{

//void base64Encode(const Vector<char>&, Vector<char>&, bool insertLFs = false);
void base64Encode(const char*, unsigned, std::vector<char>&, bool insertLFs = false);

// this decoder is not general purpose - it returns an error if it encounters a linefeed, as needed for window.atob
//bool base64Decode(const Vector<char>&, Vector<char>&);
bool base64Decode(const char*, unsigned, std::vector<char>&);

} // namespace DCanvas

#endif // Base64_h
