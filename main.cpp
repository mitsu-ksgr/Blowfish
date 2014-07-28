//
// Blowfish Command Line Tools.
//
// CC0 - PUBLIC DOMAIN
// This work is free of known copyright restrictions.
// http://creativecommons.org/publicdomain/zero/1.0/
//

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include "blowfish.h"


//------------------------------------------------------------------------------
//  Constants.
//------------------------------------------------------------------------------
constexpr const char *kUsageText = ""
        "NAME"
        "\n\tblowfish"
        "\nSYNOPSIS"
        "\n\tblowfish file_path key [-o output_file_path] [-e | -d]"
        "\nDESCRIPTION"
        "\n\tEncrypt/Decrypt file using the Blowfish."
        "\n\n\tOptions:"
        "\n\t-o\tOutput file path."
        "\n\t-e\tRun in the Encrypt Mode(Default)."
        "\n\t-d\tRun in the Decrypt Mode."
        ;

constexpr const char *kFlagEncryptionMode   = "-e";
constexpr const char *kFlagDecryptionMode   = "-d";
constexpr const char *kFlagOutputFile       = "-o";
constexpr const char *kDefaultEncryptedFileFormatExtension = ".bfe";
constexpr const char *kDefaultDecryptedFileFormatExtension = ".bfd";


//------------------------------------------------------------------------------
//  Console Option.
//------------------------------------------------------------------------------
struct ConsoleOption {
    bool        isFailed;
    bool        isEncryptMode;
    std::string FilePath;
    std::string Key;
    std::string OutputFilePath;

    // constructor.
    ConsoleOption()
    : isFailed(false), isEncryptMode(true)
    , FilePath(""), Key(""), OutputFilePath("")
    {}
};

ConsoleOption getOption(int argc, const char **argv)
{
    ConsoleOption option;

    // Argment Error!
    if(argc <= 2) {
        option.isFailed = true;
        return option;
    }

    // Get FilePath & Key.
    option.FilePath = argv[1];
    option.Key      = argv[2];

    // Get Option.
    for(int cur_idx = 3; cur_idx < argc; ++cur_idx) {
        std::string opt_flag = argv[cur_idx];

        // Option: Output File.
        if(opt_flag.compare(kFlagOutputFile) == 0 && argc >= cur_idx + 2) {
            if(argv[cur_idx + 1][0] == '-')
                continue;
            option.OutputFilePath = argv[++cur_idx];

        // Option: Run Mode.
        } else {
            if(opt_flag.compare(kFlagEncryptionMode) == 0)
                option.isEncryptMode = true;
            else if(opt_flag.compare(kFlagDecryptionMode) == 0)
                option.isEncryptMode = false;
        }
    }

    // If -o option is not specified, Set up the output file path.
    if(option.OutputFilePath.empty()) {
        option.OutputFilePath = option.FilePath;
        if(option.isEncryptMode)
            option.OutputFilePath += kDefaultEncryptedFileFormatExtension;
        else
            option.OutputFilePath += kDefaultDecryptedFileFormatExtension;
    }
    return option;
}


//------------------------------------------------------------------------------
//  File Reader/Writer.
//------------------------------------------------------------------------------
bool readFile(const std::string &file_path, char **data, size_t *data_size)
{
    std::ifstream ifs(file_path, std::ios::binary);
    if(ifs.fail())
        return false;

    size_t file_size = static_cast<size_t>(ifs.seekg(0, std::ios::end).tellg());
    ifs.seekg(0, std::ios::beg);

    *data = new char[file_size];
    ifs.read(*data, file_size);
    if(ifs.bad()) {
        if(*data) {
            delete[] *data;
            *data = nullptr;
        }
        return false;
    }
    ifs.close();
    *data_size = file_size;
    return true;
}

bool writeFile(const std::string &file_path, const char *data, size_t data_size)
{
    std::ofstream ofs(file_path, std::ios::binary);
    if(ofs.fail())
        return false;

    ofs.write(data, data_size);
    bool result = !ofs.bad();
    ofs.close();
    return result;
}


//------------------------------------------------------------------------------
//  Entry point.
//------------------------------------------------------------------------------
int main(int argc, const char **argv)
{
    auto option = getOption(argc, argv);
    if(option.isFailed) {
        std::cerr << "ERROR: Invalid argument was specified." << std::endl;
        std::cout << kUsageText << std::endl;
        exit(-1);
    }

    // File Data.
    char *data = nullptr;
    size_t data_size = 0;

    // Read File.
    if(!readFile(option.FilePath, &data, &data_size)) {
        std::cerr << "ERROR: Failed to open file. FilePath = ";
        std::cerr << option.FilePath << std::endl;
        exit(-1);
    }

    // Blowfish
    Blowfish blowfish;
    blowfish.SetKey(option.Key);
    if(option.isEncryptMode)    blowfish.Encrypt(data, data, data_size);
    else                        blowfish.Decrypt(data, data, data_size);

    // Write File.
    if(!writeFile(option.OutputFilePath, data, data_size)) {
        std::cerr << "ERROR: Failed to write file. OutputFilePath = ";
        std::cerr << option.OutputFilePath << std::endl;
    } else {
        // All done! Display result.
        auto get_mode_name = [&option]() {
            return (option.isEncryptMode?"Encryption":"Decryption"); };
        std::cout << "Succeeded! "
                    << "\"" << option.FilePath << "\""
                    << " --[" << get_mode_name()
                    << "(Key:" << option.Key << ")]--> "
                    << "\"" << option.OutputFilePath << "\"" << std::endl;
    }

    // Release dynamic allocated memory.
    delete[] data;
    data = nullptr;
    return 0;
}
