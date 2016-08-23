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

    unsigned char mapPixels2[widthImg2 * heightImg2 * 3];

    // preencher mapa de pixels
    fseek(fp, 4, SEEK_CUR);
    for (i = 0; i < widthImg2 * heightImg2 * 3; i++) {
        fread(mapPixels2 + i, sizeof(char), 1, fp);
    }

    //
    fclose(fp);

    /////////////////////////////////////////
    // sobrepor uma imagem sobre a outra
    fp = fopen("/home/macabeus/ApenasMeu/Dropbox/CEFET/SistemasEmbarcados/marcadagua/new.ppm", "wb");
    fputs("P6\n81 78\n255\n", fp); // todo: usar as dimensões de acordo com a maior imagem!

    //o código de iterar abaixo não está dando certo, talvez usar a solução da gabi que é passando pelos índices
    //        int numero           //numero que vc quer saber o indice

    //        int coluna             //coluna que ele está
    //        int linha                //linha que ele está

    //        int colunaCount  //descobre com modulo

    //        linha = (numero/colunaCount)-1
    //        coluna = matriz[0][coluna] - (linha * countColuna)

    for (posX = 0, posY = -1; posX != widthImg1 + 1 && posY != 78; posX++) {
        posX %= widthImg1;
        if (posX == 0) {
            posY += 1;
        }

        if (mapPixels[posY][posX][0] == 0xFF && mapPixels[posY][posX][1] == 0xFF && mapPixels[posY][posX][2] == 0xFF) {
            fputc(0x44, fp);
            fputc(0x44, fp);
            fputc(0x44, fp);
        } else {
            fputc(mapPixels[posY][posX][0], fp);
            fputc(mapPixels[posY][posX][1], fp);
            fputc(mapPixels[posY][posX][2], fp);
        }
    }

    fclose(fp);

    return 0;

    for (posX = 0, posY = -1; posX != widthImg1 + 1 && posY != 78; posX++) {
        posX %= widthImg1;
        if (posX == 0) {
            posY += 1;
        }

        /*if (mapPixels[posX * posY] == 0x255 && mapPixels[(posX * posY) + 1] == 0x255 && mapPixels[(posX * posY) + 2] == 0x255) {
            fputc(0x100, fp);
            fputc(0x100, fp);
            fputc(0x100, fp);
        } else {*/
            fputc(mapPixels[posX][posY][0], fp);
            fputc(mapPixels[posX][posY][1], fp);
            fputc(mapPixels[posX][posY][2], fp);
        //}


        /*if (posX <= widthImg2 && posY <= heightImg2) {
            fputc(mapPixels2[posX * posY * 3], fp);
            fputc(mapPixels2[(posX * posY * 3) + 1], fp);
            fputc(mapPixels2[(posX * posY * 3) + 2], fp);
        } else {
            fputc(mapPixels[posX * posY], fp);
            fputc(mapPixels[(posX * posY * 3) + 1], fp);
            fputc(mapPixels[(posX * posY * 3) + 2], fp);
        }*/
    }

    fclose(fp);

    return 0;
}