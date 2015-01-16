
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "pqueue.h"

#define CHAR_COUNT CHAR_BIT * CHAR_BIT

#define OPTION_COMPRESS_S "-c"
#define OPTION_COMPRESS_L "--compress"
#define OPTION_DECOMPRESS_S "-d"
#define OPTION_DECOMPRESS_L "--decompress"

typedef struct {
    char bits;
    int data;
} compressed_char;

int analyze_file(FILE* file, unsigned char *freqs) {
    
    unsigned int freqs_accurate[CHAR_COUNT], max_accurate_freq = 0;
    int c = INT_MIN + 7, i;
    
    /*přesná frekvenční analýza*/
    while ((c = fgetc(file)) != EOF) {
        freqs_accurate[(char)c]++;
    }
    
    /*detekce prázdného souboru*/
    if(c == INT_MIN + 7) {
        return 0;
    }
    
    /*zjištění nejvyššího výskytu*/
    for (i = 0; i < CHAR_COUNT; i++) {
        if(freqs_accurate[i] > max_accurate_freq) {
            max_accurate_freq = freqs_accurate[i];
        }
    }
    
    /*zmenšení datového typu se zachováním poměrných hodnot výskytu*/
    for (i = 0; i < CHAR_COUNT; i++) {
        if(freqs_accurate[i] > 0) {
            freqs[i] = ceil(max_accurate_freq / (double)freqs_accurate[i] * UCHAR_MAX);
        }
    }
    
    return 1;
}

/*načtení četností z komprimovaného souboru*/
int load_freqs(FILE* file, unsigned char *freqs) {
    
    char *chars;
    unsigned int alphabet_size;
    int rtn = 1;
    
    /*načtení počtu znaků v abecedě*/
    if(!fread(&alphabet_size, sizeof(unsigned char), CHAR_COUNT, file)) {
        return 0;
    }
    
    alphabet_size++;
    
    chars = malloc(alphabet_size * sizeof(char));
    
    /*načtení znaků abecedy*/
    if(fread(chars, sizeof(char), alphabet_size, file)) {
        
        int i;
        
        /*načtení četností*/
        for (i = 0; i < alphabet_size; i++) {
            if(!fread(freqs + chars[i], sizeof(unsigned char), 1, file)) { 
                rtn = 0;
                break;
            }
        }
    }
    else {
        rtn = 0;
    }

    free(chars);
    
    return rtn;
}

binary_node* build_huffman_tree(unsigned char *freqs) {
    
    int i;
    pqueue *pq = pqueue_create(CHAR_COUNT / 4);
    
    /*naplnění fronty*/
    for (i = 0; i < CHAR_COUNT; i++) {
        if(freqs[i] != 0) {
            binary_node_create(freqs[i], i, NULL, NULL);
        }
    }
}

/*návod k použití*/
int usage(char *command) {
    
    printf(
        "Usage: %s options input_file output_file\n"
        "Compress or decompress file using huffman coding.\n\n"
        "  options:\n"
        "      %s %s\n"
        "          Input file will be compressed\n"
        "      %s %s\n"
        "          Input file will be decompressed\n\n"
        "  input_file Input filename\n\n"
        "  output_file Output filename",
        command, OPTION_COMPRESS_S, OPTION_COMPRESS_L, OPTION_DECOMPRESS_S, OPTION_DECOMPRESS_L
    );
    
    return EXIT_FAILURE;
}

int main(int argc, char **argv) {
    
    int compress;
    FILE* input, output;
    unsigned char freqs[CHAR_COUNT] = {0};
    
    /*kontrola vstupních parametrů*/
    if(argc < 4 || !(compress = !(strcmp(argv[1], OPTION_COMPRESS_S) && strcmp(argv[1], OPTION_COMPRESS_L))) && strcmp(argv[1], OPTION_DECOMPRESS_S) && strcmp(argv[1], OPTION_DECOMPRESS_S)) {
        return usage(argv[0]);
    }
    
    input = fopen(argv[2], "rb");
    
    if(input == NULL) {
        printf("The input file \"%s\" cannot be opened.");
        return EXIT_FAILURE;
    }
    
    if(compress) {
        compressed_char *huff_codes[CHAR_COUNT] = {NULL};
        
        if(!analyze_file(input, freqs)) {
            puts("Empty file cannot be compressed.");
        }
        
    }
    else {
        if(!load_freqs(input, freqs)) {
            printf("Supplied file is corrupt.");
        }
    }
    
    binary_node *tree = build_huffman_tree();
    
    
    
    printf("%d", compress);
    
    return EXIT_SUCCESS;
}