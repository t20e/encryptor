#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "utils/file_utils.h"

/**
 * @brief Helper function to convert string to vector of bytes
 * 
 * @param s string
 * @return std::vector<unsigned char> 
 */
std::vector<unsigned char> s_to_vector(const std::string &s)
{
	return std::vector<unsigned char>(s.begin(), s.end());
}


// --- TEST to_hex_preview() ---
struct HexTestParam {
		std::vector<unsigned char> input;
		std::string expected_out;
};

class ToHexPreviewTest : public ::testing::TestWithParam<HexTestParam> {};

TEST_P(ToHexPreviewTest, ValidateVariousOutput)
{
	// Get the current parameters struct for the current test run
	const auto &params = GetParam();

	// Call the function under test
	std::string output = to_hex_preview(params.input);

	EXPECT_EQ(output, params.expected_out);
}

INSTANTIATE_TEST_SUITE_P(
	HexConversionTests,
	ToHexPreviewTest,
	::testing::Values(
		// Test simple short string
		HexTestParam{
			// Hexadecimal:  48 45 4c 4c 30 is a representation of the string "HELLO"
			{'H', 'E', 'L', 'L', '0'},
			"0x00000000: 48 45 4c 4c 30 "},

		// Test empty input
		HexTestParam{{}, ""},

		// Test string "ABC 123" which is represented in hexadecimal form as '41 42 43 20 31 32 33'
		HexTestParam{
			{'A', 'B', 'C', ' ', '1', '2', '3'},
			"0x00000000: 41 42 43 20 31 32 33 "},
		// Test string thats longer than 16 bytes.
		HexTestParam{
			s_to_vector("This string is longer than 16 bytes."),
			"0x00000000: 54 68 69 73 20 73 74 72 69 6e 67 20 69 73 20 6c \n0x00000010: 6f 6e 67 65 72 20 74 68 61 6e 20 31 36 20 62 79 \n0x00000020: 74 65 73 2e "
			// the \n0x00000010 singles a new line and a new row containing 16 bytes, same as for \n0x00000020

		}

		));


// --- TEST is_likely_binary() ---
struct IsLikelyBinaryParam {
		std::vector<unsigned char> input;
		bool expected_out;
};

class IsLikelyBinaryTest : public ::testing::TestWithParam<IsLikelyBinaryParam> {};

TEST_P(IsLikelyBinaryTest, ValidateVariousOutput)
{
	// Get the current parameters struct for the current test run
	const auto &params = GetParam();

	// Call the function under test
	bool output = is_likely_binary(params.input);

	EXPECT_EQ(output, params.expected_out);
}


INSTANTIATE_TEST_SUITE_P(
	IsLikelyBinaryTest,
	IsLikelyBinaryTest,
	::testing::Values(

		// Normal text byte data
		IsLikelyBinaryParam{
			s_to_vector("Is not binary"),
			false},
		// Test with new line and tabs
		IsLikelyBinaryParam{
			s_to_vector("Line 1 \n Line 2 with tabs \t\t"),
			false},

		// Test with null byte: '\0' is binary
		IsLikelyBinaryParam{
			{'H', 'E', 'L', 'L', '\0', '0'},
			true},

		// Test with non-printable characters: '0x02'
		IsLikelyBinaryParam{
			{'H', 'E', 'L', 'L', 0x02, '0'},
			true}

		));
