#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <locale>
#include <QString>
#include <QFile>
#include <QVector>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>

#pragma pack(push, 1)
static const char *operations[] = {
    "Negative\n",
    "White&Black\n",
    "Median filter\n",
    "Gamma correction\n",
    "Exit\n"
};

enum operation {
    NEGATIVE,
    WNB,
    MEDIAN,
    GAMMA,
    EXIT
};

struct headerFileBitMap {
    unsigned short int bfType;
    unsigned int bfSize;
    unsigned short int reserved1;
    unsigned short int reserved2;
    unsigned int bfOffBits;
};

struct infoHeaderBitMap {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short int biPlanes;
    unsigned short int biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
};

struct pixelBitMap {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

#pragma pack(pop)

int checkBitCount(infoHeaderBitMap info, int *lessThanEight);
pixelBitMap getPixel(const QVector<pixelBitMap> &arrayOfPix, int height, int width, int y, int x);
QString formName(const QString &nameOfFile, const QString &add);
void wnb(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName);
void negative(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName);
void gammaCorrection(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName);
void medianFilter(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName);
void blurImage(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName, int radius);
void adjustBrightness(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName, int delta);
#endif // FUNCTIONS_H
