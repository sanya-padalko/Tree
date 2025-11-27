#include "tree.h"

#define READ(name)  scanf("%[^\n]", name); \
                    getchar();

#define VOICE(str)  printf("%s", str); \
                    txSpeak("<speak version='1.0' xml:lang='EN'>%s</speak>", str);

#define ull unsigned long long

Node_t* NodeCtor(const char* message) {
    my_assert(message, NULLPTR, NULL);

    Node_t* new_node = (Node_t*)calloc(1, sizeof(Node_t));
    my_assert(new_node, CALLOC_ERR, NULL);

    new_node->message = strdup(message);
    new_node->parent = new_node->left = new_node->right = NULL;

    return new_node;
}

Tree_t* TreeCtor() {
    Tree_t* tree = (Tree_t*)calloc(1, sizeof(Tree_t));
    my_assert(tree, NULLPTR, NULL);

    tree->root = NodeCtor("Unknown what");
    tree->nodes_cnt = 1;

    tree->html_file_name = "dump.html";
    tree->dot_file_name = "tree.dot";

    return tree;
}

CodeError_t TreeDtor(Node_t* root) {
    my_assert(root, NULLPTR, NULLPTR);

    if (root->left)
        TreeDtor(root->left);

    if (root->right)
        TreeDtor(root->right);

    free(root);

    return NOTHING;
}

CodeError_t TreeVerify(Tree_t* tree) {
    if (!tree)
        return NULLPTR;

    if (!tree->root)
        return NULLPTR;

    if (tree->nodes_cnt <= 0)
        return SIZE_ERR;

    int real_size = GetSize(tree->root);

    if ((size_t)real_size != tree->nodes_cnt)
        return CYCLE_ERR;

    return NOTHING;
}

int GetSize(Node_t* root) {
    my_assert(root, NULLPTR, MAX_NODES_CNT);

    int sz = 1;
    if (root->left)
        sz += GetSize(root->left);

    if (sz > MAX_NODES_CNT)
        return sz;

    if (root->right)
        sz += GetSize(root->right);

    return sz;
}

CodeError_t HtmlDump(Tree_t* tree, VarInfo varinfo) {
    my_assert(tree, NULLPTR, NULLPTR);

    static int dump_counter = 0;

    FILE* dump_file = fopen(tree->html_file_name, "a");
    fprintf(dump_file, "<h2 style = \"color: blue\"> %s </h2>\n", varinfo.add_info);
    const char* color = "green";
    if (varinfo.error_code != NOTHING) {
        fprintf(dump_file, "<h2 style = \"color: red\"> ERROR: %s</h2>\n", ErrorType(varinfo.error_code));
        color = "red";
    }

    fprintf(dump_file, "<h2 style=\"color: %s\"> TreeDump called from %s, function: %s, line %d, list name: %s</h2>\n", color, varinfo.file_name, varinfo.func_name, varinfo.line, varinfo.name);

    if (tree->root == NULL) {
        fprintf(dump_file, "<h3 style=\"color: red\">Root of tree pointer equal NULL</h3>\n");
        return NULLPTR;
    }

    TreeImgDump(tree);

    char dot_str[100] = {};
    sprintf(dot_str, "\"C:\\Program Files\\Graphviz\\bin\\dot.exe\" tree.dot -T svg -o result%d.svg", dump_counter);
    system(dot_str);

    char img_path[100] = {};
    sprintf(img_path, "result%d.svg", dump_counter++);

    fprintf(dump_file, "<h3>");
    TextDump(tree->root, dump_file);
    fprintf(dump_file, "</h3>");

    fprintf(dump_file, "<img src=\"%s\">\n", img_path);

    fprintf(dump_file, "<hr>\n");

    fclose(dump_file);

    return NOTHING;
}

void TreeImgDump(Tree_t* tree) {
    FILE* dot_file = fopen(tree->dot_file_name, "w");
    fprintf(dot_file, "digraph G {\n");

    RecDump(tree->root, dot_file);

    fprintf(dot_file, "}");
    fclose(dot_file);
}

void RecDump(Node_t* root, FILE* dot_file) {
    fprintf(dot_file, "\tNode%llX[shape = Mrecord, style = \"filled\", fillcolor = \"#%06x\", label = <\n\t<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"5\">\n\t\t<TR>\n\t\t\t<TD> ptr: 0x%llX </TD>\n\t\t</TR>\n\n\t\t<TR>\n\t\t\t<TD> %s </TD>\n\t\t</TR>\n\n\t\t<TR>\n\t\t\t<TD BGCOLOR = \"#%06x\"> left: ", (ull)root, (unsigned int)CalcHash((long long)root), (ull)root, root->message, (unsigned int)CalcHash((long long)root->left));
    if (root->left)
        fprintf(dot_file, "0x%llX", (ull)root->left);
    else
        fprintf(dot_file, "NULL");

    fprintf(dot_file, " </TD>\n\t\t</TR>\n\n\t\t<TR>\n\t\t\t<TD BGCOLOR = \"#%06x\"> right: ", (unsigned int)CalcHash((long long)root->right));
    if (root->right)
        fprintf(dot_file, "0x%llX", (ull)root->right);
    else
        fprintf(dot_file, "NULL");

    fprintf(dot_file, "</TD>\n\t\t</TR>\n\t</TABLE>>];\n\n");

    if (root->left) {
        RecDump(root->left, dot_file);
        fprintf(dot_file, "\tNode%llX->Node%llX;\n", (ull)root, (ull)root->left);
    }
    if (root->right) {
        RecDump(root->right, dot_file);
        fprintf(dot_file, "\tNode%llX->Node%llX;\n", (ull)root, (ull)root->right);
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

    VOICE("Hi, that's Akinator\n");

    char* answer = (char*)calloc(MSG_SIZE, sizeof(char));

    do {
        VOICE("Choose game mode (f - finding, c - compare, d - definition): ");
        READ(answer);

        while (answer[0] != 'f' && answer[0] != 'c' && answer[0] != 'd') {
            VOICE("Wrong input. Please, enter game mode again: ");
            READ(answer);
        }

        if (answer[0] == 'f')
            FindingWord(tree);
        else if (answer[0] == 'c')
            Comparison(tree);
        else
            Definition(tree);

        VOICE("Do you want continue?\n");
        READ(answer);

    } while (CheckAnswer(answer));

    VOICE("Thanks for playing :)\n");

    return NOTHING;
}

CodeError_t FindingWord(Tree_t* tree) {
    VOICE("Finding mode...\n");

    char* answer = (char*)calloc(MSG_SIZE, sizeof(char));
    char* message = (char*)calloc(MSG_SIZE, sizeof(char));
    Node_t* cur = tree->root;

    for (; cur && !(cur->left == NULL && cur->right == NULL);) {
        sprintf(message, "%s?\n", cur->message);
        VOICE(message);
        READ(answer);

        if (CheckAnswer(answer))
            cur = cur->left;
        else
            cur = cur->right;
    }

    if (!cur) return NULLPTR;

    sprintf(message, "Finded: %s, is it true?\n", cur->message);
    VOICE(message);
    READ(answer);

    if (strcmp(answer, "no") == 0)
        NewVertex(tree, cur);

    return NOTHING;
}

bool CheckAnswer(const char* answer) {
    return (!strcmp(answer, "yes") || !strcmp(answer, "yes") || !strcmp(answer, "y"));
}

CodeError_t NewVertex(Tree_t* tree, Node_t* cur) {
    VOICE("Who it was?\n");
    char* new_msg = (char*)calloc(MSG_SIZE, sizeof(char));
    READ(new_msg);

    char* message = (char*)calloc(MSG_SIZE, sizeof(char));
    sprintf(message, "How is %s different from %s?: %s ", new_msg, cur->message, new_msg);
    VOICE(message);

    char* diff_param = (char*)calloc(MSG_SIZE, sizeof(char));
    READ(diff_param);

    AddVertex(cur, diff_param, new_msg);
    ++tree->nodes_cnt;

    htmldump(tree, NOTHING, "added new word");

    return NOTHING;
}

CodeError_t AddVertex(Node_t* root, char* root_new_msg, char* left_new_msg) {
    my_assert(root, NULLPTR, NULLPTR);
    my_assert(root_new_msg, NULLPTR, NULLPTR);
    my_assert(left_new_msg, NULLPTR, NULLPTR);

    root->left = NodeCtor(left_new_msg);
    (root->left)->parent = root;

    root->right = NodeCtor(root->message);
    (root->right)->parent = root;

    root->message = root_new_msg;

    return NOTHING;
}

CodeError_t Comparison(Tree_t* tree) {
    treeverify(tree);

    VOICE("Comparing mode...\n");
    VOICE("Please, enter names of nodes, which you want compare: \n");

    char* first_name = (char*)calloc(MSG_SIZE, sizeof(char));
    char* second_name = (char*)calloc(MSG_SIZE, sizeof(char));

    char* message = (char*)calloc(MSG_SIZE, sizeof(char));

    READ(first_name);
    READ(second_name);

    Node_t* node = tree->root;

    if (!CheckSubtree(node, first_name)) {
        sprintf(message, "There is no %s in base (use finding mode, to add it)\n", first_name);
        VOICE(message);
        return VALUE_ERR;
    }

    if (!CheckSubtree(node, second_name)) {
        sprintf(message, "There is no %s in base (use finding mode, to add it)\n", second_name);
        VOICE(message);
        return VALUE_ERR;
    }

    sprintf(message, "Common part:");
    VOICE(message);
    int common_part = 0;
    int rev = 0;
    while (CheckSubtree(node, first_name) && CheckSubtree(node, second_name)) {
        if (node->left == NULL && node->right == NULL)
            break;

        if (CheckSubtree(node->left, first_name) && CheckSubtree(node->left, second_name)) {
            printf(" --- ");
            sprintf(message, "%s", node->message);
            VOICE(message);
            ++common_part;

            node = node->left;
        }
        else if (CheckSubtree(node->right, first_name) && CheckSubtree(node->right, second_name)) {
            printf(" --- ");
            sprintf(message, "(not) %s", node->message);
            VOICE(message);
            ++common_part;
            
            node = node->right;
        }
        else {
            if (CheckSubtree(node->right, first_name)) 
                rev = 1;

            break;
        }
    }

    if (node == tree->root) {
        sprintf(message, " NOTHING");
        VOICE(message);
    }

    if (common_part)
        printf(" --- ");
    printf("\n");

    if (node->left == NULL && node->right == NULL) {
        sprintf(message, "The elements are the same\n");
        VOICE(message);
        return NOTHING;
    }

    if (rev) {
        char* add_name = first_name;
        first_name = second_name;
        second_name = add_name;
    }

    sprintf(message, "First different part: %s - %s, and %s not\n", first_name, node->message, second_name);
    VOICE(message);

    Node_t* left = node->left;
    Node_t* right = node->right;
    sprintf(message, "%s:", first_name);
    VOICE(message);

    while (left->left || left->right) {
        if (CheckSubtree(left->left, first_name)) {
            printf(" --- ");
            sprintf(message, "%s", left->message);
            VOICE(message);
            left = left->left;
        }
        else {
            printf(" --- ");
            sprintf(message, "(not) %s", left->message);
            VOICE(message);
            left = left->right;
        }
    }

    if (left != node->left) {
        printf(" --- ");
    }
    else {
        sprintf(message, " no added information");
        VOICE(message);
    }
    printf("\n");

    sprintf(message, "%s:", second_name);
    VOICE(message);

    while (right->left || right->right) {
        if (CheckSubtree(right->left, second_name)) {
            printf(" --- ");
            sprintf(message, "%s", right->message);
            VOICE(message);
            right = right->left;
        }
        else {
            printf(" --- ");
            sprintf(message, "(not) %s", right->message);
            VOICE(message);
            right = right->right;
        }
    }

    if (right != node->right) {
        printf(" --- ");
    }
    else {
        sprintf(message, " no added information");
        VOICE(message);
    }
    printf("\n");

    return NOTHING;
}

CodeError_t Definition(Tree_t* tree) {
    treeverify(tree);

    VOICE("Definition mode...\n");
    VOICE("Enter the name of node, the definition of which you want to get:\n");

    char* name = (char*)calloc(MSG_SIZE, sizeof(char));
    char* message = (char*)calloc(MSG_SIZE, sizeof(char));

    READ(name);

    Node_t* node = tree->root;

    if (!CheckSubtree(node, name)) {
        sprintf(message, "There is no %s in base (use finding mode, to add it)\n", name);
        VOICE(message);
        return VALUE_ERR;
    }

    sprintf(message, "%s: ", name);
    VOICE(message);
    while (node->left || node->right) {
        if (CheckSubtree(node->left, name)) {
            printf(" --- ");
            sprintf(message, "%s", node->message);
            VOICE(message);
            node = node->left;
        }
        else {
            printf(" --- ");
            sprintf(message, "(not) %s", node->message);
            VOICE(message);
            node = node->right;
        }
    }

    printf(" --- \n");

    return NOTHING;
}

Node_t* CheckSubtree(Node_t* node, const char* name) {
    if (!node || !name)
        return NULL;

    if (!strcmp(node->message, name) && node->left == NULL && node->right == NULL)
        return node;

    Node_t* node_name = CheckSubtree(node->left, name);
    if (node_name)
        return node_name;

    return CheckSubtree(node->right, name);
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

        node->message = (char*)calloc(MSG_SIZE, sizeof(char));
        sscanf(*cur_pos, " \"%[^\"]\" %n", node->message, &read_bytes);
        *cur_pos += read_bytes;

        node->left = ParseBase(cur_pos);
        if (node->left)
            node->left->parent = node;

        node->right = ParseBase(cur_pos);
        if (node->right)
            node->right->parent = node;

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

int CalcHash(long long p) {
    int c = (int)p;

    c = (c ^ (c >> 16)) * 0xC4CEB9FE;
    c = (c ^ (c >> 13)) * 0xFF51AFD7;
    c = c ^ (c >> 16);

    return (c >> 8) | 0x00808080;
}
