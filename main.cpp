#include <iostream>

using namespace std;

int compareBytes(const void *a, const void *b) {
    return *(unsigned char*)a - *(unsigned char*)b;
}

int main() {
    cout << "Hello, World!" << endl;

    FILE* f = fopen("source", "r+");

    char* buffer = (char*)malloc(200);

    fread(buffer, 1, 100, f);

    fread(buffer + 100, 1, 100, f);

    qsort(buffer, 200, 1, compareBytes);

    FILE* out = fopen("out", "w");

    fwrite(buffer, 1, 100, out);

    fclose(f);
    fclose(out);

    return 0;
}
