#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>

#define HEADER_SIZE (sizeof(BSPHeader))
#define LUMP_SIZE (sizeof(BSPLump))

#define HEADER_LUMPS 64

static char * const BSP_MAGIC = "VBSP";

static char * const HTBP_HEADER = 
    "############################\n"
    "#                          #\n"
    "# Hazard Team BSP Profiler #\n"
    "#          By DKY          #\n"
    "#                          #\n"
    "############################"
    ;
    
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


static void get_filename(char filename[PATH_MAX], char filepath[PATH_MAX]) {
    // Find the last slash position.
    int lastSlashPos = -1;
    for (size_t i=0; i<PATH_MAX && filepath[i]; i++) {
        char c = filepath[i];
        if (c == '\\') {
            lastSlashPos = i;
        }
    }
    
    // Copy everything after the last slash.
    strncpy(filename, filepath + lastSlashPos + 1, PATH_MAX);
    
}


static off_t get_file_size(const char* filename) {
    struct stat statStruct;
    
    if (stat(filename, &statStruct) == 0) {
        return statStruct.st_size;
    }
    else {
        perror("stat() error");
        return -1;
    }
    
}


int main(int argc, char* argv[]) {
    int rc = 0;
    
    // The name of the BSP file.
    char* filepath;
    
    // Get the argument(s).
    if (argc > 1) {
        filepath = argv[1];
    }
    else {
        fprintf(stderr, "ERROR: You must provide at least 1 BSP file.\n");
        return 1;
    }
    
    // The BSP header that we will be loading.
    BSPHeader header;
    
    // Open the file for binary reading.
    printf("Opening %s...\n", filepath);
    FILE* f = fopen(filepath, "rb");
    if (f == NULL) {
        perror("fopen() error");
        return 1;
    }
    
    // Read the data into the header struct.
    printf("Reading %s...\n", filepath);
    if (fread(&header, 1, HEADER_SIZE, f) != HEADER_SIZE) {
        perror("fread() error");
        rc = 1;
        goto cleanup;   // This is a legitimate reason to use a goto...
    }
    
    char idStr[5];
    build_str_from_int32(idStr, header.ident);
    
    // Check the 4-byte magic number to make sure this is a VBSP file.
    if (strcmp(idStr, BSP_MAGIC) != 0) {
        fprintf(
                stderr,
                "ERROR: %s is not a Source Engine BSP file!\n",
                filepath
            );
        rc = 1;
        goto cleanup;
    }
    
    char filename[PATH_MAX];
    get_filename(filename, filepath);
    
    printf("\n%s\n\n", HTBP_HEADER);
    printf("== %s ==\n", filename);
    printf("\nID: %s\n", idStr);
    printf("BSP Version: %d\n", header.version);
    printf("Map Revision: %d\n", header.mapRevision);
    
    off_t totalFileSize = get_file_size(filepath);
    if (totalFileSize == -1) {
        fprintf(stderr, "ERROR: Failed to get file size.\n");
        rc = 1;
        goto cleanup;
    }
    
    printf("Total file size: %d\n\n", totalFileSize);
    
    // Calculate size percentages for each lump.
    for (size_t i=0; i<HEADER_LUMPS; i++) {
        BSPLump lump = header.lumps[i];
        
        off_t size = (off_t) lump.filelen;
        double percent = ((double) size / (double) totalFileSize) * 100.0;
        
        printf("Lump %d:\t\t%d\t\t(%f%%)\n", i, size, percent);
        
    }
    
    printf("Press [ENTER] to exit.\n");
    
    char throwaway;
    scanf("%c", &throwaway);
    
cleanup:
    fclose(f);
    
    return rc;
}

