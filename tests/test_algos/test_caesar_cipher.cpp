#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "algos/caesar_cipher.h"

TEST(CaesarCipherTest, WorksCorrectly)
{
    std::string originalText = "Hello, World! 123 START GO ||| FEARLESS _ DONER";
    int shift = 16;

    // --- Encryption ---
    std::string encryptedText = caesarCipher(originalText, shift);
    std::cout << "Original:  " << originalText << std::endl;
    std::cout << "Encrypted: " << encryptedText << std::endl;

    // --- Decryption ---
    // To decrypt, just use a negative shift
    // std::string decryptedText = caesarCipher(encryptedText, -shift);
    // std::cout << "Decrypted: " << decryptedText << std::endl;

    ASSERT_EQ(caesarCipher(encryptedText, -shift), originalText);

    // Will cause error
    // ASSERT_EQ(caesarCipher(encryptedText, -9), originalText);
}
