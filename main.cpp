#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

auto file_name = "s";

inline int int_comparer(const void* a, const void* b) {
    return *(u_int8_t*)a - *(u_int8_t*)b;
}

int main() {

    unsigned CHUNK_SIZE = 100;

    auto f = fopen(file_name, "r+");

    fseek(f, 0, SEEK_END);

    auto file_size = ftell(f);

    auto BUFFER_LENGTH = CHUNK_SIZE * 2;

    auto buffer = new u_int8_t[BUFFER_LENGTH];
    auto buf_2 = buffer + CHUNK_SIZE;

    auto last_chunk_size = (unsigned int)file_size % CHUNK_SIZE;

    auto chunk_count = file_size / CHUNK_SIZE;

    if (last_chunk_size) {
        chunk_count++;
    } else {
        last_chunk_size = CHUNK_SIZE;
    }

    for (auto i = 0; i < chunk_count - 1; ++i) {

        fseek(f, i * CHUNK_SIZE, SEEK_SET);

        fread(buffer, 1, CHUNK_SIZE, f);

        for (auto j = i + 1; j < chunk_count; ++j) {

            auto current_chunk_size = CHUNK_SIZE;

            if (j == chunk_count - 1) {
                current_chunk_size = last_chunk_size;
            }

            fseek(f, j * current_chunk_size, SEEK_SET);

            fread(buf_2, 1, current_chunk_size, f);

            qsort(buffer, CHUNK_SIZE + current_chunk_size, 1, int_comparer);

            fseek(f, j * CHUNK_SIZE, SEEK_SET);

            fwrite(buf_2, 1, current_chunk_size, f);

        }

        fseek(f, i * CHUNK_SIZE, SEEK_SET);

        fwrite(buffer, 1, CHUNK_SIZE, f);
    }

    fclose(f);

    return 0;

}
