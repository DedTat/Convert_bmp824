/// Convert bmp from 8 bit to 24 bit

#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

int main()
 {
     string sFileName;
     BITMAPFILEHEADER bfh;
     BITMAPINFOHEADER bih;
     int Width, Height;
     RGBQUAD Palette[256];
     BYTE *inBuf;
     RGBTRIPLE *outBuf;
     HANDLE hIn, hOut;
     DWORD RW;
     DWORD OffBits;

     cout << "Enter the full name, please: ";
     cin >> sFileName;
     int i, j;

     hIn = CreateFile (sFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
     if (hIn == INVALID_HANDLE_VALUE)
         return 0;

     hOut = CreateFile ("Result.bmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
     if (hOut == INVALID_HANDLE_VALUE)
     {
         CloseHandle (hIn);
         return 0;
     }

     // Прочтем данные
     ReadFile (hIn, &bfh, sizeof(bfh), &RW, NULL);
     ReadFile (hIn, &bih, sizeof(bih), &RW, NULL);
     ReadFile (hIn, Palette, 256 * sizeof(RGBQUAD), &RW, NULL);

     // Установим указатель на начало растра
     SetFilePointer (hIn, bfh.bfOffBits, NULL, FILE_BEGIN);
     Width = bih.biWidth;
     Height = bih.biHeight;
     OffBits = bfh.bfOffBits;

     // Выделим память
     inBuf = new BYTE [Width];
     outBuf = new RGBTRIPLE [Width];

     // Заполним заголовки
     bfh.bfOffBits = sizeof (bfh) + sizeof (bih);                               // Не будем писать палитру
     bih.biBitCount = 24;
     bfh.bfSize = bfh.bfOffBits + 3 * Width * Height + Height * (Width % 4);    // Размер файла

     // А остальное не меняется
     // Запишем заголовки
     WriteFile (hOut, &bfh, sizeof(bfh), &RW, NULL);
     WriteFile (hOut, &bih, sizeof(bih), &RW, NULL);

     // Начнем преобразовывать
     for (i = 0; i < Height; i++)
     {
         ReadFile (hIn, inBuf, Width, &RW, NULL);
         for (j = 0; j < Width; j++)
         {
             outBuf[j].rgbtRed = Palette[inBuf[j]].rgbRed;
             outBuf[j].rgbtGreen = Palette[inBuf[j]].rgbGreen;
             outBuf[j].rgbtBlue = Palette[inBuf[j]].rgbBlue;
         }
         WriteFile (hOut, outBuf, sizeof(RGBTRIPLE) * Width, &RW, NULL);

         // Пишем мусор для выравнивания
         WriteFile (hOut, Palette, Width % 4, &RW, NULL);
         SetFilePointer (hIn, (3 * Width) % 4, NULL, FILE_CURRENT);
     }

     delete inBuf;
     delete outBuf;
     CloseHandle (hIn);
     CloseHandle (hOut);
    // return;
    cout << "Updating has come to the end successfully!";
system("pause");
 }
