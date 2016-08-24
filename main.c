#include <stdio.h>
#include <stdlib.h>

int main() {
    int posX, posY;

    FILE *fp;
    fp = fopen("/home/macabeus/ApenasMeu/Dropbox/CEFET/SistemasEmbarcados/marcadagua/foo.ppm", "rb");
    if (fp == NULL) {
        printf("Erro ao tentar carregar o arquivo!");
        return 1;
    }

    //unsigned char myfile[3] = {0x80, 0x7F, 0x84};

    ////
    // ler dimenssões da imagem para gerar o mapa de pixels
    long i;
    char widthText[4];
    char heightText[4];
    int widthImg1;
    int heightImg1;
    fseek(fp, 3, SEEK_CUR);

    for (i = 0; i < 4; i++) {
        fread(widthText + i, sizeof(char), 1, fp);
        if (widthText[i] == ' ') {
            break;
        }
    }
    if (i == 4) {
        printf("Erro: Imagem com dimensões não suportadas!");
        return 1;
    }
    widthText[i] = '\0';

    for (i = 0; i < 4; i++) {
        fread(heightText + i, sizeof(char), 1, fp);
        if (heightText[i] == '\n') {
            break;
        }
    }
    if (i == 4) {
        printf("Erro: Imagem com dimensões não suportadas!");
        return 1;
    }
    heightText[i] = '\0';

    widthImg1 = atoi(widthText);
    heightImg1 = atoi(heightText);

    //unsigned char mapPixels[widthImg1 * heightImg1 * 3];
    unsigned char mapPixels[heightImg1][widthImg1][3];

    // preencher mapa de pixels
    fseek(fp, 4, SEEK_CUR);

    for (posX = 0, posY = -1; posX != widthImg1 + 1 && posY != 78; posX++) {
        posX %= widthImg1;
        if (posX == 0) {
            posY += 1;
        }

        fread(&mapPixels[posY][posX][0], sizeof(char), 1, fp);
        fread(&mapPixels[posY][posX][1], sizeof(char), 1, fp);
        fread(&mapPixels[posY][posX][2], sizeof(char), 1, fp);
    }

    //
    fclose(fp);

    // ler arquivo da segunda imagem
    // TODO: código abaixo é o mesmo de cima! não é bom ter código repetido!
    int widthImg2;
    int heightImg2;

    fp = fopen("/home/macabeus/ApenasMeu/Dropbox/CEFET/SistemasEmbarcados/marcadagua/baz.ppm", "rb");
    if (fp == NULL) {
        printf("Erro ao tentar carregar o arquivo!");
        return 1;
    }

    //unsigned char myfile[3] = {0x80, 0x7F, 0x84};

    // ler dimenssões da imagem para gerar o mapa de pixels
    fseek(fp, 3, SEEK_CUR);

    for (i = 0; i < 4; i++) {
        fread(widthText + i, sizeof(char), 1, fp);
        if (widthText[i] == ' ') {
            break;
        }
    }
    if (i == 4) {
        printf("Erro: Imagem com dimensões não suportadas!");
        return 1;
    }
    widthText[i] = '\0';

    for (i = 0; i < 4; i++) {
        fread(heightText + i, sizeof(char), 1, fp);
        if (heightText[i] == '\n') {
            break;
        }
    }
    if (i == 4) {
        printf("Erro: Imagem com dimensões não suportadas!");
        return 1;
    }
    heightText[i] = '\0';

    widthImg2 = atoi(widthText);
    heightImg2 = atoi(heightText);

    unsigned char mapPixels2[heightImg2][widthImg2][3];

    // preencher mapa de pixels
    fseek(fp, 4, SEEK_CUR);
    for (posX = 0, posY = -1; posX != widthImg2 + 1 && posY != 78; posX++) {
        posX %= widthImg2;
        if (posX == 0) {
            posY += 1;
        }

        fread(&mapPixels2[posY][posX][0], sizeof(char), 1, fp);
        fread(&mapPixels2[posY][posX][1], sizeof(char), 1, fp);
        fread(&mapPixels2[posY][posX][2], sizeof(char), 1, fp);
    }
    //
    fclose(fp);

    /////////////////////////////////////////
    // sobrepor uma imagem sobre a outra
    fp = fopen("/home/macabeus/ApenasMeu/Dropbox/CEFET/SistemasEmbarcados/marcadagua/new.ppm", "wb");
    fputs("P6\n81 78\n255\n", fp); // todo: usar as dimensões de acordo com a maior imagem!

    for (posX = 0, posY = -1; posX != widthImg1 + 1 && posY != 78; posX++) {
        posX %= widthImg1;
        if (posX == 0) {
            posY += 1;
        }

        if (widthImg2 > posX && heightImg2 > posY) {
            fputc((char) (mapPixels[posY][posX][0] * 0.5 + mapPixels2[posY][posX][0] * 0.5), fp);
            fputc((char) (mapPixels[posY][posX][1] * 0.5 + mapPixels2[posY][posX][1] * 0.5), fp);
            fputc((char) (mapPixels[posY][posX][2] * 0.5 + mapPixels2[posY][posX][2] * 0.5), fp);
        } else {
            fputc(mapPixels[posY][posX][0], fp);
            fputc(mapPixels[posY][posX][1], fp);
            fputc(mapPixels[posY][posX][2], fp);
        }
    }

    fclose(fp);

    return 0;
}