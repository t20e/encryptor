#pragma once

#include <tuple>
#include <vector>
#include <variant>

// Type alias
using cryptoResult = std::variant<std::vector<unsigned char>, std::string>; // return either a vector or a string

using validationResult = std::tuple<bool, std::string, int>; 


/**
 * @brief One of the most simplest and oldest cryptography algorithms, it just shifts the characters around. Not a secure modern algorithm!
 * 
 * @param data The vector of bytes to encrypt or decrypt.
 * @param shift How much to shift individual characters. To decrypt an encrypted file use the same shift integer that was use to encrypt it.
 * @note The Caesar Cipher algorithm is named after Julius Caesar who implemented it to protect military communications. This function applies the algorithm directly to the bytes, instead of first converting it into a string!
 * @return cryptoResult Returns either an error string to display to the user, or the encrypted or decrypted result.
 */
cryptoResult caesarCipher(
		const std::vector<unsigned char> &data,
		const std::string &shift);


/**
 * @brief Validate the users input for the caesar cipher shift.
 * 
 * @param shift The users shift input.
 * @return std::tuple<bool, std::string> The first element is 'true' if valid, 'false' otherwise. The second contains the error message if invalid.
 */



/**
 * @brief Validate the users input for the caesar cipher shift.
 * 
 * @param shift The users shift input.
 * @return validationResult The first element is 'true' if valid, 'false' otherwise. The second contains the error message if invalid. And the third contains the converted shift string to an int.
 */
validationResult caesarCipherValidate(const std::string &shift);
