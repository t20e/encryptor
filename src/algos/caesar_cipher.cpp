#include <algorithm>
#include <limits>
#include <tuple>
#include <vector>

#include "algos/caesar_cipher.h"

cryptoResult caesarCipher(
		const std::vector<unsigned char> &data,
		const std::string &shift)
{
	// validate the user input and convert to int
	auto [valid, error_msg, valShift] = caesarCipherValidate(shift);

	if (!valid) {
		// return std::make_tuple({}, "");
		return error_msg;
	}

	std::vector<unsigned char> result;
	result.reserve(data.size()); // Pre-allocate memory

	// Perform algo
	for (unsigned char byte: data) {
		if (isupper(byte)) {
			result.push_back((byte + valShift - 'A' + 26) % 26 + 'A');
		} else if (islower(byte)) {
			result.push_back((byte + valShift - 'a' + 26) % 26 + 'a');
		} else { // keep non-alphanumeric characters the same
			result.push_back(byte);
		}
	}
	return result;
}

validationResult caesarCipherValidate(const std::string &shift)
{
	if (shift.empty()) { //empty string
		return {false, "Please enter input", 0};
	}

	//Make the shift amount is within a limit range.
	//Since there are onl 26 letters in the English alphabet, a shift size of -28 is the same as a shift size of -2, a shift size of 52 is identical to 0, a shift size of 27 is identical to a shift size of 1, because 26+1 = 27.
	// So we will limit the shift size to be between [-25, 25].
	int valShift = 0;
	size_t chars_processed = 0; // Store how many character std::stoi() reads.

	try {
		valShift = std::stoi(shift, &chars_processed); // convert to string
	} catch (const std::invalid_argument &e) {
		return {false, "Please enter a number!", 0};
	} catch (const std::out_of_range &e) { // User entered a number that can't be stored in a integer data type.
		return {false, "Please enter a number between -25 and 25!", 0};
	}

	// Note that even if the string has a non-number in it, as long as the string has a number before it, std::stoi() will count it as a number and return the number without the non-number, ex: -1g -> -1 & 1h -> 1, so we need to handle that!
	if (chars_processed != shift.length()) {
		return {false, "Please only enter a number!", 0};
	}

	const int MAX_SHIFT_SIZE = 25;
	if (valShift > MAX_SHIFT_SIZE || valShift < -MAX_SHIFT_SIZE) {
		return {false, "Please enter a number between -25 and 25!", 0};
	}

	return {true, "", valShift};
}
