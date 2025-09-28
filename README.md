
# Encryptor


- TODO add more info about the project here

- TODO add the stuff about the algo identifiers when encrypting and decrypting

**cryptography:** A file encrypt/decrypt CLI built on `C++`.

Prerequisite:

- Install packages with `homebrew` globally:
  1. Google Test (GTest)
  2. [FTXUI](https://github.com/ArthurSonzogni/FTXUI)

    ```shell
    brew install googletest
    brew install ftxui
    ```

### Validating Methods

- Compare hashes with `sha256sum`

    ```shell
    sha256sum file_to_encrypt_orig.txt file_to_encrypt.txt
    ```

- Or use `diff` to compare if the decrypted file is the same as the original file:

    ```shell
    diff file_to_encrypt_orig.txt file_to_encrypt.txt
    ```

### How To Build

`Note:` on the first build, it will take a little longer than usual, thats because it's archiving the FTXUI library, which will only occur on the first build. However, when you run `make clean` it will have to re-archive it.

`Building final executable:`

```shell
# Build the application executable
make
# Run application
../build/encryptor;
```

`Testing:`

```shell
# Clean, build, and run the tests
make clean; make test
```

### How Install locally

TODO

