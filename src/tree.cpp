#include "tree.h"

#define READ(name)  scanf("%[^\n]", name); \
                    getchar();

FILE* dump_file = NULL;

Node_t* NodeCtor(const char* message) {
    Node_t* new_node = (Node_t*)calloc(1, sizeof(Node_t));
    if (!new_node)
        return NULL;

    new_node->message = message;
    new_node->left = new_node->right = NULL;
}

Tree_t* TreeCtor() {
    Tree_t* tree = (Tree_t*)calloc(1, sizeof(Tree_t));

    tree->root = NodeCtor("Unknown what");
    tree->nodes_cnt = 1;
}

CodeError_t TreeDtor(Node_t* root) {
    my_assert(root, NULLPTR, NULLPTR);

    if (root->left) 
        TreeDtor(root->left);

    if (root->right) 
        TreeDtor(root->right);

    free(root);
}

CodeError_t TreeVerify(Tree_t* tree) {
    if (!tree)
        return NULLPTR;

    if (!tree->root)
        return NULLPTR;

    if (tree->nodes_cnt <= 0)
        return SIZE_ERR;

    int real_size = GetSize(tree->root, tree->nodes_cnt);

    if (real_size > tree->nodes_cnt)
        return CYCLE_ERR;
    
    return NOTHING;
}

int GetSize(Node_t* root, int max_size) {
    my_assert(root, NULLPTR, max_size + 1);

    int sz = 1;
    if (root->left)
        sz += GetSize(root->left, max_size);
    
    if (sz > max_size)
        return sz;

    if (root->right)
        sz += GetSize(root->right, max_size);

    return sz;
}

int dump_counter = 0;

void HtmlDump(Tree_t* tree, VarInfo varinfo) {

    fprintf(dump_file, "<h2 style = \"color: blue\"> %s </h2>\n", varinfo.add_info);
    const char* color = "green";
    if (varinfo.error_code != NOTHING) {
        fprintf(dump_file, "<h2 style = \"color: red\"> ERROR: %s</h2>\n", ErrorType(varinfo.error_code));
        color = "red";
    }

    fprintf(dump_file, "<h2 style=\"color: %s\"> TreeDump called from %s, function: %s, line %d, list name: %s</h2>\n", color, varinfo.file_name, varinfo.func_name, varinfo.line, varinfo.name);

    if (tree->root == NULL) {
        fprintf(dump_file, "<h3 style=\"color: red\">Root of tree pointer equal NULL</h3>\n");
        return;
    }

    FILE* dot_file = fopen("tree.dot", "w");
    fprintf(dot_file, "digraph G {\n");

    TreeDump(tree->root, dot_file);

    fprintf(dot_file, "}");
    fclose(dot_file);
    
    char dot_str[100] = {};
    sprintf(dot_str, "dot tree.dot -T png -o result%d.png", dump_counter);
    system(dot_str);

    char img_path[100] = {};
    sprintf(img_path, "result%d.png", dump_counter++);

    fprintf(dump_file, "<h3>");
    TextDump(tree->root, dump_file);
    fprintf(dump_file, "</h3>");

    fprintf(dump_file, "<img src=\"%s\">\n", img_path);

    fprintf(dump_file, "<hr>\n");
}

void TreeDump(Node_t* root, FILE* dot_file) {
    fprintf(dot_file, "\tNode%X[shape = Mrecord, style = \"filled\", fillcolor = \"#%06x\", label = <\n\t<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"5\">\n\t\t<TR>\n\t\t\t<TD> ptr: 0x%p </TD>\n\t\t</TR>\n\n\t\t<TR>\n\t\t\t<TD> %s </TD>\n\t\t</TR>\n\n\t\t<TR>\n\t\t\t<TD BGCOLOR = \"#%06x\"> left: ", (int)root, CalcHash((int)root), root, root->message, CalcHash((int)root->left));
    if (root->left)
        fprintf(dot_file, "0x%p", root->left);
    else
        fprintf(dot_file, "NULL");

    fprintf(dot_file, " </TD>\n\t\t</TR>\n\n\t\t<TR>\n\t\t\t<TD BGCOLOR = \"#%06x\"> right: ", CalcHash((int)root->right));
    if (root->right)
        fprintf(dot_file, "0x%p", root->right);
    else
        fprintf(dot_file, "NULL");

    fprintf(dot_file, "</TD>\n\t\t</TR>\n\t</TABLE>>];\n\n");
    
    if (root->left) {
        TreeDump(root->left, dot_file);
        fprintf(dot_file, "\tNode%X->Node%X;\n", (int)root, (int)root->left);
    }
    if (root->right) {
        TreeDump(root->right, dot_file);
        fprintf(dot_file, "\tNode%X->Node%X;\n", (int)root, (int)root->right);
    }
}

void TextDump(Node_t* root, FILE* text_file) {
    fprintf(text_file, "( ");
    fprintf(text_file, "\"%s\" ", root->message);

    if (root->left == NULL)
        fprintf(text_file, " nil ");
    else
        TextDump(root->left, text_file);

    if (root->right == NULL)
        fprintf(text_file, " nil ");
    else
        TextDump(root->right, text_file);

    fprintf(text_file, " ) ");
}

CodeError_t Akinator(Tree_t* tree) {
    treeverify(tree);

    const char* answer = (const char*)calloc(MSG_SIZE, sizeof(char));

    Node_t* cur = tree->root;

    do {
        cur = tree->root;

        for (; cur && !(cur->left == NULL && cur->right == NULL);) {
            printf("%s?\n", cur->message);

            READ(answer);

            if (strcmp(answer, "yes") == 0)
                cur = cur->left;
            else
                cur = cur->right;
        }

        if (!cur) return NULLPTR;

        printf("Finded: " YELLOW_COLOR "%s" RESET_COLOR ", is it true?\n", cur->message);
        READ(answer);

        if (strcmp(answer, "no") == 0)
            NewVertex(tree, cur);

        printf("Do you want continue?\n");
        READ(answer);
        
    } while (strcmp(answer, "yes") == 0);

    return NOTHING;
}

CodeError_t NewVertex(Tree_t* tree, Node_t* cur) {
    printf("Who it was?\n");
    const char* new_msg = (const char*)calloc(MSG_SIZE, sizeof(char));
    READ(new_msg);
    printf("How is " YELLOW_COLOR "%s" RESET_COLOR " different from " YELLOW_COLOR "%s" RESET_COLOR "?: " YELLOW_COLOR "%s" RESET_COLOR " ", new_msg, cur->message, new_msg);

    const char* diff_param = (const char*)calloc(MSG_SIZE, sizeof(char));
    READ(diff_param);

    AddVertex(cur, diff_param, new_msg);
    ++tree->nodes_cnt;
    
    htmldump(tree, NOTHING, "added new word");
}

CodeError_t AddVertex(Node_t* root, const char* root_new_msg, const char* left_new_msg) {
    my_assert(root, NULLPTR, NULLPTR);
    my_assert(root_new_msg, NULLPTR, NULLPTR);
    my_assert(left_new_msg, NULLPTR, NULLPTR);

    root->left = NodeCtor(left_new_msg);
    root->right = NodeCtor(root->message);
    root->message = root_new_msg;

    return NOTHING;
}

CodeError_t ReadBase(Tree_t* tree, const char* file_name) {
    my_assert(tree, NULLPTR, NULLPTR);
    my_assert(file_name, NULLPTR, NULLPTR);

    int file_size = get_file_size(file_name);

    tree->buf = (char*)calloc(file_size + 1, sizeof(char));

    FILE* file_base = fopen(file_name, "r");
    fread(tree->buf, sizeof(char), file_size, file_base);

    fclose(file_base);

    return NOTHING;
}

int get_file_size(const char* file_name) {
    my_assert(file_name, NULLPTR, 0);

    struct stat file_stat; 
    int stat_result = stat(file_name, &file_stat);
    my_assert(!stat_result, FILE_ERR, 0);

    return file_stat.st_size;
}

Node_t* ParseBase(char** cur_pos) {
    my_assert(cur_pos, NULLPTR, NULL);
    my_assert(*cur_pos, NULLPTR, NULL);

    if (**cur_pos == '(') {
        ++*cur_pos;

        Node_t* node = NodeCtor("");
        int read_bytes = 0;

        node->message = (const char*)calloc(MSG_SIZE, sizeof(char));
        sscanf(*cur_pos, " \"%[^\"]\" %n", node->message, &read_bytes);
        *cur_pos += read_bytes;

        node->left = ParseBase(cur_pos);
        node->right = ParseBase(cur_pos);

        if (**cur_pos != ')') {
            printerr(RED_COLOR "ERRORS IN TEXT OF BASE\n" RESET_COLOR);
            return node;
        }

        ++*cur_pos;
        sscanf(*cur_pos, " %n", &read_bytes);
        *cur_pos += read_bytes;

        return node;
    }

    if (**cur_pos == 'n') {
        *cur_pos += strlen("nil");

        int read_bytes = 0;
        sscanf(*cur_pos, " %n", &read_bytes);
        *cur_pos += read_bytes;
        return NULL;
    }

    printerr(RED_COLOR "ERRORS IN TEXT OF BASE\n" RESET_COLOR);
    return NULL;
}

int CalcHash(int p) {
    p = (p ^ (p >> 16)) * 0xC4CEB9FE;
    p = (p ^ (p >> 13)) * 0xFF51AFD7;
    p = p ^ (p >> 16);

    return (p >> 8) | 0x00808080;
}

void OpenHtml() {
    dump_file = fopen("dump.html", "w");
}

void CloseHtml() {
    fclose(dump_file);
}