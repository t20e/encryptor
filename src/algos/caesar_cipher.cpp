#include <iostream>
#include <string>

/**
 * @brief One of the most simplest and oldest cryptography algorithms, it just shifts the characters around. Not a secure modern algorithm.
 * 
 * @param text The memory addresss of the text to encrypt or decrypt.
 * @param shift How much to shift individual characters. To decrypt an encrypted file use the same shift integer that was use to encrypt it.
 * 
 * @note The Caesar Cipher algorithm is named after Julius Caesar who implemented it to protect military communications.
 * 
 * @return std::string The encrypted or decrypted text.
 */
std::string caesarCipher(const std::string &text, int shift)
{
    std::string result = "";

    for (char c : text)
    {
        if (isupper(c))
        {
            // Encrypt uppercase characters
            result += char(int(c + shift - 65 + 26) % 26 + 65);
        }
        else if (islower(c))
        {
            result += char(int(c + shift - 97 + 26) % 26 + 97);
        }
        else if (isdigit(c))
        {
            result += char(int(c + shift - 48 + 10) % 10 + 48);
        }
        else
        { // keep non-alphanumeric characters the same
            result += c;
        }
    }
    return result;
}