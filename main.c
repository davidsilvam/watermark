#include <stdio.h>
#include <stdlib.h>

int main() {
    int posX, posY, i;
    float ad = 0.5, as = 1;
    char widthTextBuffer[4];
    char heightTextBuffer[4];
    int widthImgDest, heightImgDest;
    int widthImgSour, heightImgSour;
    FILE *fp;

    //////////////////////////////////////////////////////////////////////////
    // load destination image
    fp = fopen("destination.ppm", "rb");
    if (fp == NULL) {
        printf("Erro ao tentar carregar o arquivo!");
        return 1;
    }

    // read dimensions
    fseek(fp, 3, SEEK_CUR);

    for (i = 0; i < 4; i++) {
        fread(widthTextBuffer + i, sizeof(char), 1, fp);
        if (widthTextBuffer[i] == ' ') {
            break;
        }
    }
    if (i == 4) {
        printf("Erro: Imagem com dimensões não suportadas!");
        return 1;
    }
    widthTextBuffer[i] = '\0';

    for (i = 0; i < 4; i++) {
        fread(heightTextBuffer + i, sizeof(char), 1, fp);
        if (heightTextBuffer[i] == '\n') {
            break;
        }
    }
    if (i == 4) {
        printf("Erro: Imagem com dimensões não suportadas!");
        return 1;
    }
    heightTextBuffer[i] = '\0';

    widthImgDest = atoi(widthTextBuffer);
    heightImgDest = atoi(heightTextBuffer);

    // fill map pixels
    unsigned char mapPixelsDest[heightImgDest][widthImgDest][3];
    fseek(fp, 4, SEEK_CUR);

    for (posX = 0, posY = -1; posX != widthImgDest + 1 && posY != 78; posX++) {
        posX %= widthImgDest;
        if (posX == 0) {
            posY += 1;
        }

        fread(&mapPixelsDest[posY][posX][0], sizeof(char), 1, fp);
        fread(&mapPixelsDest[posY][posX][1], sizeof(char), 1, fp);
        fread(&mapPixelsDest[posY][posX][2], sizeof(char), 1, fp);
    }

    //
    fclose(fp);

    //////////////////////////////////////////////////////////////////////////
    // load source image
    fp = fopen("source.ppm", "rb");
    if (fp == NULL) {
        printf("Erro ao tentar carregar o arquivo!");
        return 1;
    }

    // read dimensions
    fseek(fp, 3, SEEK_CUR);

    for (i = 0; i < 4; i++) {
        fread(widthTextBuffer + i, sizeof(char), 1, fp);
        if (widthTextBuffer[i] == ' ') {
            break;
        }
    }
    if (i == 4) {
        printf("Erro: Imagem com dimensões não suportadas!");
        return 1;
    }
    widthTextBuffer[i] = '\0';

    for (i = 0; i < 4; i++) {
        fread(heightTextBuffer + i, sizeof(char), 1, fp);
        if (heightTextBuffer[i] == '\n') {
            break;
        }
    }
    if (i == 4) {
        printf("Erro: Imagem com dimensões não suportadas!");
        return 1;
    }
    heightTextBuffer[i] = '\0';

    widthImgSour = atoi(widthTextBuffer);
    heightImgSour = atoi(heightTextBuffer);

    // fill map pixels
    unsigned char mapPixelsSour[heightImgSour][widthImgSour][3];
    fseek(fp, 4, SEEK_CUR);
    for (posX = 0, posY = -1; posX != widthImgSour + 1 && posY != 78; posX++) {
        posX %= widthImgSour;
        if (posX == 0) {
            posY += 1;
        }

        fread(&mapPixelsSour[posY][posX][0], sizeof(char), 1, fp);
        fread(&mapPixelsSour[posY][posX][1], sizeof(char), 1, fp);
        fread(&mapPixelsSour[posY][posX][2], sizeof(char), 1, fp);
    }

    //
    fclose(fp);

    //////////////////////////////////////////////////////////////////////////
    // blend
    fp = fopen("output.ppm", "wb");
    fputs("P6\n81 78\n255\n", fp); // todo: usar as dimensões de acordo com a maior imagem!

    for (posX = 0, posY = -1; posX != widthImgDest + 1 && posY != 78; posX++) {
        posX %= widthImgDest;
        if (posX == 0) {
            posY += 1;
        }

        if (widthImgSour > posX && heightImgSour > posY) {
            fputc((char) (mapPixelsDest[posY][posX][0] * ad + mapPixelsSour[posY][posX][0] * as * (1 - ad)), fp);
            fputc((char) (mapPixelsDest[posY][posX][1] * ad + mapPixelsSour[posY][posX][1] * as * (1 - ad)), fp);
            fputc((char) (mapPixelsDest[posY][posX][2] * ad + mapPixelsSour[posY][posX][2] * as * (1 - ad)), fp);
        } else {
            fputc(mapPixelsDest[posY][posX][0], fp);
            fputc(mapPixelsDest[posY][posX][1], fp);
            fputc(mapPixelsDest[posY][posX][2], fp);
        }
    }

    fclose(fp);

    return 0;
}
