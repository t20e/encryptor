#pragma once

#include <vector>

/**
 * @brief One of the most simplest and oldest cryptography algorithms, it just shifts the characters around. Not a secure modern algorithm!
 * 
 * @param data The vector of bytes to encrypt or decrypt.
 * @param shift How much to shift individual characters. To decrypt an encrypted file use the same shift integer that was use to encrypt it.
 * 
 * @note The Caesar Cipher algorithm is named after Julius Caesar who implemented it to protect military communications. This function applies the algorithm directly to the bytes, instead of first converting it into a string!
 * 
 * @return std::vector<unsigned char> The resulting encrypted or decrypted bytes.
 */
std::vector<unsigned char> caesarCipher(
		const std::vector<unsigned char> &data,
		int shift);
