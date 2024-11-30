#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Symbol structure to store information
struct Symbol {
    char name[50];
    char type[10];
    char scope[10];  // Scope can be 'local' or 'global'
    int address;
    int value;  // Value of the symbol (0 if reserved, or the assigned value)
    int defined;
    int declared;
    char section[10];  // Sections like ".data", ".bss", ".text"
    int size;
};

// Global variables to track addresses in each section
int data_address = 0;
int bss_address = 0;
int text_address = 0;
char current_section[10] = "";  // Global variable to track the current section

// Function to parse each line of assembly code
void parse_line(char* line, struct Symbol* symbol, FILE *address_file) {
    // Handle section declaration (e.g., .data, .bss, .text)
    if (strstr(line, ".data")) {
        strcpy(current_section, "data");
        return; // Skip processing if it's a section declaration
    }
    if (strstr(line, ".bss")) {
        strcpy(current_section, "bss");
        return; // Skip processing if it's a section declaration
    }
    if (strstr(line, ".text")) {
        strcpy(current_section, "text");
        return; // Skip processing if it's a section declaration
    }

    // Ignore instructions like 'mov', 'add', etc.
    if (strstr(line, "mov") || strstr(line, "add") || strstr(line, "sub") || strstr(line, "mul") || strstr(line,"cmp") || strstr(line,"xchg") || strstr(line,"push")) {
        return; // These are instructions, not symbols, so skip them
    }

    // Handle labels in the .text section (e.g., main:, ab:)
    if (current_section[0] == 't') {
        // Find the position of the colon
        char *colon_pos = strchr(line, ':');
        if (colon_pos != NULL) {
            // Null-terminate the string at the colon to get the label name
            *colon_pos = '\0';  // Cut off the colon and get the label name

            // Process the label
            strcpy(symbol->name, line);  // The label name is the part before the colon
            strcpy(symbol->type, "label");
            strcpy(symbol->section, "text");
            symbol->address = text_address++;  // Increment address for each label in the text section
            symbol->size = 0;  // Labels don't have a size
            strcpy(symbol->scope, "local");  // Scope of labels is local
            symbol->value = 0;  // Labels have no value
            symbol->defined = 1; // Labels are considered defined
            symbol->declared = 1; // Labels are declared

            // Write the label address and line to the address file in hexadecimal form
            fprintf(address_file, "%08X  %s\n", symbol->address,  line);

            return; // Return after processing the label to skip further handling
        }
        return;  // If it's not a label, just return and do not process the line
    }

    // Assign the section to the symbol if it is in data or bss
    if (current_section[0] != 't') {
        strcpy(symbol->section, current_section);
    }

    // Assign global scope for all non-label symbols
    strcpy(symbol->scope, "global");

    // Handle dd (define double word - 4 bytes)
    if (strstr(line, "dd")) {
        sscanf(line, "%s dd %d", symbol->name, &symbol->value);
        strcpy(symbol->type, "int");
        symbol->size = 4; // 4 bytes for dd (double word)
        if (strcmp(current_section, "data") == 0) {
            symbol->address = data_address;
            data_address += 4; // Increment address by 4 bytes for dd
        } else if (strcmp(current_section, "bss") == 0) {
            symbol->address = bss_address;
            bss_address += 4; // Increment address by 4 bytes for resd
        }
        symbol->defined = 1; // section has defined
        symbol->declared = 1; // section has declared

        // Write the symbol address and line to the address file in hexadecimal format
        fprintf(address_file, "%08X  %s\n", symbol->address,  line);
    }
    // Handle dw (define word - 2 bytes)
    else if (strstr(line, "dw")) {
        sscanf(line, "%s dw %d", symbol->name, &symbol->value);
        strcpy(symbol->type, "short");
        symbol->size = 2; // 2 bytes for dw (word)
        if (strcmp(current_section, "data") == 0) {
            symbol->address = data_address;
            data_address += 2; // Increment address by 2 bytes for dw
        } else if (strcmp(current_section, "bss") == 0) {
            symbol->address = bss_address;
            bss_address += 2; // Increment address by 2 bytes for resw
        }
        symbol->defined = 1;  // section has defined
        symbol->declared = 1; // section has declared

        // Write the symbol address and line to the address file in hexadecimal format
        fprintf(address_file, "%08X  %s\n", symbol->address,  line);
    }
    // Handle db (define byte - 1 byte)
    else if (strstr(line, "db")) {
        sscanf(line, "%s db %d", symbol->name, &symbol->value);
        strcpy(symbol->type, "char");
        symbol->size = 1; // 1 byte for db (byte)
        if (strcmp(current_section, "data") == 0) {
            symbol->address = data_address;
            data_address += 1; // Increment address by 1 byte for db
        } else if (strcmp(current_section, "bss") == 0) {
            symbol->address = bss_address;
            bss_address += 1; // Increment address by 1 byte for resb
        }
        symbol->defined = 1;  // section has defined
        symbol->declared = 1; // section has declared

        // Write the symbol address and line to the address file in hexadecimal format
        fprintf(address_file, "%08X  %s\n", symbol->address,  line);
    }
    // Handle dq (define quad word - 8 bytes)
    else if (strstr(line, "dq")) {
        sscanf(line, "%s dq %d", symbol->name, &symbol->value);
        strcpy(symbol->type, "long");
        symbol->size = 8; // 8 bytes for dq (quad word)
        if (strcmp(current_section, "data") == 0) {
            symbol->address = data_address;
            data_address += 8; // Increment address by 8 bytes for dq
        } else if (strcmp(current_section, "bss") == 0) {
            symbol->address = bss_address;
            bss_address += 8; // Increment address by 8 bytes for resq
        }
        symbol->defined = 1;  // section has defined
        symbol->declared = 1; // section has declared

        // Write the symbol address and line to the address file in hexadecimal format
        fprintf(address_file, "%08X  %s\n", symbol->address,  line);
    }
    // Handle resb (reserve byte)
    else if (strstr(line, "resb")) {
        int num_bytes;
        sscanf(line, "%s resb %d", symbol->name, &num_bytes);
        strcpy(symbol->type, "char");
        symbol->address = bss_address; // Reserve at the current bss address
        symbol->size = num_bytes; // Size is simply the number of bytes reserved
        bss_address += num_bytes; // Increment address by the number of bytes reserved
        symbol->value = 0;  // No value for reserved memory
        symbol->defined = 0; // Reserved space is not defined
        symbol->declared = 1; // section has declared

        // Write the symbol address and line to the address file in hexadecimal format
        fprintf(address_file, "%08X  %s\n", symbol->address,  line);
    }
    // Handle resd (reserve double word - 4 bytes)
    else if (strstr(line, "resd")) {
        int num_words;
        sscanf(line, "%s resd %d", symbol->name, &num_words);
        strcpy(symbol->type, "int");
        symbol->address = bss_address; // Reserve at the current bss address
        symbol->size = num_words * 4; // 4 bytes for each double word (resd)
        bss_address += num_words * 4; // Increment address by the number of bytes reserved
        symbol->value = 0;  // No value for reserved memory
        symbol->defined = 0; // Reserved space is not defined
        symbol->declared = 1; // section has declared

        // Write the symbol address and line to the address file in hexadecimal format
        fprintf(address_file, "%08X  %s\n", symbol->address,  line);
    }
}

// Function to print the symbol table
void print_symbol_table(struct Symbol *symbols, int num_symbols) {
    printf("| Name           | Address  | Type      | Scope      | Section   | Size | Defined   | Declared |\n");
    printf("|----------------|----------|-----------|------------|-----------|------|-----------|----------|\n");

    for (int i = 0; i < num_symbols; i++) {
        printf("| %-15s | %-8X | %-10s | %-10s | %-10s | %-4d | %-10s | %-10s |\n",
               symbols[i].name,
               symbols[i].address,  // Address as hexadecimal
               symbols[i].type,
               symbols[i].scope,
               symbols[i].section,
               symbols[i].size,
               symbols[i].defined ? "defined" : "not defined",   // Check if defined and print string
               symbols[i].declared ? "declared" : "not declared"  // Check if declared and print string
        );
    }
}


// Main function to process input assembly code and generate symbol table
int main() {
    FILE *source_file = fopen("input.asm", "r");
    if (!source_file) {
        printf("Error opening source file!\n");
        return 1;
    }

    FILE *address_file = fopen("symbol_addresses.txt", "w");
    if (!address_file) {
        printf("Error opening address file!\n");
        fclose(source_file);
        return 1;
    }

    char line[256];
    struct Symbol symbol;
    while (fgets(line, sizeof(line), source_file)) {
        parse_line(line, &symbol, address_file); // Parse and process each line
    }

    fclose(source_file);
    fclose(address_file);

    printf("Symbol addresses have been written to symbol_addresses.txt\n");

    return 0;
}

