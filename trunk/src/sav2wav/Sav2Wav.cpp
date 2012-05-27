/*  This file is part of UKNCBTL.
    UKNCBTL is free software: you can redistribute it and/or modify it under the terms
of the GNU Lesser General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
    UKNCBTL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License along with
UKNCBTL. If not, see <http://www.gnu.org/licenses/>. */

// Sav2Wav.cpp
//
// Originally written by Alexander Alexandrow aka BYTEMAN
// mailto: sash-a@nm.ru
//
// Converted from Delphi 7 to C++ by Nikita Zimin
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <memory.h>
#include <string.h>


#define BYTE unsigned char
#define WORD unsigned short


static const BYTE chunk1[] = {
    0x52, 0x49, 0x46, 0x46, 0x0, 0x0, 0x0, 0x0, 'W','A','V','E','f','m','t',' ', 0x10, 0x0, 0x0, 0x0, 0x1, 0x0, 0x1, 0x0, 
    0x22, 0x56, 0x0, 0x0, 0x22, 0x56, 0x0, 0x0, 0x1, 0x0, 0x8, 0x0, 0x64, 0x61, 0x74, 0x61, 0x0, 0x0, 0x0, 0x0
};
static const BYTE bit1[] = {
    0x11, 0x5f, 0xbf, 0xd8, 0xf2, 0xd5, 0x5a, 0x2a, 0x1b, 0x11, 0x5f, 0xbf, 0xd8, 0xf2, 0xd5, 0x5a, 0x2a, 0x1b };
static const BYTE bit0[] = {
    0x63, 0xb8, 0xd3, 0xe4, 0xea, 0xef, 0xec, 0xef, 0xd8, 0x6c, 0x2a, 0x19, 0x0f, 0x0f, 0x10, 0x11, 0x26 };
static const BYTE pilot[] = {
    0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8,
    0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8,
    0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8,
    0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8,
    0xa8, 0x9c, 0x51, 0x25, 0x1b, 0x12, 0x10, 0x0f, 0x0d, 0x0d, 0x0d, 0x0d, 0x0f, 0x0f, 0x0f, 0x0f,
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x12, 0x12, 0x12, 0x12,
    0x12, 0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x1b, 0x1b,
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1e,
    0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
    0x1f, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
    0x22, 0x22, 0x22, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x25, 0x25, 0x25, 0x25,
    0x25, 0x25, 0x25, 0x25, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x28, 0x28, 0x28,
    0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2b, 0x49,
    0xa2, 0xb9, 0xc2, 0xc5, 0xc5, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,
    0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5,
    0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5,
    0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5,
    0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5,
    0xc5, 0xc3, 0xc5, 0xc3, 0xc5, 0xc5, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2,
    0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xbf, 0x89, 0x46, 0x39,
    0x2d, 0x2a, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b,
    0x2b, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
    0x2e, 0x2e, 0x2e, 0x2e, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x31,
    0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
    0x36, 0x36, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x39, 0x39,
    0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a,
    0x3a, 0x3a, 0x3a, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3d, 0x3d, 0x3d,
    0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f,
    0x3f, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x4c, 0x9c, 0xce, 0xd4, 0xdb, 0xdb,
    0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb,
    0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb,
    0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda,
    0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda,
    0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8,
    0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8,
    0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd7,
    0xd7, 0xd8, 0xd7, 0xd7, 0xd7, 0xd8, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7,
    0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7, 0xd7,
    0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5,
    0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5
};

static BYTE const loader[] = {
    0xa0, 0x00, 0x21, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xac, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8c, 0x00, 0xe0, 0x00, 0xa0, 0x00, 0xe0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x38, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdf, 0x0b, 0x72, 0xff, 0x02, 0x00, 0x00, 0x00,
    0x44, 0x00, 0x80, 0x00, 0x02, 0x00, 0xc6, 0x15, 0x00, 0x02, 0xc0, 0x15, 0x0c, 0x01, 0x04, 0x00,
    0xc0, 0x15, 0xff, 0xff, 0x01, 0x7e, 0xc0, 0x15, 0xba, 0x00, 0x00, 0x88, 0xf7, 0x8b, 0x5e, 0x00,
    0x0c, 0x03, 0xd7, 0xad, 0x58, 0x00, 0xff, 0x00, 0x03, 0x02, 0xc1, 0x15, 0x01, 0x01, 0x02, 0x01,
    0xc1, 0x15, 0xf5, 0x00, 0x00, 0x89, 0x00, 0x00, 0xfe, 0x01, 0xc6, 0x17, 0x22, 0x00, 0xdf, 0x55,
    0x40, 0x00, 0x70, 0xff, 0x17, 0x8d, 0x00, 0x00, 0xc7, 0x17, 0x20, 0x00, 0xc1, 0x15, 0x05, 0x00,
    0x02, 0x01, 0x1f, 0x94, 0xbe, 0xfd, 0xdf, 0x8b, 0xbc, 0xfd, 0xfd, 0x80, 0x46, 0x7e, 0x02, 0x00,
    0xc0, 0x15, 0xec, 0x00, 0x04, 0x00, 0x40, 0x10, 0x04, 0x00, 0x02, 0x00, 0xdf, 0x8b, 0x74, 0xff,
    0xfd, 0x80, 0x1f, 0x94, 0x76, 0xff, 0xfa, 0x02, 0x02, 0x00, 0xbe, 0x00, 0xff, 0xff, 0x00, 0x08,
    0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x54, 0x41, 0x50,
    0x45, 0x2d, 0x55, 0x2d, 0x00, 0x49, 0x2f, 0x4f, 0x20, 0x65, 0x72, 0x72, 0x6f, 0x72, 0x0d, 0x0a,
    0x00, 0x42, 0x72, 0x65, 0x61, 0x6b, 0x2e, 0x2e, 0x2e, 0x0d, 0x0a, 0x00, 0x4c, 0x6f, 0x61, 0x64,
    0x69, 0x6e, 0x67, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


char inputfilename[256];
char outputfilename[256];
FILE* inputfile;
FILE* outputfile;
int samplerate = 22050;
unsigned long datalength;
BYTE load[512];
char name1[16];


void PauseWrite(int n)  // ������ �����
{
    const BYTE wr = 0x7f;
    for (int i = 1; i < n + 1; ++i)
        fwrite(&wr, 1, 1, outputfile);
}

void ByteWrite(BYTE n)
{
    unsigned int k;

    fwrite(bit0, 1, sizeof(bit0), outputfile);

    for (k = 0; k < 8; ++k)
    {
        // �������� ������������
        if ((n & 1) == 0)
            fwrite(bit0, 1, sizeof(bit0), outputfile);
        else
            fwrite(bit1, 1, sizeof(bit1), outputfile);
        n = n >> 1;
    }

    fwrite(bit1, 1, sizeof(bit1), outputfile);
    fwrite(bit1, 1, sizeof(bit1), outputfile);
}

void WordWrite(WORD n)
{
    ByteWrite( (BYTE)(n & 0xff) );
    ByteWrite( (BYTE)(n >> 8) );
}

// Colors from HERE:
// http://kisly-alexey.newmail.ru/UKNC/TAPE/
void PrepareLoader()
{
    WORD i;
    BYTE t1, t2;

    memcpy(load, loader, 512);

    // YELLOW color
    // ���������� ������ � ���������
    fseek(inputfile, 32, SEEK_SET);
    fread(&t1, 1, 1, inputfile);
    load[ 32 ] = t1;
    fread(&t1, 1, 1, inputfile);
    load[ 33 ] = t1;
    fread(&t1, 1, 1, inputfile);
    load[ 34 ] = t1;
    fread(&t1, 1, 1, inputfile);
    load[ 35 ] = t1;

    fseek(inputfile, 41, SEEK_SET);
    fread(&t1, 1, 1, inputfile);
    load[ 41 ] = t1;
    fread(&t1, 1, 1, inputfile);
    load[ 42 ] = t1;

    fseek(inputfile, 320, SEEK_SET);
    for (i = 320; i < 512; ++i)
    {
        fread(&t1, 1, 1, inputfile);
        load[ i ] = t1;
    }

    // GREEN color
    fseek(inputfile, 276, SEEK_SET);
    for ( i = 0; i < 6; ++i)
        load[ 276 + i ] = name1[ i ];
    load[ 283 - 7 + i ] = (BYTE)13;
    load[ 284 - 7 + i ] = (BYTE)10;
    load[ 285 - 7 + i ] = (BYTE)0;

    // BLUE color
    for (i = 0; i < 16; ++i)
        load[ 194 + i ] = name1[ i ];

    // RED color
    fseek(inputfile, 40, SEEK_SET);
    fread(&t1, 1, 1, inputfile);
    fread(&t2, 1, 1, inputfile);
    datalength = ( ( (WORD)( t1 ) + (WORD)( t2 ) * 256 ) - 510 ) / 2;
    load[ 213 ] = (BYTE)( datalength / 256 );
    load[ 212 ] = (BYTE)( datalength - ( ( datalength / 256 ) * 256 ) );
}

void DefWaveFormat()  // ����� ��������� WAV
{
    unsigned long length1, length2;

    fseek(outputfile, 0, SEEK_END);
    long streamsize = ftell(outputfile);

    fseek(outputfile, 4, SEEK_SET);
    length1 = (unsigned long)streamsize - 8;
    length2 = (unsigned long)streamsize - 44;
    fwrite(&length1, 1, sizeof(length1), outputfile);
    fseek(outputfile, 40, SEEK_SET);
    fwrite(&length2, 1, sizeof(length2), outputfile);

    fseek(outputfile, 24, SEEK_SET);
    fwrite(&samplerate, 1, sizeof(samplerate), outputfile);
    fseek(outputfile, 28, SEEK_SET);
    fwrite(&samplerate, 1, sizeof(samplerate), outputfile);
}

void Convert()
{
    BYTE n;
    WORD i;
    unsigned long checksum;

    inputfile = fopen(inputfilename, "rb");
    if (inputfile == NULL)
    {
        printf("Failed to open input file.");
        return;
    }

    outputfile = fopen(outputfilename, "w+b");
    if (outputfile == NULL)
    {
        printf("Failed to open output file.");
        return;
    }
    
    PrepareLoader();

    fwrite(chunk1, 1, sizeof(chunk1), outputfile);  // ����� ��������� WAV-�����

    fwrite(pilot, 1, sizeof(pilot), outputfile);  // ����� ��������� ��� �����-����

    // ����� ��������� ����������
    for ( i = 0; i < 8000; ++i )  // �����-��� ����� ���������� ������������� 8000 ����� "1"
        fwrite(bit1, 1, sizeof(bit1), outputfile);
    for ( i = 0; i < 16; ++i )  // 16 ���� ����� �����
        ByteWrite( 0 );
    WordWrite( 256 );       // 1 ����� ����� ������ � ������
    WordWrite( 0 );         // 1 ����� ������ �������� ������
    for ( i = 0; i < 2000; ++i )  // �����-��� ����� ������� ������������� 2000 ����� "1"
        fwrite(bit1, 1, sizeof(bit1), outputfile);

    // ����� ������ ����������
    checksum = 0;
    for (i = 0; i < 256; ++i)
    {
        ByteWrite( ( load[ i * 2 ] ) );  // ����� loader...
        ByteWrite( ( load[ i * 2 + 1 ] ) );
        checksum = checksum + (WORD)( load[ i * 2 ] ) + ((WORD)( load[ i * 2 + 1 ] )) * 256;
        checksum = checksum - checksum / 65536 * 65536 + checksum / 65536;
    }
    WordWrite( (WORD)checksum );  // ����� ��

    // ����� ����� �������
    PauseWrite( 11000 );
    // ������� ��������
    fwrite(pilot, 1, sizeof(pilot), outputfile);
    
    for ( i = 1; i < 8001; ++i )  // �����-��� ����� ���������� ������������� 8000 ����� "1"
        fwrite(bit1, 1, sizeof(bit1), outputfile);

    for ( i = 0; i < 16; ++i )  // 16 ���� ����� �����
        ByteWrite( ( name1[ i ] ) );
    WordWrite( (WORD)datalength );    // 1 ����� ����� ������ � ������
    WordWrite( 512 );           // 1 ����� ������ �������� ������

    for ( i = 0; i < 2000; ++i )  // �����-��� ����� ������� ������������� 2000 ����� "1"
        fwrite(bit1, 1, sizeof(bit1), outputfile);
    
    fseek(inputfile, 512, SEEK_SET);  //inputstream.Position = 512;
    checksum = 0;
    for ( i = 0; i < datalength; ++i )
    {
        fread(&n, 1, 1, inputfile);
        ByteWrite( n );
        checksum = checksum + (WORD)( n );

        fread(&n, 1, 1, inputfile);
        ByteWrite( n );
        checksum = checksum + ( ((WORD)( n )) * 256 );
        checksum = checksum - checksum / 65536 * 65536 + checksum / 65536;
    }
    WordWrite( (WORD)checksum );

    fclose(inputfile);

    DefWaveFormat();  // ��������� ����� � ��������� WAV-�����

    fseek(outputfile, 0, SEEK_END);
    long streamsize = ftell(outputfile);
    printf("WAV file size:\t%ld bytes\n", streamsize);

    fclose(outputfile);

    printf("Done.");
}


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Usage: sav2wav <inputfile.SAV> <outputfile.WAV>");
        return 255;
    }

    strcpy(inputfilename, argv[1]);
    strcpy(outputfilename, argv[2]);

    // Prepare program name
    memset(name1, ' ', 16);
    char* nameend = strchr(inputfilename, '.');
    int namelen = (nameend == NULL) ? strlen(inputfilename) : nameend - inputfilename;
    strncpy(name1, inputfilename, namelen);

    printf("Input:\t\t%s\n", inputfilename);
    printf("Output:\t\t%s\n", outputfilename);
    printf("Name:\t\t%s\n", name1);

    Convert();

	return 0;
}
