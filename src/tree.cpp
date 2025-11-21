#include "tree.h"

#define READ(name)  scanf("%[^\n]", name); \
                    getchar();

Node_t* NodeCtor(const char* message) {
    my_assert(message, NULLPTR, NULL);

    Node_t* new_node = (Node_t*)calloc(1, sizeof(Node_t));
    my_assert(new_node, CALLOC_ERR, NULL);

    new_node->message = message;
    new_node->parent = new_node->left = new_node->right = NULL;
}

Tree_t* TreeCtor() {
    Tree_t* tree = (Tree_t*)calloc(1, sizeof(Tree_t));
    my_assert(tree, NULLPTR, NULL);

    tree->root = NodeCtor("Unknown what");
    tree->nodes_cnt = 1;

    tree->html_file_name = "dump.html";
    tree->dot_file_name = "tree.dot";
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

    int real_size = GetSize(tree->root);

    if (real_size != tree->nodes_cnt)
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
    sprintf(dot_str, "dot tree.dot -T png -o result%d.png", dump_counter);
    system(dot_str);

    char img_path[100] = {};
    sprintf(img_path, "result%d.png", dump_counter++);

    fprintf(dump_file, "<h3>");
    TextDump(tree->root, dump_file);
    fprintf(dump_file, "</h3>");

    fprintf(dump_file, "<img src=\"%s\">\n", img_path);

    fprintf(dump_file, "<hr>\n");

    fclose(dump_file);
}

void TreeImgDump(Tree_t* tree) {
    FILE* dot_file = fopen(tree->dot_file_name, "w");
    fprintf(dot_file, "digraph G {\n");

    RecDump(tree->root, dot_file);

    fprintf(dot_file, "}");
    fclose(dot_file);
}

void RecDump(Node_t* root, FILE* dot_file) {
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
        RecDump(root->left, dot_file);
        fprintf(dot_file, "\tNode%X->Node%X;\n", (int)root, (int)root->left);
    }
    if (root->right) {
        RecDump(root->right, dot_file);
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

    printf(GREEN_COLOR "Hi, that's Akinator\n" RESET_COLOR);

    const char* answer = (const char*)calloc(MSG_SIZE, sizeof(char));

    do {
        printf("Change game mode (f - finding, c - compare, d - definition): ");
        READ(answer);

        while (answer[0] != 'f' && answer[0] != 'c' && answer[0] != 'd') {
            printf(RED_COLOR "Wrong input. Please, enter game mode again: " RESET_COLOR);
        }

        if (answer[0] == 'f')
            FindingWord(tree);
        else if (answer[0] == 'c')
            Comparison(tree);
        else
            Definition(tree);

        printf("Do you want continue?\n");
        READ(answer);

    } while (CheckAnswer(answer));

    return NOTHING;
}

CodeError_t FindingWord(Tree_t* tree) {
    printf(YELLOW_COLOR "Finding mode...\n" RESET_COLOR);

    const char* answer = (const char*)calloc(MSG_SIZE, sizeof(char));
    Node_t* cur = tree->root;

    for (; cur && !(cur->left == NULL && cur->right == NULL);) {
        printf("%s?\n", cur->message);

        READ(answer);

        if (CheckAnswer(answer))
            cur = cur->left;
        else
            cur = cur->right;
    }

    if (!cur) return NULLPTR;

    printf("Finded: " YELLOW_COLOR "%s" RESET_COLOR ", is it true?\n", cur->message);
    READ(answer);

    if (strcmp(answer, "no") == 0)
        NewVertex(tree, cur);
}

bool CheckAnswer(const char* answer) {
    return (!strcmp(answer, "yes") || !strcmp(answer, "yes") || !strcmp(answer, "y"));
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
    (root->left)->parent = root;

    root->right = NodeCtor(root->message);
    (root->right)->parent = root;

    root->message = root_new_msg;

    return NOTHING;
}

CodeError_t Comparison(Tree_t* tree) {
    treeverify(tree);

    printf(YELLOW_COLOR "Comparing mode...\n" RESET_COLOR);
    printf("Please, enter names of nodes, which you want compare: \n");

    const char* first_name = (const char*)calloc(MSG_SIZE, sizeof(char));
    const char* second_name = (const char*)calloc(MSG_SIZE, sizeof(char));

    READ(first_name);
    READ(second_name);

    Node_t* node = tree->root;

    if (!CheckSubtree(node, first_name)) {
        printf(RED_COLOR "There is no %s in base (use finding mode, to add it)\n" RESET_COLOR, first_name);
        return VALUE_ERR;
    }

    if (!CheckSubtree(node, second_name)) {
        printf(RED_COLOR "There is no %s in base (use finding mode, to add it)\n" RESET_COLOR, second_name);
        return VALUE_ERR;
    }

    printf(YELLOW_COLOR "Common part:" RESET_COLOR);
    int common_part = 0;
    int rev = 0;
    while (CheckSubtree(node, first_name) && CheckSubtree(node, second_name)) {
        if (node->left == NULL && node->right == NULL)
            break;

        if (CheckSubtree(node->left, first_name) && CheckSubtree(node->left, second_name)) {
            printf(YELLOW_COLOR " --- ");
            printf(GREEN_COLOR "%s" RESET_COLOR, node->message);
            ++common_part;

            node = node->left;
        }
        else if (CheckSubtree(node->right, first_name) && CheckSubtree(node->right, second_name)) {
            printf(YELLOW_COLOR " --- ");
            printf(RED_COLOR "%s" RESET_COLOR, node->message);
            ++common_part;
            
            node = node->right;
        }
        else {
            if (CheckSubtree(node->right, first_name)) 
                rev = 1;

            break;
        }
    }

    if (node == tree->root)
        printf(RED_COLOR " NOTHING" RESET_COLOR);
    
    if (common_part)
        printf(YELLOW_COLOR " --- " RESET_COLOR);
    printf("\n");

    if (node->left == NULL && node->right == NULL) {
        printf(GREEN_COLOR "The elements are the same\n" RESET_COLOR);
        return NOTHING;
    }

    if (rev) {
        const char* add_name = first_name;
        first_name = second_name;
        second_name = add_name;
    }

    printf(YELLOW_COLOR "First different part: " GREEN_COLOR "%s %s" YELLOW_COLOR", and " RED_COLOR "%s not\n" RESET_COLOR, first_name, node->message, second_name);

    Node_t* left = node->left;
    Node_t* right = node->right;
    printf(GREEN_COLOR "%s:" RESET_COLOR, first_name);

    while (left->left || left->right) {
        if (CheckSubtree(left->left, first_name)) {
            printf(YELLOW_COLOR " --- ");
            printf(GREEN_COLOR "%s", left->message);
            left = left->left;
        }
        else {
            printf(YELLOW_COLOR " --- ");
            printf(RED_COLOR "%s", left->message);
            left = left->right;
        }
    }

    if (left != node->left)
        printf(YELLOW_COLOR " --- ");
    else
        printf(YELLOW_COLOR " no added information");
    printf("\n" RESET_COLOR);

    printf(GREEN_COLOR "%s:" RESET_COLOR, second_name);

    while (right->left || right->right) {
        if (CheckSubtree(right->left, second_name)) {
            printf(YELLOW_COLOR " --- ");
            printf(GREEN_COLOR "%s", right->message);
            right = right->left;
        }
        else {
            printf(YELLOW_COLOR " --- ");
            printf(RED_COLOR "%s", right->message);
            right = right->right;
        }
    }

    if (right != node->right)
        printf(YELLOW_COLOR " --- ");
    else
        printf(YELLOW_COLOR " no added information");
    printf("\n" RESET_COLOR);

    return NOTHING;
}

CodeError_t Definition(Tree_t* tree) {
    treeverify(tree);

    printf(YELLOW_COLOR "Definition mode...\n" RESET_COLOR);
    printf("Enter the name of node, the definition of which you want to get:\n");

    const char* name = (const char*)calloc(MSG_SIZE, sizeof(char));

    READ(name);

    Node_t* node = tree->root;

    if (!CheckSubtree(node, name)) {
        printf(RED_COLOR "There is no %s in base (use finding mode, to add it)\n" RESET_COLOR, name);
        return VALUE_ERR;
    }

    printf(GREEN_COLOR "%s" YELLOW_COLOR ": ", name);
    while (node->left || node->right) {
        if (CheckSubtree(node->left, name)) {
            printf(YELLOW_COLOR " --- ");
            printf(GREEN_COLOR "%s", node->message);
            node = node->left;
        }
        else {
            printf(YELLOW_COLOR " --- ");
            printf(RED_COLOR "%s", node->message);
            node = node->right;
        }
    }

    printf(YELLOW_COLOR " --- \n" RESET_COLOR);

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

        node->message = (const char*)calloc(MSG_SIZE, sizeof(char));
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

int CalcHash(int p) {
    p = (p ^ (p >> 16)) * 0xC4CEB9FE;
    p = (p ^ (p >> 13)) * 0xFF51AFD7;
    p = p ^ (p >> 16);

    return (p >> 8) | 0x00808080;
}
