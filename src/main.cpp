#include "tree.h"
#include "folders.h"

int main(int argc, char* argv[]) {
    MakeDir();
    OpenHtml();

    const char* base_file = "../base.txt";

    Tree_t* tree = TreeCtor();

    ReadBase(tree, base_file);

    char* cur_pos = tree->buf;
    tree->root = ParseBase(&cur_pos);

    htmldump(tree, NOTHING, "base after parse");

    htmldump(tree, NOTHING, "from main #1");

    Akinator(tree);

    htmldump(tree, NOTHING, "from main #2");

    FILE* base = fopen(base_file, "w");
    TextDump(tree->root, base);
    fclose(base);

    TreeDtor(tree->root);
    free(tree);

    CloseHtml();
}