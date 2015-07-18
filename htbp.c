#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define HEADER_SIZE (sizeof(BSPHeader))
#define LUMP_SIZE (sizeof(BSPLump))

#define HEADER_LUMPS 64

static char * const BSP_MAGIC = "VBSP";


typedef struct {
    int32_t fileofs;
    int32_t filelen;
    int32_t version;
    char fourCC[4];
} BSPLump;


typedef struct {
    int32_t ident;
    int32_t version;
    BSPLump lumps[HEADER_LUMPS];
    int32_t mapRevision;
} BSPHeader;


static inline void build_str_from_int32(char dest[5], uint32_t n) {
    sprintf(
            dest,
            "%c%c%c%c",
            n & 0xFF,
            (n >> 8) & 0xFF,
            (n >> 16) & 0xFF,
            (n >> 24) & 0xFF
        );
}


static void print_lump_bytes(BSPLump* p_lump) {
    char* lumpBytes = (char*) p_lump;
    
    for (size_t i=0; i<LUMP_SIZE; i++) {
        unsigned char byte = lumpBytes[i];
        
        printf("0x%x", byte);
        
        if (i < LUMP_SIZE - 1) {
            printf(" ");
        }
        
    }
    
}


int main(int argc, char* argv[]) {
    int rc = 0;
    
    // The name of the BSP file.
    char* filename;
    
    // Get the argument(s).
    if (argc > 1) {
        filename = argv[1];
    }
    else {
        fprintf(stderr, "ERROR: You must provide at least 1 BSP file.\n");
        return 1;
    }
    
    // The BSP header that we will be loading.
    BSPHeader header;
    
    // Open the file for binary reading.
    printf("Opening %s...\n", filename);
    FILE* f = fopen(filename, "rb");
    if (f == NULL) {
        perror("fopen() failure");
        return 1;
    }
    
    // Read the data into the header struct.
    printf("Reading %s...\n", filename);
    if (fread(&header, 1, HEADER_SIZE, f) != HEADER_SIZE) {
        perror("fread() failure");
        rc = 1;
        goto cleanup;   // This is a legitimate reason to use a goto...
    }
    
    char idStr[5];
    build_str_from_int32(idStr, header.ident);
    
    if (strcmp(idStr, BSP_MAGIC) != 0) {
        fprintf(stderr, "ERROR: %s is not a Source Engine BSP file!\n");
        rc = 1;
        goto cleanup;
    }
    
    printf("\nID: %s\n", idStr);
    printf("Version: %d\n", header.version);
    printf("Revision: %d\n", header.mapRevision);
    
    for (size_t i=0; i<HEADER_LUMPS; i++) {
        BSPLump lump = header.lumps[i];
        
        printf("\nLump %d\n", i);
        printf("Offset: %d\n", lump.fileofs);
        printf("Length: %d\n", lump.filelen);
        printf("Version: %d\n", lump.version);
        printf("FourCC: '%.*s'\n", 4, lump.fourCC);
        
        printf("Lump Bytes: ");
        print_lump_bytes(&lump);
        printf("\n");
        
        char throwaway;
        scanf("%c", &throwaway);
    }
    
cleanup:
    fclose(f);
    
    return rc;
}

