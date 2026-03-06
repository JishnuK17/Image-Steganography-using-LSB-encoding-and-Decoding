#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"


 //Structure to store all decode information 

typedef struct _DecodeInfo
{
    /* Stego image name */
    char *stego_image_fname;

    /* Output file name */
    char *output_fname;

    /* File pointers */
    FILE *fptr_stego_image;  
    FILE *fptr_output;        

    /* Magic string buffer */
    char magic_string[3];

    /* Secret file extension size */
    unsigned int extn_size;

    /* Secret file extension */
    char secret_file_extn[10];

    /* Secret file size */
    unsigned int secret_file_size;

    /* Image data buffer */
    char image_data[32];

} DecodeInfo;

/* Function prototypes */

/* Read and validate decode arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Open required files */
Status open_decode_files(DecodeInfo *decInfo);

/* Main decoding function */
Status do_decoding(DecodeInfo *decInfo);

/* Decode magic string (#*) */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_extn_size(DecodeInfo *decInfo);

/* Decode secret file extension characters */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode actual secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode one byte from 8 LSBs */
Status decode_byte_from_lsb(char buffer[], char *data);

/*Decode 32-bit size */
Status decode_size_from_lsb(FILE *fptr, unsigned int *size);

#endif 
