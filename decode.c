#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"
#include "types.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    //validate whether user has provided the .bmp or not
    if(argv[2] != NULL && strcmp(strstr(argv[2],"."),".bmp")==0)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    //optional output file
    if(argv[3] != NULL)
    {
        decInfo->output_fname = argv[3];
    }
    else
    {
        decInfo->output_fname = "decoded.txt";
    }

    return e_success;
}
/* Open stego image and output file */
Status open_decode_files(DecodeInfo *decInfo)
{
    // Open stego image in read mode
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname,"r");
    if(decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr,"ERROR: Unable to open %s\n",decInfo->stego_image_fname);
        return e_failure;
    }
    // Open output file to write decoded data
    decInfo->fptr_output = fopen(decInfo->output_fname,"w");
    if(decInfo->fptr_output == NULL)
    {
        perror("fopen");
        fprintf(stderr,"ERROR: Unable to create %s\n",decInfo->output_fname);
        return e_failure;
    }

    return e_success;
}

//Decode one byte from LSB of 8 image bytes
Status decode_byte_from_lsb(char buffer[], char *data)
{
    unsigned char ch = 0;

    for(int i = 0; i < 8; i++)
    {
        ch = (ch << 1) | (buffer[i] & 1);
    }

    *data = ch;
    return e_success;
}

//Decode 32-bit integer  from LSB
Status decode_size_from_lsb(FILE *fptr, unsigned int *size)
{
    char buffer[32];
    fread(buffer,1,32,fptr);

    unsigned int num = 0;
    for(int i = 0; i < 32; i++)
    {
        num = (num << 1) | (buffer[i] & 1);
    }

    *size = num;
    return e_success;
}

/* Decode magic string "#*" */
Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);

    for(int i = 0; i < 2; i++)
    {
        fread(decInfo->image_data,1,8,decInfo->fptr_stego_image);
        decode_byte_from_lsb(decInfo->image_data,&decInfo->magic_string[i]);
    }

    decInfo->magic_string[2] = '\0';

    printf("Decoded Magic String: %s\n",decInfo->magic_string);

    if(strcmp(decInfo->magic_string,MAGIC_STRING) != 0)
    {
        printf("ERROR: Magic string mismatch\n");
        return e_failure;
    }

    printf("Successfully decoded the magic string\n");
    return e_success;
}

//Decode secret file extension size 
Status decode_extn_size(DecodeInfo *decInfo)
{
    decode_size_from_lsb(decInfo -> fptr_stego_image, &decInfo -> extn_size);

    printf("Decoded secret file extension size: %u\n", decInfo -> extn_size);

    return e_success;
}

//Decode secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    for(unsigned int i = 0; i < decInfo->extn_size; i++)
    {
        fread(decInfo->image_data,1,8,decInfo->fptr_stego_image);
        decode_byte_from_lsb(decInfo->image_data, &decInfo->secret_file_extn[i]);
    }

    decInfo->secret_file_extn[decInfo->extn_size] = '\0';

    printf("Decoded secret file extension: %s\n",decInfo->secret_file_extn);

    return e_success;
}

//Decode secret file size 
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_size_from_lsb(decInfo->fptr_stego_image, &decInfo->secret_file_size);

    printf("Decoded secret file size: %u\n",decInfo->secret_file_size);

    return e_success;
}

//Decode secret file data
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;

    for(unsigned int i = 0; i < decInfo->secret_file_size; i++)
    {
        fread(decInfo->image_data,1,8,decInfo->fptr_stego_image);

        decode_byte_from_lsb(decInfo->image_data,&ch);
        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }

    printf("Successfully decoded the secret data\n");

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if(open_decode_files(decInfo) == e_success)
    {
        printf("Opened stego image successfully\n");

        if(decode_magic_string(decInfo) == e_success)
        {
            if(decode_extn_size(decInfo) == e_success)
            {
                if(decode_secret_file_extn(decInfo) == e_success)
                {
                    if(decode_secret_file_size(decInfo) == e_success)
                    {
                        if(decode_secret_file_data(decInfo) == e_success)
                        {
                            printf("Decoding Completed Successfully\n");
                            return e_success;
                        }
                        else
                        {
                            printf("Failed to decode secret data!\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Failed to decode secret file size!\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Failed to decode secret file extension!\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Failed to decode secret extension size!\n");
                return e_failure;
            }
        }
        else
        {
            printf("Failed to decode magic string!\n");
            return e_failure;
        }
    }
    else
    {
        printf("Failed to open stego image file!\n");
        return e_failure;
    }

    return e_success;
}
