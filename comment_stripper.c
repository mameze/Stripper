/*
 * Comment Stripper - C Implementation
 * Version: 1.0.0
 * Removes comments from C/C++ source files
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define VERSION "1.1.0"
#define MAX_LINE_LENGTH 4096

typedef enum {
    STATE_NORMAL,
    STATE_SINGLE_LINE_COMMENT,
    STATE_MULTI_LINE_COMMENT,
    STATE_STRING,
    STATE_CHAR
} ParserState;

void print_version() {
    printf("Comment Stripper v%s\n", VERSION);
    printf("Removes comments from C/C++ source files\n");
}

void print_usage(const char *prog_name) {
    printf("Usage: %s [OPTIONS] <input_file> [output_file]\n\n", prog_name);
    printf("Options:\n");
    printf("  -h, --help       Show this help message\n");
    printf("  -v, --version    Show version information\n");
    printf("  -i, --in-place   Modify file in place (use with caution!)\n");
    printf("\nExamples:\n");
    printf("  %s input.c output.c\n", prog_name);
    printf("  %s input.c              (creates input.stripped.c)\n", prog_name);
    printf("  %s -i input.c           (modifies input.c directly)\n", prog_name);
}

char *generate_output_filename(const char *input_file) {
    size_t len = strlen(input_file);
    const char *ext = strrchr(input_file, '.');
    size_t base_len = ext ? (size_t)(ext - input_file) : len;
    
    char *output = malloc(base_len + 20); // enough for ".stripped" + extension
    if (!output) return NULL;
    
    strncpy(output, input_file, base_len);
    output[base_len] = '\0';
    strcat(output, ".stripped");
    if (ext) {
        strcat(output, ext);
    }
    
    return output;
}

bool strip_comments(FILE *input, FILE *output) {
    ParserState state = STATE_NORMAL;
    int c, prev_c = '\0';
    bool newline_pending = false;
    
    while ((c = fgetc(input)) != EOF) {
        switch (state) {
            case STATE_NORMAL:
                if (c == '/' && prev_c == '/') {
                    // Start of single-line comment
                    state = STATE_SINGLE_LINE_COMMENT;
                    // Remove the previous '/' we already wrote
                    fseek(output, -1, SEEK_CUR);
                } else if (c == '*' && prev_c == '/') {
                    // Start of multi-line comment
                    state = STATE_MULTI_LINE_COMMENT;
                    fseek(output, -1, SEEK_CUR);
                } else if (c == '"') {
                    // Start of string literal
                    state = STATE_STRING;
                    fputc(c, output);
                } else if (c == '\'') {
                    // Start of char literal
                    state = STATE_CHAR;
                    fputc(c, output);
                } else {
                    fputc(c, output);
                }
                break;
                
            case STATE_SINGLE_LINE_COMMENT:
                if (c == '\n') {
                    state = STATE_NORMAL;
                    fputc(c, output);
                }
                // Ignore all other characters in single-line comment
                break;
                
            case STATE_MULTI_LINE_COMMENT:
                if (c == '/' && prev_c == '*') {
                    state = STATE_NORMAL;
                    // Optionally add space to prevent token merging
                    fputc(' ', output);
                }
                // Ignore all characters in multi-line comment
                break;
                
            case STATE_STRING:
                fputc(c, output);
                if (c == '"' && prev_c != '\\') {
                    state = STATE_NORMAL;
                } else if (c == '\\' && prev_c == '\\') {
                    // Handle escaped backslash
                    prev_c = '\0'; // Reset to not treat next char as escaped
                    continue;
                }
                break;
                
            case STATE_CHAR:
                fputc(c, output);
                if (c == '\'' && prev_c != '\\') {
                    state = STATE_NORMAL;
                } else if (c == '\\' && prev_c == '\\') {
                    prev_c = '\0';
                    continue;
                }
                break;
        }
        
        prev_c = c;
    }
    
    return true;
}

int main(int argc, char *argv[]) {
    const char *input_filename = NULL;
    const char *output_filename = NULL;
    bool in_place = false;
    char *generated_output = NULL;
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            return 0;
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--in-place") == 0) {
            in_place = true;
        } else if (input_filename == NULL) {
            input_filename = argv[i];
        } else if (output_filename == NULL) {
            output_filename = argv[i];
        } else {
            fprintf(stderr, "Error: Too many arguments\n");
            print_usage(argv[0]);
            return 1;
        }
    }
    
    if (input_filename == NULL) {
        fprintf(stderr, "Error: No input file specified\n");
        print_usage(argv[0]);
        return 1;
    }
    
    // Open input file
    FILE *input = fopen(input_filename, "r");
    if (!input) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", input_filename);
        return 1;
    }
    
    // Determine output filename
    if (in_place) {
        output_filename = "temp_stripped_file.tmp";
    } else if (output_filename == NULL) {
        generated_output = generate_output_filename(input_filename);
        if (!generated_output) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            fclose(input);
            return 1;
        }
        output_filename = generated_output;
    }
    
    // Open output file
    FILE *output = fopen(output_filename, "w");
    if (!output) {
        fprintf(stderr, "Error: Cannot open output file '%s'\n", output_filename);
        fclose(input);
        free(generated_output);
        return 1;
    }
    
    printf("Stripping comments from '%s'...\n", input_filename);
    
    // Process the file
    if (!strip_comments(input, output)) {
        fprintf(stderr, "Error: Failed to process file\n");
        fclose(input);
        fclose(output);
        free(generated_output);
        return 1;
    }
    
    fclose(input);
    fclose(output);
    
    // Handle in-place modification
    if (in_place) {
        if (remove(input_filename) != 0) {
            fprintf(stderr, "Error: Cannot remove original file\n");
            free(generated_output);
            return 1;
        }
        if (rename(output_filename, input_filename) != 0) {
            fprintf(stderr, "Error: Cannot rename temporary file\n");
            free(generated_output);
            return 1;
        }
        printf("✓ Modified '%s' in place\n", input_filename);
    } else {
        printf("✓ Output written to '%s'\n", output_filename);
    }
    
    free(generated_output);
    return 0;
}
