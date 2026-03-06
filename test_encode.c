/*#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>

int main(int argc, char *argv[])
{
   //check operation type
   if(check_operation_type(argv) == e_encode)
   {
    EncodeInfo encInfo;
    printf("Selected Encoding\n");
    if(read_and_validate_encode_args(argv,&encInfo)==e_success)
    {
        printf("Read and validate input arguments successfully\n");
        if(do_encoding(&encInfo) == e_success)
        {
            printf("Completed Encoding\n");
        }
        else
        {
            printf("Failed to do encoding\n");
        }
    }
    else
    {
        printf("Failed to validate the input arguments\n");
    }
   }
   else if(check_operation_type(argv) == e_decode)
   {
        printf("Selected Decoding\n");
   }
   else
   {
        printf("Invalid Operation\n");
        printf("***********************Usage***********************\n");
        printf("Encoding : ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
        printf("Decoding : ./a.out -d stego.bmp decode.txt\n");
   }
   return 0;
}
OperationType check_operation_type(char *argv[])
{
    //./a.out -e beautiful.bmp secret.txt
    if(strcmp(argv[1],"-e")==0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}*/
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

OperationType check_operation_type(char *argv[])
{
    if (argv[1] != NULL && strcmp(argv[1], "-e") == 0)
        return e_encode;

    else if (argv[1] != NULL && strcmp(argv[1], "-d") == 0)
        return e_decode;

    else
        return e_unsupported;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage:\n");
        printf("./a.out -e <src.bmp> <secret.txt> <stego.bmp optional>\n");
        printf("./a.out -d <stego.bmp> <output optional>\n");
        return 0;
    }

    OperationType option = check_operation_type(argv);

    if (option == e_encode)
    {
        EncodeInfo encInfo;

        printf("------ Selected Encoding ------\n");

        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("Read and validate encode arguments successfully\n");

            if (do_encoding(&encInfo) == e_success)
            {
                printf("------ Encoding Completed Successfully ------\n");
            }
            else
            {
                printf("Encoding Failed\n");
            }
        }
        else
        {
            printf("Invalid encoding arguments\n");
        }
    }
    else if (option == e_decode)
    {
        DecodeInfo decInfo;

        printf("------ Selected Decoding ------\n");

        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            printf("Read and validate decode arguments successfully\n");

            if (do_decoding(&decInfo) == e_success)
            {
                printf("------ Decoding Completed Successfully ------\n");
            }
            else
            {
                printf("Decoding Failed\n");
            }
        }
        else
        {
            printf("Invalid decoding arguments\n");
        }
    }
    else
    {
        printf("Unsupported Option\n");
    }

    return 0;
}
