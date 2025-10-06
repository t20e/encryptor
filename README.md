
# Encryptor

- **//TODO's:**
  
- Add more info & pics about the project in this ReadMe.md.
- Add the stuff about the algo identifiers when encrypting and decrypting
- Make the ASCII art interactive.
- Add the createCollapsible() to the cryptoWindow

**Cryptography:** A file encrypt/decrypt terminal user interface (TUI) built on `C++`.

**NOTE:** This project is a portfolio showcase, don't use for state-of-the-art encryption, use `openssl` for that.

- **Prerequisite:**

  - Install necessary packages with `homebrew` globally:
    - [Google Test (GTest)](https://github.com/google/googletest) & [FTXUI](https://github.com/ArthurSonzogni/FTXUI)

        ```shell
        brew install googletest;
        brew install ftxui;
        ```
    - `clang` compiler.

    - Optional: // TODO needed?
      - [Bear](https://github.com/rizsotto/Bear): "Bear is a tool that generates a compilation database for clang tooling", I use it for the [clangD](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd) intellisense VSC extension.

        ```shell
        brew install bear;
        make clean;
        bear -- make; # Creates a ./compile_commands.json that helps the clangD extension.
        ```

## Validating Encryptions

Check if the decrypted file is the same as the original pre-encrypted file.

- Compare hashes with `sha256sum`

    ```shell
    sha256sum file_to_encrypt_orig.txt decrypted_file.txt
    ```

- Or use `diff`:

    ```shell
    diff file_to_encrypt_orig.txt file_to_encrypt.txt
    ```

## How To Build & Install Locally

`Build:`

```shell
# Build the executable application.
make
# Run application
../build/encryptor;
```

`If Test:`

```shell
# Clean, build, and run the tests
make clean; make test
```

`Install Locally:` # TODO 