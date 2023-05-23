/*
 * SecureBlackbox 2022 C++ Edition - Demo Application
 *
 * Copyright (c) 2023 /n software inc. - All rights reserved. - www.nsoftware.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/secureblackbox.h"

namespace ArgParser {
    static char* optval(int argc, char** argv, const char* option) {
        for (int x = 0; x < argc - 1; x++) {
            if (!strcmp(argv[x], option)) {
                return argv[x + 1];
            }
        }
        return (char*)"";
    }

    static bool optext(int argc, char** argv, const char* option) {
        for (int x = 0; x < argc; x++) {
            if (!strcmp(argv[x], option)) {
                return true;
            }
        }
        return false;
    }
};

using namespace ArgParser;

void displayHelp() {
    printf(
        "NAME\n"
        "  messagecompressor -- SecureBlackbox MessageCompressor Demo Application\n\n"
        "SYNOPSIS\n"
        "  messagecompressor [-input input_file] [-output output_file] [-level level]\n\n"
        "DESCRIPTION\n"
        "  MessageCompressor demonstrates the usage of MessageCompressor from SecureBlackbox.\n"
        "  Used to create compressed PKCS#7 messages.\n\n"
        "  The options are as follows:\n\n"
        "  -input        An input file to compress (Required).\n\n"
        "  -output       Where the compressed file will be saved (Required).\n\n"
        "  -level        The comression level. \n\n"
        "EXAMPLES\n"
        "  messagecompressor -input C:\\pkcs7\\helloworld.txt -output C:\\pkcs7\\compress.pkcs7 \n\n"
        "  messagecompressor -input C:\\pkcs7\\helloworld.txt -output C:\\pkcs7\\compress.pkcs7 -level 7 \n\n"
    );
}

int main(int argc, char** argv) {
    MessageCompressor compressor;

    // Validate input
    if (argc < 4) {
        displayHelp();
        goto done;
    }

    char* input = optval(argc, argv, "-input");
    if (!strcmp(input, "")) {
        printf("-input is required.");
        displayHelp();
        goto done;
    }

    char* output = optval(argc, argv, "-output");
    if (!strcmp(output, "")) {
        printf("-output is required.");
        displayHelp();
        goto done;
    }

    // Additional options
    if (optext(argc, argv, "-level")) {
        compressor.SetCompressionLevel(atoi(optval(argc, argv, "-level")));
    }

    // Required options
    compressor.SetInputFile(input);
    compressor.SetOutputFile(output);

    // Compress
    if (compressor.Compress()) {
        goto done;
    }
    printf("The file successfully compressed.\n");

done:
    if (compressor.GetLastErrorCode()) {
        printf("Error: [%i] %s\n", compressor.GetLastErrorCode(), compressor.GetLastError());
    }
    getchar();
}


