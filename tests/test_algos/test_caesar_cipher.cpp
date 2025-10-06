#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <tuple>
#include <variant>
#include <vector>


#include "algos/caesar_cipher.h"

// --- TEST caesarCipherValidate() ---

using validationParams = std::tuple<std::string, bool, std::string, int>;

class CaesarCipherValidateParamTest : public ::testing::TestWithParam<validationParams> {
		// Setup adn TearDown() methods can go here if needed!
};

TEST_P(CaesarCipherValidateParamTest, HandleVariousInputs)
{
	// Get the current parameters for the current test run
	const auto &[input_shift, expected_valid, expected_msg, expected_value] = GetParam();

	// Call the function under test
	auto [actual_valid, actual_msg, actual_value] = caesarCipherValidate(input_shift);

	EXPECT_EQ(actual_valid, expected_valid);
	EXPECT_EQ(actual_msg, expected_msg);

	if (actual_valid) { // Only check the return integer value if the return expected_valid passed.
		EXPECT_EQ(actual_value, expected_value);
	}
}

INSTANTIATE_TEST_SUITE_P(
	ValidationInputs,
	CaesarCipherValidateParamTest,
	::testing::Values(
		// Each line is a complete test case.
		std::make_tuple("10", true, "", 10),
		std::make_tuple("-5", true, "", -5),
		std::make_tuple("", false, "Please enter input", 0),
		std::make_tuple("abc", false, "Please enter a number!", 0),
		std::make_tuple("5a", false, "Please only enter a number!", 0),
		std::make_tuple("26", false, "Please enter a number between -25 and 25!", 0),
		std::make_tuple("-26", false, "Please enter a number between -25 and 25!", 0)));


// --- TEST caesarCipher() ---

TEST(CaesarCipherTest, VerifyDecryptionMatchesEncryption)
{
	const std::string origString = "Hello, World! 123 START GO ||| FEARLESS _ DONER";
	const std::vector<unsigned char> origVector(origString.begin(), origString.end());

	// ENCRYPT
	std::string encryptShift = "16";
	cryptoResult encryption = caesarCipher(origVector, encryptShift);

	// Verify if the function succeeded and returned a vector instead of a string.
	ASSERT_TRUE(std::holds_alternative<std::vector<unsigned char>>(encryption));

	// Since the result can be a variant we need to unpack the result to use it.
	std::vector<unsigned char> encrypted_vec = std::get<std::vector<unsigned char>>(encryption);

	// DECRYPT
	std::string decryptShift = "-16";
	cryptoResult decryption = caesarCipher(encrypted_vec, decryptShift);

	ASSERT_TRUE(std::holds_alternative<std::vector<unsigned char>>(decryption));
	std::vector<unsigned char> decrypted_vec = std::get<std::vector<unsigned char>>(decryption);

	// Compare if the two vector of bytes are equal
	ASSERT_EQ(decrypted_vec, origVector);
}
