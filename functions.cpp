#include "functions.h"

int checkBitCount(infoHeaderBitMap info, int *lessThanEight) {
    if (info.biBitCount == 24 || info.biBitCount == 16) {
        return 0;
    } else if (info.biBitCount == 4 || info.biBitCount == 2 || info.biBitCount == 1) {
        *lessThanEight = 1;
        return 0;
    } else {
        return 1;
    }
}

pixelBitMap getPixel(const QVector<pixelBitMap> &arrayOfPix, int height, int width, int y, int x) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return arrayOfPix[y * width + x];
    } else {
        pixelBitMap nullPix = {0, 0, 0};
        return nullPix;
    }
}

QString formName(const QString &nameOfFile, const QString &add) {
    QString result = "./" + add + nameOfFile.mid(nameOfFile.lastIndexOf('/') + 1);
    return result;
}

    void wnb(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName) {
        QFile result(resultName);
        if (!result.open(QIODevice::WriteOnly)) {
            qWarning("Cannot open file for writing");
            return;
        }

        QDataStream out(&result);
        out.writeRawData(reinterpret_cast<const char*>(&header), sizeof(headerFileBitMap));
        out.writeRawData(reinterpret_cast<const char*>(&info), sizeof(infoHeaderBitMap));

        pixelBitMap pixel;
        for (int y = 0; y < info.biHeight; y++) {
            for (int x = 0; x < info.biWidth; x++) {
                inFile.read(reinterpret_cast<char*>(&pixel), sizeof(pixelBitMap));
                unsigned char average = static_cast<unsigned char>((pixel.green + pixel.blue + pixel.red) / 3);
                pixel.red = average;
                pixel.green = average;
                pixel.blue = average;
                out.writeRawData(reinterpret_cast<const char*>(&pixel), sizeof(pixelBitMap));
            }
        }
    }

    void negative(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName) {
        QFile result(resultName);
        if (!result.open(QIODevice::WriteOnly)) {
            qWarning("Cannot open file for writing");
            return;
        }

        QDataStream out(&result);
        out.writeRawData(reinterpret_cast<const char*>(&header), sizeof(headerFileBitMap));
        out.writeRawData(reinterpret_cast<const char*>(&info), sizeof(infoHeaderBitMap));

        pixelBitMap pixel;
        for (int y = 0; y < info.biHeight; y++) {
            for (int x = 0; x < info.biWidth; x++) {
                inFile.read(reinterpret_cast<char*>(&pixel), sizeof(pixelBitMap));
                pixel.red = 255 - pixel.red;
                pixel.green = 255 - pixel.green;
                pixel.blue = 255 - pixel.blue;
                out.writeRawData(reinterpret_cast<const char*>(&pixel), sizeof(pixelBitMap));
            }
        }
    }

    void gammaCorrection(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName) {
        QFile result(resultName);
        if (!result.open(QIODevice::WriteOnly)) {
            qWarning("Cannot open file for writing");
            return;
        }

        QDataStream out(&result);
        out.writeRawData(reinterpret_cast<const char*>(&header), sizeof(headerFileBitMap));
        out.writeRawData(reinterpret_cast<const char*>(&info), sizeof(infoHeaderBitMap));

        pixelBitMap pixel;
        double gamma = 2.2;
        for (int y = 0; y < info.biHeight; y++) {
            for (int x = 0; x < info.biWidth; x++) {
                inFile.read(reinterpret_cast<char*>(&pixel), sizeof(pixelBitMap));
                pixel.red = static_cast<unsigned char>(pow(pixel.red / 255.0, gamma) * 255);
                pixel.green = static_cast<unsigned char>(pow(pixel.green / 255.0, gamma) * 255);
                pixel.blue = static_cast<unsigned char>(pow(pixel.blue / 255.0, gamma) * 255);
                out.writeRawData(reinterpret_cast<const char*>(&pixel), sizeof(pixelBitMap));
            }
        }
    }

    void medianFilter(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName) {
        QFile result(resultName);
        if (!result.open(QIODevice::WriteOnly)) {
            qWarning("Cannot open file for writing");
            return;
        }

        QDataStream out(&result);
        out.writeRawData(reinterpret_cast<const char*>(&header), sizeof(headerFileBitMap));
        out.writeRawData(reinterpret_cast<const char*>(&info), sizeof(infoHeaderBitMap));

        QVector<pixelBitMap> pixels(info.biHeight * info.biWidth);
        for (int y = 0; y < info.biHeight; y++) {
            for (int x = 0; x < info.biWidth; x++) {
                inFile.read(reinterpret_cast<char*>(&pixels[y * info.biWidth + x]), sizeof(pixelBitMap));
            }
        }

        for (int y = 0; y < info.biHeight; y++) {
            for (int x = 0; x < info.biWidth; x++) {
                QVector<unsigned char> reds, greens, blues;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        pixelBitMap pixel = getPixel(pixels, info.biHeight, info.biWidth, y + dy, x + dx);
                        reds.push_back(pixel.red);
                        greens.push_back(pixel.green);
                        blues.push_back(pixel.blue);
                    }
                }
                std::sort(reds.begin(), reds.end());
                std::sort(greens.begin(), greens.end());
                std::sort(blues.begin(), blues.end());
                pixelBitMap medianPixel = {reds[4], greens[4], blues[4]};
                out.writeRawData(reinterpret_cast<const char*>(&medianPixel), sizeof(pixelBitMap));
            }
        }
    }
    void adjustBrightness(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName, int delta) {
        QFile result(resultName);
        if (!result.open(QIODevice::WriteOnly)) {
            qWarning("Cannot open file for writing");
            return;
        }

        QDataStream out(&result);
        out.writeRawData(reinterpret_cast<const char*>(&header), sizeof(headerFileBitMap));
        out.writeRawData(reinterpret_cast<const char*>(&info), sizeof(infoHeaderBitMap));

        pixelBitMap pixel;
        for (int y = 0; y < info.biHeight; y++) {
            for (int x = 0; x < info.biWidth; x++) {
                inFile.read(reinterpret_cast<char*>(&pixel), sizeof(pixelBitMap));
                pixel.red = std::clamp(pixel.red + delta, 0, 255);
                pixel.green = std::clamp(pixel.green + delta, 0, 255);
                pixel.blue = std::clamp(pixel.blue + delta, 0, 255);
                out.writeRawData(reinterpret_cast<const char*>(&pixel), sizeof(pixelBitMap));
            }
        }
    }
    void blurImage(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName, int radius) {
        QFile result(resultName);
        if (!result.open(QIODevice::WriteOnly)) {
            qWarning("Cannot open file for writing");
            return;
        }

        QDataStream out(&result);
        out.writeRawData(reinterpret_cast<const char*>(&header), sizeof(headerFileBitMap));
        out.writeRawData(reinterpret_cast<const char*>(&info), sizeof(infoHeaderBitMap));

        int width = info.biWidth;
        int height = info.biHeight;
        int paddedWidth = (width * 3 + 3) & ~3;  // Выравнивание до 4 байт

        QVector<uint8_t> inputBuffer(paddedWidth * height);
        inFile.read(reinterpret_cast<char*>(inputBuffer.data()), inputBuffer.size());

        auto getPixel = [&](int x, int y) -> pixelBitMap {
            x = std::clamp(x, 0, width - 1);
            y = std::clamp(y, 0, height - 1);
            uint8_t *pixelData = &inputBuffer[y * paddedWidth + x * 3];
            return { pixelData[0], pixelData[1], pixelData[2] };
        };

        QVector<pixelBitMap> outputBuffer(width * height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int r = 0, g = 0, b = 0, count = 0;

                for (int ky = -radius; ky <= radius; ++ky) {
                    for (int kx = -radius; kx <= radius; ++kx) {
                        pixelBitMap pixel = getPixel(x + kx, y + ky);
                        r += pixel.red;
                        g += pixel.green;
                        b += pixel.blue;
                        count++;
                    }
                }

                pixelBitMap &blurredPixel = outputBuffer[y * width + x];
                blurredPixel.red = r / count;
                blurredPixel.green = g / count;
                blurredPixel.blue = b / count;
            }
        }

        for (int y = 0; y < height; ++y) {
            out.writeRawData(reinterpret_cast<const char*>(&outputBuffer[y * width]), width * 3);
            out.writeRawData("\0\0\0\0", paddedWidth - width * 3);  // Заполнение байтами выравнивания
        }
    }
    void cropImage(QFile &inFile, const headerFileBitMap &header, const infoHeaderBitMap &info, const QString &resultName, int cropX, int cropY, int cropWidth, int cropHeight) {
        QFile result(resultName);
        if (!result.open(QIODevice::WriteOnly)) {
            qWarning("Cannot open file for writing");
            return;
        }

        QDataStream out(&result);

        headerFileBitMap newHeader = header;
        infoHeaderBitMap newInfo = info;

        newInfo.biWidth = cropWidth;
        newInfo.biHeight = cropHeight;
        int paddedWidth = (cropWidth * 3 + 3) & ~3;
        newInfo.biSizeImage = paddedWidth * cropHeight;
        newHeader.bfSize = newHeader.bfOffBits + newInfo.biSizeImage;

        out.writeRawData(reinterpret_cast<const char*>(&newHeader), sizeof(headerFileBitMap));
        out.writeRawData(reinterpret_cast<const char*>(&newInfo), sizeof(infoHeaderBitMap));

        int inputPaddedWidth = (info.biWidth * 3 + 3) & ~3;
        QVector<uint8_t> inputBuffer(inputPaddedWidth * info.biHeight);
        inFile.read(reinterpret_cast<char*>(inputBuffer.data()), inputBuffer.size());

        QVector<uint8_t> outputBuffer(paddedWidth * cropHeight);

        for (int y = 0; y < cropHeight; ++y) {
            for (int x = 0; x < cropWidth; ++x) {
                int srcX = cropX + x;
                int srcY = cropY + y;
                uint8_t *srcPixel = &inputBuffer[srcY * inputPaddedWidth + srcX * 3];
                uint8_t *dstPixel = &outputBuffer[y * paddedWidth + x * 3];
                std::copy(srcPixel, srcPixel + 3, dstPixel);
            }
        }

        out.writeRawData(reinterpret_cast<const char*>(outputBuffer.data()), outputBuffer.size());
    }



