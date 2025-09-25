
# Encryptor

**cryptography:** A file encrypt/decrypt CLI built on `C++`.

Prerequisite:

- Install google test globally with `homebrew`.

    ```shell
    brew install googletest
    ```
- Install [FTXUI](https://github.com/ArthurSonzogni/FTXUI)


- TODO add more info about the project here

- TODO add the stuff about the algo identifiers when encrypting and decrypting

### Validating Methods

- Compare hashes with `sha256sum`

    ```shell

    sha256sum file_to_encrypt_orig.txt file_to_encrypt.txt
    ```

- Or use `diff` to compare if the decrypted file is the same as the original file:

    ```shell
    diff file_to_encrypt_orig.txt file_to_encrypt.txt
    ```

### How To Run


`Building final executable`

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

