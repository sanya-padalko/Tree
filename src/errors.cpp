#include "errors.h"

void PrintErr(CodeError_t error_type, const char* file_name, const char* func_name, const int line_ind) {
    printerr(RED_COLOR "error in %s: function -> %s, line %d: " RESET_COLOR, file_name, func_name, line_ind);
    switch (error_type) {
        case NOTHING:
            printerr(GREEN_COLOR "Everything is OK\n");
            break;
        case NULLPTR:
            printerr(RED_COLOR "Pointer went equal NULL\n");
            break;
        case SIZE_ERR:
            printerr(RED_COLOR "Unavailable size\n");
            break;
        case CAPACITY_ERR:
            printerr(RED_COLOR "Unavailable capactiy\n");
            break;
        case CAP_SIZE_ERR:
            printerr(RED_COLOR "Size become greater than capacity\n");
            break;
        case FREE_ERR:
            printerr(RED_COLOR "Unavailable tree free index\n");
            break;
        case CYCLE_ERR:
            printerr(RED_COLOR "Tree cycle isn't correct\n");
            break;
        case CANARY_ERR:
            printerr(RED_COLOR "The canaries have been changed\n");
            break;
        case REALLOC_ERR:
            printerr(RED_COLOR "Realloc went wrong\n");
            break;
        case HASH_ERR:
            printerr(RED_COLOR "Hash went wrong\n");
            break;
        case VALUE_ERR:
            printerr(RED_COLOR "Trying to work with wrong value\n");
            break;
        case IND_ERR:
            printerr(RED_COLOR "Tree index out of range\n");
            break;
        case CALLOC_ERR:
            printerr(RED_COLOR "Calloc went wrong\n");
            break;
        default:
            printerr(YELLOW_COLOR "Unknown error\n");
    }

    printerr(RESET_COLOR);
}

const char* ErrorType(CodeError_t error_code) {
    switch (error_code) {
        case NOTHING:
            return "Everything is OK\n";
        case NULLPTR:
            return "Pointer went equal NULL\n";
        case SIZE_ERR:
            return "Unavailable size\n";
        case CAPACITY_ERR:
            return "Unavailable capactiy\n";
        case CAP_SIZE_ERR:
            return "Size become greater than capacity\n";
        case FREE_ERR:
            return "Unavailable tree free index\n";
        case CYCLE_ERR:
            return "Tree cycle isn't correct\n";
        case CANARY_ERR:
            return "The canaries have been changed\n";
        case REALLOC_ERR:
            return "Realloc went wrong\n";
        case HASH_ERR:
            return "Hash went wrong\n";
        case VALUE_ERR:
            return "Trying to work with wrong value\n";
        case IND_ERR:
            return "Tree index out of range\n";
        case CALLOC_ERR:
            return "Calloc went wrong\n";
    }
    return "Unknown error\n";
}