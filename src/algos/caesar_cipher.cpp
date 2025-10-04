#include <vector>


std::vector<unsigned char> caesarCipher(
		const std::vector<unsigned char> &data,
		int shift)
{
	std::vector<unsigned char> result;
	result.reserve(data.size()); // Pre-allocate memory

	for (unsigned char byte: data) {
		if (isupper(byte)) {
			result.push_back((byte + shift - 65 + 26) % 26 + 65);
		} else if (islower(byte)) {
			result.push_back((byte + shift - 97 + 26) % 26 + 97);
		} else { // keep non-alphanumeric characters the same
			result.push_back(byte);
		}
	}
	return result;
}

// std::string caesarCipher(const std::string &text, int shift)
// {
//     std::string result = "";

//     for (char c : text)
//     {
//         if (isupper(c))
//         {
//             // Encrypt uppercase characters
//             result += char(int(c + shift - 65 + 26) % 26 + 65);
//         }
//         else if (islower(c))
//         {
//             result += char(int(c + shift - 97 + 26) % 26 + 97);
//         }
//         else if (isdigit(c))
//         {
//             result += char(int(c + shift - 48 + 10) % 10 + 48);
//         }
//         else
//         { // keep non-alphanumeric characters the same
//             result += c;
//         }
//     }
//     return result;
// }
