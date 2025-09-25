#pragma once

// Set global variables
// The class with the enum provides stronger type safety, which prevents accidental conversion of the enum values to integers and requires you to be explicit when using them.
enum class AlgoIdentifier
{
    // We add a smaller identify integer ID to the beginner/header of the encrypted file.
    // We can then later use this header when the user wants to decrypt a file. They pass in the encrypted file, we read it, and know if they need to pass a hash, a shift number, or other.
    caesar_cipher = 1,
    XOR_cipher = 2,
    AES = 3,
};