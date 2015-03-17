#include <iostream>

using namespace std;

int main() {

    unsigned int threshold = 64;

    unsigned chunkSize = 100;

    FILE* f = fopen("text.txt", "r+");

    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);

    long leftIndex = 0;
    long rightIndex = fileSize - chunkSize - 1;

    unsigned char* bufferL = (unsigned char*)malloc(chunkSize);
    unsigned char* bufferR = (unsigned char*)malloc(chunkSize);

    fseek(f, leftIndex, SEEK_SET);
    fread(bufferL, 1, chunkSize, f);

    fseek(f, rightIndex, SEEK_SET);
    fread(bufferR, 1, chunkSize, f);

    int bufIndexLeft = 0;
    int bufIndexRight = chunkSize - 1;

    unsigned skipAfterSwapLeft = 0;
    unsigned skipAfterSwapRight = 0;

    while (1) {

        while (1) {
            if (bufferL[bufIndexLeft] >= threshold && !skipAfterSwapLeft) {
                break;
            }

            skipAfterSwapLeft = 0;

            bufIndexLeft++;

            if (bufIndexLeft == chunkSize) {
                if (rightIndex - leftIndex < chunkSize) {
                    goto end;
                }

                fseek(f, leftIndex, SEEK_SET);
                fwrite(bufferL, 1, chunkSize, f);

                fseek(f, leftIndex + chunkSize, SEEK_SET);
                fread(bufferL, 1, chunkSize, f);

                leftIndex += chunkSize;

                bufIndexLeft = 0;
            }
        }

        while (1) {
            if (bufferR[bufIndexRight] < threshold && !skipAfterSwapRight) {
                break;
            }

            skipAfterSwapRight = 0;

            bufIndexRight--;

            if (bufIndexRight == -1) {
                if (rightIndex - leftIndex < chunkSize) {
                    goto end;
                }

                fseek(f, rightIndex, SEEK_SET);
                fwrite(bufferR, 1, chunkSize, f);

                fseek(f, rightIndex - chunkSize, SEEK_SET);
                fread(bufferR, 1, chunkSize, f);

                rightIndex -= chunkSize;

                bufIndexRight = chunkSize - 1;
            }
        }

        cout << "SWAP " << (int)bufferL[bufIndexLeft] << " | " << (int)bufferR[bufIndexRight] << endl;

        swap(bufferL[bufIndexLeft], bufferR[bufIndexRight]);

        skipAfterSwapLeft = skipAfterSwapRight = 1;
    }

    end:

    fseek(f, leftIndex, SEEK_SET);
    fwrite(bufferL, 1, chunkSize, f);

    fseek(f, rightIndex, SEEK_SET);
    fwrite(bufferR, 1, chunkSize, f);

    fclose(f);

    return 0;
}
