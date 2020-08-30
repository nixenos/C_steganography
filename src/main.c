/*
 * @file main.c
 * @author Wojciech Janota
 * @brief plik główny
 * @version 0.1
 * @date 2020-05-02
 *
 * @copyright Copyright (c) Wojciech Janota 2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/functions.h"

int main(int argc, char* argv[]) {
    /* WSKAŹNIKI NA PLIKI */
    FILE* sourceImage;
    FILE* destinationImage;
    FILE* sourceFile;
    FILE* resultFile;
    char sourceImageFilename[200];
    char destinationImageFilename[200];
    char sourceFileFilename[200];
    char resultFileFilename[200];
    uint8_t* pixelArray;
    uint8_t* dataArray;
    BMPHEADER bmpHeader;
    DIBHEADER dibHeader;
    int bitsPerPixel;
    int pixelCount;
    int dataArraySize;
    int sourceFileSize;
    int i = 0;
    if (argc < 2) {
        printf("Zbyt mało argumentów wywołania programu!\n");
        printf("Aby wyświetlić pomoc, wywołaj program z flagą -h\n");
        return 1;
    }
    if(argc==2){
        if(strcmp(argv[1], "-h")==0) {
            printf("Pomoc:\n");
            printf(
                "Pierwszy argument wywołania programu powninien zawierać następujące flagi: -w lub -r\n");
            printf("Flaga -w służy do zapisywania danych w obrazie\n"
                "Flagi, które są po niej wymagane są nastepujące:\n"
                "-i -- po niej należy podać plik z danymi, które program ma ukryć\n"
                "-p -- po niej należy podać plik obrazu, w którym ma zostać ukryty plik\n"
                "-o -- po niej należy podać plik wynikowy, w którym będzie ukryty plik\n"
                "-b -- po niej należy podać na ilu bitach ma zostać zapisana informacja (obsługiwane liczby bitów: 1, 2, 4, 8)\n");
            printf("Flaga -r służy do odczytywania danych z obrazu\n"
                "Flagi, które są po niej wymagane są następujące:\n"
                "-p -- po niej nalezy podać plik obrazu, z którego ma zostać odczytany ukryty plik\n"
                "-o -- po niej należy podać plik wynikowy, do którego na zostać odczytana informacja\n"
                "-b -- po niej nalezy podać na ilu bitach została zapisana ukryta informacja\n");
            printf("Obsługiwane pliki BMP: 16bit[RGB(1,5,5,5)], 24bit[RGB(8,8,8)], 32bit[ARGB(8,8,8,8), XRBG(8,8,8,8)]\n");
        }
        else{
            printf("Błędna flaga, aby wyświetlić pomoc, wywołaj program z flagą -h\n");
        }
    }
    if (argc > 2) {
        if (strcmp(argv[1], "-w") == 0) {
            printf("Zapisywanie pliku do pliku BMP\n");
            if (argc != 10) {
                printf("Zła liczba argumentów wywołania programu!\n");
                return 1;
            }
            for (i = 2; i < argc; i++) {
                if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
                    i++;
                    strcpy(sourceFileFilename, argv[i]);
                } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
                    i++;
                    strcpy(sourceImageFilename, argv[i]);
                } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
                    i++;
                    strcpy(destinationImageFilename, argv[i]);
                } else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
                    i++;
                    bitsPerPixel = atoi(argv[i]);
                } else {
                    printf("Niepoprawna flaga przekazana do programu!\n");
                    return 1;
                }
            }
            sourceImage = fopen(sourceImageFilename, "rb");
            if (sourceImage == 0) {
                printf("Błąd: nie można otworzyć obrazu o nazwie: %s\n",
                       sourceImageFilename);
                return 1;
            }
            sourceFile = fopen(sourceFileFilename, "rb");
            if (sourceFile == 0) {
                printf("Nie można otworzyć pliku o nazwie: %s\n",
                       sourceFileFilename);
                return 1;
            }
            destinationImage = fopen(destinationImageFilename, "wb");
            if (destinationImage == 0) {
                printf("Nie można utworzyć pliku wynikowego o nazwie: %s\n",
                       destinationImageFilename);
                return 1;
            }
            if(read_headers(sourceImage, &bmpHeader, &dibHeader)==0){
                printf("Błąd odczytu nagłówków z pliku!\n");
                fclose(sourceFile);
                fclose(destinationImage);
                fclose(sourceImage);
                return 1;
            }
            pixelCount = dibHeader.WIDTH * dibHeader.HEIGHT;
            printf("Podstawowe informacje o pliku wejściowym:\n");
            printf("NAGŁÓWEK PLIKU: %s\n", bmpHeader.HEADER);
            printf("OFFSET TABELI PIXELI: %d\n", bmpHeader.OFFSET);
            printf("GŁĘBIA KOLORÓW: %d\n", dibHeader.BITSPERPIXEL);
            printf("PIXELE: %d\n", pixelCount);
            printf("KOMPRESJA: %d\n", dibHeader.COMPRESSION);
            printf("SZEROKOŚĆ: %d\n", dibHeader.WIDTH);
            printf("WYSOKOŚĆ: %d\n", dibHeader.HEIGHT);
            switch (dibHeader.BITSPERPIXEL) {
                case 16:
                    pixelArray = pixelArray_read_16bit(sourceImage, pixelCount);
                    break;
                case 24:
                    pixelArray = pixelArray_read_24bit(sourceImage, pixelCount);
                    break;
                case 32:
                    pixelArray = pixelArray_read_32bit(sourceImage, pixelCount);
                    break;
                default:
                    pixelArray = 0;
                    break;
            }
            printf("DEBUG FILESIZE HEADER: %d\n", bmpHeader.SIZE);
            printf("DEBUG FILESIZE CALCULATE: %d\n", sizeof(bmpHeader)+sizeof(dibHeader)+pixelCount*3);
            dataArray = read_file_to_memory(sourceFile, &sourceFileSize,
                                            &dataArraySize);
            if(pixelArray) {
                if(dataArray) {
                    if(dataArraySize>65535){
                        printf("Nie można zapisać pliku większego niż 65kB!\n");
                        return 1;
                    }
                    write_data_to_image(
                        pixelArray, dataArray, bmpHeader, dibHeader,
                        destinationImage, sourceFileSize, bitsPerPixel,
                        dibHeader.BITSPERPIXEL, pixelCount, dataArraySize);
                    fclose(sourceImage);
                    fclose(sourceFile);
                    fclose(destinationImage);
                    free(pixelArray);
                    free(dataArray);
                }
                else{
                    printf("Błąd: brak tablicy bajtów!\n");
                    free(pixelArray);
                    fclose(sourceImage);
                    fclose(sourceFile);
                    fclose(destinationImage);
                    return 1;
                }
            }
            else{
                printf("Błąd: brak tablicy pixeli!\n");
                fclose(sourceImage);
                fclose(sourceFile);
                fclose(destinationImage);
                return 1;
            }
            free(bmpHeader.HEADER);
            free(bmpHeader.RESERVED1);
            return 0;
        } else if (strcmp(argv[1], "-r") == 0) {
            printf("Odczytywanie ukrytego pliku z pliku BMP\n");
            for (i = 2; i < argc; i++) {
                if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
                    i++;
                    strcpy(sourceImageFilename, argv[i]);
                } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
                    i++;
                    strcpy(resultFileFilename, argv[i]);
                } else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
                    i++;
                    bitsPerPixel = atoi(argv[i]);
                } else {
                    printf("Niepoprawna flaga przekazana do programu!\n");
                    return 1;
                }
            }
            sourceImage = fopen(sourceImageFilename, "rb");
            resultFile = fopen(resultFileFilename, "wb");
            if (sourceImage == 0) {
                printf("Nie można otworzyć pliku obrazu o nazwie: %s\n",
                       sourceImageFilename);
                return 1;
            }
            if (resultFile == 0) {
                printf("Nie można otworzyć pliku wyjściowego o nazwie: %s",
                       resultFileFilename);
                return 1;
            }
            if(read_headers(sourceImage, &bmpHeader, &dibHeader)==0){
                printf("Błąd odczytu nagłówków!\n");
                return 1;
            }
            pixelCount = dibHeader.WIDTH * dibHeader.HEIGHT;
            switch (dibHeader.BITSPERPIXEL) {
                case 16:
                    pixelArray = pixelArray_read_16bit(sourceImage, pixelCount);
                    break;
                case 24:
                    pixelArray = pixelArray_read_24bit(sourceImage, pixelCount);
                    break;
                case 32:
                    pixelArray = pixelArray_read_32bit(sourceImage, pixelCount);
                    break;
                default:
                    pixelArray = 0;
                    break;
            }
            if(pixelArray) {
                write_data_from_image(bmpHeader, pixelArray, resultFile,
                                      dibHeader, bitsPerPixel);
                fclose(sourceImage);
                fclose(resultFile);
                free(pixelArray);
            }
            else{
                printf("Błąd: brak tablicy pixeli!\n");
                fclose(sourceImage);
                fclose(resultFile);
                return 1;
            }
            free(bmpHeader.HEADER);
            free(bmpHeader.RESERVED1);
            return 0;
        } else {
            printf(
                "Pierwszy argument powinien zawierać następujące flagi: -r, "
                "-w\n");
            return 1;
        }
    }
    return 1;
}