
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>

#include "pqueue.h"
#include "binary_tree.h"
#include "safe_malloc.h"

#define CHAR_COUNT (0x1 << CHAR_BIT)

#define OPTION_COMPRESS_S "-c"
#define OPTION_COMPRESS_L "--compress"
#define OPTION_DECOMPRESS_S "-d"
#define OPTION_DECOMPRESS_L "--decompress"

#define EXIT_OOM 2
#define EXIT_INVALID_ARGS 3
#define EXIT_IO_ERROR 4

typedef struct {
    unsigned char bits;
    unsigned int data;
} huff_char;

void analyze_file(FILE* file, unsigned char* alphabet_size, unsigned char* freqs, long int* file_size) {
    
    unsigned int freqs_accurate[CHAR_COUNT] = {0}, max_accurate_freq = 0;
    int c, i;
    
    *alphabet_size = 0;
    
    /*přesná frekvenční analýza*/
    while ((c = fgetc(file)) != EOF) {
        freqs_accurate[(char)c]++;
    }
        
    /*analyzovat jen neprázdné soubory*/
    if((*file_size = ftell(file)) > 0) {
    
        /*zjištění nejvyššího výskytu*/
        for (i = 0; i < CHAR_COUNT; i++) {
            if(freqs_accurate[i] > max_accurate_freq) {
                max_accurate_freq = freqs_accurate[i];
            }
        }

        /*zmenšení datového typu se zachováním poměrných hodnot výskytu*/
        for (i = 0; i < CHAR_COUNT; i++) {
            if(freqs_accurate[i] > 0) {
                freqs[i] = ceil(UCHAR_MAX / (max_accurate_freq / (double)freqs_accurate[i]));
                (*alphabet_size)++;
            }
        }
    }
    
    if(*alphabet_size) {
        (*alphabet_size)--;
    }
}

void write_header(FILE* file, unsigned char alphabet_size, unsigned char* freqs, long int file_size) {

    int i;
    
    fwrite(&alphabet_size, sizeof(unsigned char), 1, file);
    
    for (i = 0; i < CHAR_COUNT; i++) {
        if(freqs[i] > 0) {
            fwrite(&i, sizeof(char), 1, file); /*znak*/
            fwrite(freqs + i, sizeof(unsigned char), 1, file); /*četnost*/
        }
    }
    
    fwrite(&file_size, sizeof(long int), 1, file);
}

void write_compressed(FILE* input, FILE* output, huff_char* huff_codes) {
    
    int c;
    unsigned int buffer = 0x0;
    unsigned char free_bits = sizeof(buffer) * CHAR_BIT, data_bits;
    char shl;
    
    while ((c = fgetc(input)) != EOF) {
        
        data_bits = huff_codes[(char)c].bits;
        
        shl = free_bits - data_bits;
        
        buffer |= huff_codes[(char)c].data << (shl < 0 ? free_bits : shl);
        free_bits -= data_bits > free_bits ? free_bits : data_bits;
        
        /*buffer je plný*/
        if(free_bits == 0) {
            
            fwrite(&buffer, sizeof(buffer), 1, output);
            buffer = 0x0;
            free_bits = sizeof(buffer) * CHAR_BIT;
            
            /*buffer přetekl, doplnit přetečená data*/
            if(shl < 0) {
                free_bits += shl;
                buffer |= huff_codes[(char)c].data << free_bits;
            }
        }
    }
    
    /*zapsání zbývajících dat z bufferu*/
    if(free_bits < sizeof(buffer) * CHAR_BIT) {
        fwrite(&buffer, sizeof(buffer), 1, output);
    }
}

write_decompressed(FILE* input, FILE* output, binary_node* tree, long int file_size) {
    
    unsigned int buffer, mask;
    long int decompressed = 0;
    binary_node* node = tree;
    
    /*načtení bloku zkomprimovaného souboru*/
    while(fread(&buffer, sizeof(buffer), 1, input)) {
        
        /*jedničku na nejvyšší bit masky*/
        mask = 0x1 << (sizeof(mask) * CHAR_BIT - 1);
        
        while(mask) {
            
            /*sestup stromem*/
            node = buffer & mask ? node->right : node->left;
            
            /*zápis znaku z listu*/
            if(binary_node_is_leaf(node)) {
                
                fwrite(&(node->c), sizeof(node->c), 1, output);
                decompressed++;
                
                /*původní soubor byl kompletně zrekonstruován, dekomprese končí*/
                if(decompressed >= file_size) {
                    return;
                }
                
                node = tree;
            }
            
            /*posun maskovacího bitu po jednom doprava*/
            mask >>= 1;
        }
    }
}

/*načtení četností z komprimovaného souboru*/
void read_header(FILE* file, unsigned char* freqs, long int* file_size) {
    
    char *chars;
    unsigned char i, alphabet_size;
    
    /*načtení počtu znaků v abecedě*/
    fread(&alphabet_size, sizeof(unsigned char), CHAR_COUNT, file);
    
    chars = safe_malloc((alphabet_size + 1) * sizeof(char));
    
    /*načtení tabulky četností*/
    for (i = 0; i <= alphabet_size; i++) {
        fread(freqs + i, sizeof(char), 1, file); /*znak*/
        fread(freqs + chars[i], sizeof(unsigned char), 1, file); /*četnost*/
    }
    
    /*načtení velikosti původního souboru*/
    fread(file_size, sizeof(*file_size), 1, file);

    safe_free(chars);
}

void bintree2huffcodes(binary_node* tree, huff_char* huff_codes, huff_char code) {
    
    if(binary_node_is_leaf(tree)) {
        huff_codes[tree->c] = code;
    }
    else {
        code.bits++;
        code.data <<= 1;
        
        bintree2huffcodes(tree->left, huff_codes, code);
        
        code.data |= 0x1;
        
        bintree2huffcodes(tree->right, huff_codes, code);
    }
}

binary_node* build_huffman_tree(unsigned char* freqs) {
    
    int i;
    pqueue *pq = pqueue_create(CHAR_COUNT / 4, &binary_node_comp);
    binary_node *a, *b;
    
    /*naplnění fronty*/
    for (i = 0; i < CHAR_COUNT; i++) {
        if(freqs[i] > 0) {
            a = binary_node_create(freqs[i], i, NULL, NULL);
            pqueue_push(pq, &a);
        }
    }
    
    /* stavba stromu */
    while(pqueue_pop(pq, &a) && pqueue_pop(pq, &b)) {
        
        a = binary_node_create(a->freq_sum + b->freq_sum, 0, a, b);
        
        pqueue_push(pq, &a);
    }
    
    pqueue_free(&pq);
    
    return a;
}

void early_exit(int code) {
    free_remaining();
    exit(code);
}

void oom_exit() {
    puts("Not enough memory to complete the task.");
    early_exit(EXIT_OOM);
}

void inputf_error() {
    puts("Error occured while reading input file.");
    early_exit(EXIT_IO_ERROR);
}

void outputf_error() {
    puts("Error occured while writing into output file.");
    early_exit(EXIT_IO_ERROR);
}

/*návod k použití*/
void usage(char* command) {
    
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
}

int main(int argc, char** argv) {
    
    int compress;
    FILE *input, *output;
    unsigned char freqs[CHAR_COUNT] = {0}, alphabet_size;
    long int file_size;
    
    /*kontrola vstupních parametrů*/
    if(argc < 4 || !(compress = !(strcmp(argv[1], OPTION_COMPRESS_S) && strcmp(argv[1], OPTION_COMPRESS_L))) && strcmp(argv[1], OPTION_DECOMPRESS_S) && strcmp(argv[1], OPTION_DECOMPRESS_S)) {
        usage(argv[0]);
        return EXIT_INVALID_ARGS;
    }
    
    input = fopen(argv[2], "rb");
    output = fopen(argv[3], "wb");
    
    if(input == NULL) {
        printf("The input file \"%s\" cannot be opened.", argv[2]);
        early_exit(EXIT_IO_ERROR);
    }
    
    if(output == NULL) {
        printf("The output file \"%s\" cannot be opened.", argv[3]);
        early_exit(EXIT_IO_ERROR);
    }
    
    if(compress) {
        
        analyze_file(input, &alphabet_size, freqs, &file_size);
        
        if(ferror(input)) {
            inputf_error();
        }
        else if(file_size <= 0) {
            puts("Empty file cannot be compressed.");
            early_exit(EXIT_IO_ERROR);
        }
        
    }
    else {
        
        read_header(input, freqs, &file_size);
        
        if(ferror(input)) {
            inputf_error();
        }
    }
    
    binary_node *tree = build_huffman_tree(freqs);
    

    if(compress) {
        
        huff_char huff_codes[CHAR_COUNT], initial;
        
        initial.bits = 0;
        initial.data = 0x0;
        
        bintree2huffcodes(tree, huff_codes, initial);
        
        rewind(input);
        
        write_header(output, alphabet_size, freqs, file_size);
        write_compressed(input, output, huff_codes);
    }
    else {
        write_decompressed(input, output, tree, file_size);
    }
    
    binary_node_free_tree(&tree);
    
    if(ferror(input)) {
        inputf_error();
    }
    
    if(ferror(output)) {
        outputf_error();
    }
    
    /*zavření souborů*/
    fclose(input);
    fclose(output);
    
    return EXIT_SUCCESS;
}