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
        "  officesigner -- SecureBlackbox OfficeSigner Demo Application\n\n"
        "SYNOPSIS\n"
        "  officesigner [-input input_file] [-output output_file] [-cert certificate_file] [-certpass certificate_password]\n"
        "             [-sigtype signature_type] [-hashalg hash_algorithm] [-signdoc] [-signcore] [-signorigin]\n\n"
        "DESCRIPTION\n"
        "  OfficeSigner demonstrates the usage of OfficeSigner from SecureBlackbox.\n"
        "  Used to sign office documents.\n\n"
        "  The options are as follows:\n\n"
        "  -input        An input file to sign (Required).\n\n"
        "  -output       Where the signed file will be saved (Required).\n\n"
        "  -cert         The certificate used to sign files (Required).\n\n"
        "  -certpass     The password for the signing certificate (Required).\n\n"
        "  -sigtype      The type of signature to use. Enter the corresponding number. Valid values:\n\n"
        "                  0 - OST_DEFAULT\n"
        "                  1 - OST_BINARY_CRYPTO_API\n"
        "                  2 - OST_BINARY_XML\n"
        "                  3 - OST_OPEN_XML\n"
        "                  4 - OST_OPEN_XPS\n"
        "                  5 - OST_OPEN_DOCUMENT\n\n"
        "  -hashalg      The hash algorithm.\n\n"
        "  -signdoc      Whether to sign the document itself.\n\n"
        "  -signcore     Whether to sign the core properties of the document.\n\n"
        "  -signorigin   Whether to sign the signature origin.\n\n"
        "EXAMPLES\n"
        "  officesigner -input C:\\office\\helloworld.txt -output C:\\office\\myoffice.scs -cert C:\\certs\\mycert.pfx -certpass mypassword\n\n"
        "  officesigner -input C:\\office\\helloworld.txt -output C:\\office\\myoffice.scs -cert C:\\certs\\mycert.pfx -certpass mypassword \\\n"
        "             -sigtype 2 -hashalg SHA256 -signdoc -signcore -signorigin\n\n"
    );
}

int main(int argc, char** argv) {
    OfficeSigner signer;
    CertificateManager cm;

    // Validate input
    if (argc < 8) {
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

    char* cert = optval(argc, argv, "-cert");
    if (!strcmp(cert, "")) {
        printf("-cert is required.");
        displayHelp();
        goto done;
    }

    char* certpass = optval(argc, argv, "-certpass");
    if (!strcmp(certpass, "")) {
        printf("-certpass is required.");
        displayHelp();
        goto done;
    }

    // Additional options
    if (optext(argc, argv, "-sigtype")) {
        signer.SetNewSigSignatureType(atoi(optval(argc, argv, "-sigtype")));
    }
    if (optext(argc, argv, "-hashalg")) {
        signer.SetNewSigHashAlgorithm(optval(argc, argv, "-hashalg"));
    }
    signer.SetNewSigDocumentSigned(optext(argc, argv, "-signdoc"));
    signer.SetNewSigCorePropertiesSigned(optext(argc, argv, "-signcore"));
    signer.SetNewSigSignatureOriginSigned(optext(argc, argv, "-signorigin"));

    // Required options
    signer.SetInputFile(input);
    signer.SetOutputFile(output);
    cm.ImportFromFile(cert, certpass);
    signer.SetSigningCertHandle(cm.GetCertHandle());

    // Sign
    if (signer.Sign()) {
        goto done;
    }
    printf("Office file successfully signed.\n");

done:
    if (signer.GetLastErrorCode()) {
        printf("Error: [%i] %s\n", signer.GetLastErrorCode(), signer.GetLastError());
    }
    getchar();
}


