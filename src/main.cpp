#include "tree.h"
#include "folders.h"

int main(int argc, char* argv[]) {
    MakeDir();

    const char* base_file = "../base.txt";

    Tree_t* tree = TreeCtor();

    ReadBase(tree, base_file);

    char* cur_pos = tree->buf;
    tree->root = ParseBase(&cur_pos);
    tree->root->parent = tree->root;

    htmldump(tree, NOTHING, "base after parse");

    Akinator(tree);

    htmldump(tree, NOTHING, "before writing to base");

    FILE* base = fopen(base_file, "w");
    TextDump(tree->root, base);
    fclose(base);

    TreeDtor(tree->root);
    free(tree);
}
