/**
 * This file contains code for resizing a .bmp file based on CS50 pset3.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "bmp.h"
#include <ctype.h>
#include <stdbool.h>

void skip_padding(FILE *fp, int elmnt_size, int num_elmnts);
void add_padding(FILE *fp, int elmnt_size, int num_elmnts);
void cpy_fdata2array(FILE *fp, int f_offset, BYTE** array, int rows, int cols );
void print_array(BYTE** array, int elmnt_size, int rows, int cols);
void array_multiply(BYTE** input_array, BYTE** output_array, int n, int input_rows, int input_cols);
void cpy_arraydata2f(FILE *fp, int f_offset, BYTE** array, int rows_out, int cols_out);
void print_bmp_file_info(BITMAPFILEHEADER bf, BITMAPINFOHEADER bi);

int main (int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 4 || (!isdigit(argv[1][0])))
    {
        fprintf(stderr, "Usage: resize n infile outfile\n");
        return 1;
    }


    // Save multiply factors and file names
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];



    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }


    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bf_out;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bi_out;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Create array to store raw data from input file
    int rows, cols, offset;
    rows = abs(bi.biHeight);
    cols = abs(bi.biWidth);
    offset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // first <offset> bytes of a .bmp file are just file info so I'm using this to know where raw data starts
    BYTE **cpy_array = (BYTE **)malloc(rows * sizeof(BYTE*));

    for (int i = 0; i < rows; i++){
        cpy_array[i] = (BYTE *)malloc(cols *sizeof(RGBTRIPLE)* sizeof(BYTE));
    }

    // Create array that will be used to store raw data that will be written to output file
    int rows_out, cols_out;
    rows_out = n*rows;
    cols_out = n*cols;
    BYTE **output_array = (BYTE **)malloc(rows_out  * sizeof(BYTE*));

    for (int i = 0; i < rows_out ; i++){
        output_array[i] = (BYTE *)malloc(cols_out * sizeof(RGBTRIPLE) * sizeof(BYTE));
    }

    // Create output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // Intialize outfile with empty data where the file header and info header will go
    fseek(outptr, 0, SEEK_SET);
    for (int i = 0; i < offset; i++){
        char c = 0;
        fputc(c, outptr);
    }

    // Process data - copy file raw data to array, multiply and save new data into a new array
    cpy_fdata2array(inptr, offset, cpy_array, rows, cols);
    //print_array(cpy_array, sizeof(RGBTRIPLE), rows, cols); // for debugging only
    array_multiply(cpy_array, output_array, n, rows, cols);

     // Write new raw data to output file
    cpy_arraydata2f(outptr, offset, output_array, rows_out, cols_out);
    //printf("\n \n Output Array: \n"); // debug
    //print_array(output_array, sizeof(RGBTRIPLE), rows_out, cols_out); // debug only

    // Write file header and info header
    bf_out = bf;
    bi_out = bi;
    fseek(outptr,0, SEEK_END);
    DWORD num_bytes_outputf = ftell(outptr); // number of bytes in output file
    bf_out.bfSize = num_bytes_outputf;
    bi_out.biSizeImage = bf_out.bfSize - (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
    bi_out.biWidth = bi.biWidth * n;
    bi_out.biHeight = bi.biHeight * n;

    // Write outfile's BITMAPFILEHEADER and BITMAPINFOHEADER
    fseek(outptr, 0, SEEK_SET);
    fwrite(&bf_out, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi_out, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Print file info (debugging only)
    //printf("Input file info: \n");
    //print_bmp_file_info(bf, bi);
    //printf("\n Output file info: \n");
    //print_bmp_file_info(bf_out, bi_out);

    for (int i = 0; i < rows; i++){
        free(cpy_array[i]);
    }
    free(cpy_array);

    for (int i = 0; i < rows_out; i++){
        free(output_array[i]);
    }
    free(output_array);

    // Close infile
    fclose(inptr);

    // Close outfile
    fclose(outptr);

}

void skip_padding(FILE *fp, int elmnt_size, int num_elmnts){
    /** This function skips padding inside of a .bmp file
     * elmnt_size: size of each padding element in bytes, a padding element
     * here is basically a RGBTRIPLE of 0s to be skipped
     * num_elmnts: total number of elements including padding elements
     *
     */
     int padding = ((4 - (num_elmnts % 4)) % 4)*elmnt_size;
     fseek(fp, padding, SEEK_CUR);

}

void add_padding(FILE *fp, int elmnt_size, int num_elmnts){
/** This function adds padding inside of a .bmp file
 * elmnt_size: size of each padding element in bytes, a padding element
 * here is basically a RGBTRIPLE of 0s to be added
 * num_elmnts: total number of elements including padding elements
 *
 */
int padding = ((4 - (num_elmnts*elmnt_size % 4)) % 4);

    BYTE c = 0u;
    for (int i = 0; i < padding; i++){
        fputc(c,fp);
    }
}

void cpy_fdata2array(FILE *fp, int f_offset, BYTE** array, int rows, int cols ){
    /** This function takes raw data from a bmp file and copies it onto a 2D array.
     * fp: file pointer
     * f_offset: this is where to begin reading or the number of bytes to omit/skip
     * rows, cols: used to structure the data into a 2D array
     */
    if(fp == NULL){
        printf("NULL file pointer detected\n");
    }

    fseek(fp, f_offset, SEEK_SET);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, fp);
            array[i][j*sizeof(RGBTRIPLE)] = triple.rgbtBlue;
            array[i][j*sizeof(RGBTRIPLE) + 1] = triple.rgbtGreen;
            array[i][j*sizeof(RGBTRIPLE) + 2] = triple.rgbtRed;
        }
        skip_padding(fp, sizeof(RGBTRIPLE), cols);
    }

}

void array_multiply(BYTE** input_array, BYTE** output_array, int n, int input_rows, int input_cols){
    for (int i = 0 ; i < input_rows; i++){
        for (int j = 0; j < input_cols; j++){

            RGBTRIPLE triple;
            triple.rgbtBlue = input_array[i][j*sizeof(RGBTRIPLE)];
            triple.rgbtGreen = input_array[i][j*sizeof(RGBTRIPLE) + 1];
            triple.rgbtRed = input_array[i][j*sizeof(RGBTRIPLE) + 2];

            int start_col = n*j;
            int end_col = n*(j + 1);
            int start_row = n*i;
            int end_row = n*(i + 1);

            for (int k = start_row; k < end_row ; k++){
                for(int l = start_col ; l < end_col; l++){
                    output_array[k][l*sizeof(RGBTRIPLE)] = triple.rgbtBlue;
                    output_array[k][l*sizeof(RGBTRIPLE) + 1] = triple.rgbtGreen;
                    output_array[k][l*sizeof(RGBTRIPLE) + 2] = triple.rgbtRed;

                }
            }

        }
    }

}

void cpy_arraydata2f(FILE *fp, int f_offset, BYTE** array, int rows_out, int cols_out){
    fseek(fp, f_offset, SEEK_SET);

    for (int i = 0; i < rows_out; i++){
        for (int j = 0; j < cols_out; j++){
            RGBTRIPLE triple;
            triple.rgbtBlue = array[i][j*sizeof(RGBTRIPLE)];
            triple.rgbtGreen = array[i][j*sizeof(RGBTRIPLE) + 1];
            triple.rgbtRed = array[i][j*sizeof(RGBTRIPLE) + 2];
            fwrite(&triple, sizeof(RGBTRIPLE), 1, fp);
        }
        add_padding(fp, sizeof(RGBTRIPLE), cols_out);
    }
}

void print_array(BYTE** array, int elmnt_size, int rows, int cols){
    /** This function takes a BYTE ** and prints the data it points to in the form
     * of a 2D array.
     * array: pointer used for pointing to 2D array.
     * elmnt_size: size of each element in the array (number of bytes)
     * rows, cols: number of rows and columns to be printed
     */
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            for (int k = 0; k < elmnt_size; k++){
                printf("%02X", array[i][j*elmnt_size + k]);
            }
            if (j != (cols - 1)){
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void print_bmp_file_info(BITMAPFILEHEADER bf, BITMAPINFOHEADER bi){

    // First print .bmp file header
    printf("FILE HEADER \n");
    printf("bfType: %i\n", bf.bfType);
    printf("bfSize: %i\n", bf.bfSize);
    printf("bfOffBits:%i \n", bf.bfOffBits);

    printf("INFO HEADER \n");
    printf("biSize: %i\n", bi.biSize);
    printf("biWidth: %i\n", bi.biWidth);
    printf("biHeight: %i\n", bi.biHeight);
    printf("biPlanes: %i\n", bi.biPlanes);
    printf("biBitCount: %i\n", bi.biBitCount);
    printf("biCompression: %i\n", bi.biCompression);
    printf("biSizeImage: %i\n", bi.biSizeImage);
    printf("biXPelsPerMeter: %i\n", bi.biXPelsPerMeter);
    printf("biYPelsPerMeter: %i\n", bi.biYPelsPerMeter);
    printf("biClrUsed: %i\n", bi.biClrUsed);
    printf("biClrImportant: %i\n", bi.biClrImportant);

}
