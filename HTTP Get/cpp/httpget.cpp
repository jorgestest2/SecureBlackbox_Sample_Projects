/*
 * SecureBlackbox 2022 C++ Edition - Demo Application
 *
 * Copyright (c) 2023 /n software inc. - All rights reserved. - www.nsoftware.com
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/secureblackbox.h"
#define LINE_LEN 100

class MyHTTP : public HTTPClient
{
public:

    int FireCertificateValidate(HTTPClientCertificateValidateEventParams* e) override {
        if (e->Accept) {
            return 0;
        }
        printf("Server provided the following certificate:\nIssuer: %s\nSubject: %s\n",
               this->GetServerCertIssuerRDN(0), this->GetServerCertSubjectRDN(0));
        printf("Would you like to continue? [y/n] ");
        if (getchar() == 'y') {
            e->Accept = true;
        } else {
            exit(0);
        }
        return 0;
    }
};

int main(int argc, char** argv) {

    if (argc < 2) {
        fprintf(stderr, "usage: httpget url\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "  url  the url to fetch\n");
        fprintf(stderr, "\nExample: httpget https://www.google.com\n\n");
        printf("Press enter to continue.");
        getchar();

    } else {
        MyHTTP http;

        const int ret_code = http.Get(argv[1]);

        if (ret_code) // Got an error.  The user is done.
        {
            printf("\nError: %d", ret_code);
            if (http.GetLastError()) {
                printf(" \"%s\"\n", http.GetLastError());
            }
        }

        printf("Server Response:\n\n%s", http.GetOutputString());

        fprintf(stderr, "\npress <return> to continue...\n");
        getchar();
        exit(ret_code);
        return 0;
    }

}


