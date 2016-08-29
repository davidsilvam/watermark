/*********************************************************************
*Blend Mode(Alpha compositing) - Alunos: Bruno Macabeus e David Silva*
**********************************************************************/
#include <stdio.h>
#include <stdlib.h>

int main() {
    int posX, posY, i;
    float ad = 0.5, as = 1;
    char widthTextBuffer[4], heightTextBuffer[4];
    int widthImgDest, heightImgDest, widthImgSour, heightImgSour;
    FILE *fp;

    // Início carregamento da imagem destination
    fp = fopen("destination.ppm", "rb");
    if (fp == NULL) {
        printf("Erro ao tentar carregar o arquivo!");
        return 1;
    }
    // Fim carregamento da imagem destination

    // Reposiciona o cursor em 3 bytes a partir do início
    fseek(fp, 3, SEEK_CUR);

    // Início da leitura das dimensões da imagem destination
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
    // Fim da leitura das dimensões da imagem destination

    // Inicio preenchimento mapa de pixes da imagem destination
    unsigned char mapPixelsDest[heightImgDest][widthImgDest][3];
    fseek(fp, 4, SEEK_CUR);

    for (posX = 0, posY = -1; posX != widthImgDest + 1 && posY != heightImgDest; posX++) {
        posX %= widthImgDest;
        if (posX == 0) {
            posY += 1;
        }

        fread(&mapPixelsDest[posY][posX][0], sizeof(char), 1, fp);
        fread(&mapPixelsDest[posY][posX][1], sizeof(char), 1, fp);
        fread(&mapPixelsDest[posY][posX][2], sizeof(char), 1, fp);
    }

    // Fim preechimento do mapa de pixels da imagem destination
    fclose(fp);
     //////////////////////////////////////////////////////////////////////////
    // Início carregamento da imagem source
    fp = fopen("source.ppm", "rb");
    if (fp == NULL) {
        printf("Erro ao tentar carregar o arquivo!");
        return 1;
    }
    //fim carregamento da imagem source


    fseek(fp, 3, SEEK_CUR);
    // Início da leitura das dimensões da imagem source
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

    // Fim da leitura das dimensões da imagem source

    // Início do preenchimento do mapa de píxels da imagem source
    unsigned char mapPixelsSour[heightImgSour][widthImgSour][3];


    fseek(fp, 4, SEEK_CUR);
    for (posX = 0, posY = -1; posX != widthImgSour + 1 && posY != heightImgSour; posX++) {
        posX %= widthImgSour;
        if (posX == 0) {
            posY += 1;
        }

        fread(&mapPixelsSour[posY][posX][0], sizeof(char), 1, fp);
        fread(&mapPixelsSour[posY][posX][1], sizeof(char), 1, fp);
        fread(&mapPixelsSour[posY][posX][2], sizeof(char), 1, fp);
    }

    // Fim do preenchimento do mapa de pixels da imagem source
    fclose(fp);

    if(widthImgSour < widthImgDest || heightImgSour < heightImgDest){
        printf("Atencao! Source deve ter dimensoes maiores que destination, caso contrario destination sera cortada!\n");
        printf("Dimensoes Source: %dx%d\nDimensoes Destination: %dx%d\n",widthImgSour,heightImgSour,widthImgDest,heightImgDest);
    }

    //////////////////////////////////////////////////////////////////////////
    // begin blend alpha compositing
    fp = fopen("output.ppm", "wb");
    //Convencionar source como maior imagem
    fprintf(fp,"%s\n%d %d\n255\n","P6",widthImgSour,heightImgSour);

    for (posX = 0, posY = -1; posX != widthImgSour + 1 && posY != heightImgSour; posX++) {
        posX %= widthImgSour;
        if (posX == 0) {
            posY += 1;
        }

        if (widthImgDest > posX && heightImgDest > posY) {
            fputc((char) (mapPixelsDest[posY][posX][0] * ad + mapPixelsSour[posY][posX][0] * as * (1 - ad)), fp);
            fputc((char) (mapPixelsDest[posY][posX][1] * ad + mapPixelsSour[posY][posX][1] * as * (1 - ad)), fp);
            fputc((char) (mapPixelsDest[posY][posX][2] * ad + mapPixelsSour[posY][posX][2] * as * (1 - ad)), fp);
        } else {
            fputc(mapPixelsSour[posY][posX][0], fp);
            fputc(mapPixelsSour[posY][posX][1], fp);
            fputc(mapPixelsSour[posY][posX][2], fp);
        }
    }
    fclose(fp);

    return 0;
}
