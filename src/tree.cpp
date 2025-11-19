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

void RecDump(Node_t* root, FILE* dot_file) {       // where if ...???
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

    const char* answer = (const char*)calloc(MSG_SIZE, sizeof(char));

    Node_t* cur = tree->root;

    do {
        cur = tree->root;

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

        printf("Do you want continue?\n");
        READ(answer);
        
    } while (CheckAnswer(answer));

    return NOTHING;
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

CodeError_t MessageComparison(Tree_t* tree) {
    printf("Please, enter nodes which you want compare: \n");

    const char* first_name = (const char*)calloc(MSG_SIZE, sizeof(char));
    const char* second_name = (const char*)calloc(MSG_SIZE, sizeof(char));
    
    READ(first_name);
    READ(second_name);

    Node_t* first_node = FindVertex(tree->root, first_name);
    Node_t* second_node = FindVertex(tree->root, second_name);
    
    if (!first_node)
        printf(RED_COLOR "First node isn't finded\n" RESET_COLOR);
    
    if (!second_node)
        printf(RED_COLOR "Second node isn't finded\n" RESET_COLOR);

    my_assert(first_node && second_node, IND_ERR, IND_ERR);

    Node_t* common_parent = FindParent(tree, first_node, second_node);

    if (!common_parent) {
        printf(RED_COLOR "Common parent isn't finded" RESET_COLOR);
        return IND_ERR;
    }
    
    Node_t* node = common_parent;

    if (node != tree->root) {
        printf(YELLOW_COLOR "Common: ");

        if (GetParent(node)->left == node)
            printf(GREEN_COLOR);
        else
            printf(RED_COLOR);
        node = GetParent(node);
        printf("%s" YELLOW_COLOR, node->message);

        while (node != tree->root) {
            printf(" --- ");

            if (GetParent(node)->left == node)
                printf(GREEN_COLOR);
            else
                printf(RED_COLOR);
            node = GetParent(node);
            printf("%s" YELLOW_COLOR, node->message);
        }
    }
    else {
        printf(RED_COLOR "Nothing common");
    }

    printf(YELLOW_COLOR "\nAdditionl %s features: ", first_name);

    while (GetParent(first_node) != common_parent) {
        if (GetParent(first_node)->left == first_node)
            printf(GREEN_COLOR);
        else
            printf(RED_COLOR);

        first_node = GetParent(first_node);
        printf("%s" YELLOW_COLOR, first_node->message);
        
        if (GetParent(first_node) != common_parent)
            printf(" --- ");
    }

    printf(YELLOW_COLOR "\nAdditionl %s features: ", second_name);

    while (GetParent(second_node) != common_parent) {

        if (GetParent(second_node)->left == second_node)
            printf(GREEN_COLOR);
        else
            printf(RED_COLOR);

        second_node = GetParent(second_node);
        printf("%s" YELLOW_COLOR, second_node->message);

        if (GetParent(second_node) != common_parent)
            printf(" --- ");
    }

    printf("\nFirst different feature: ");

    if (common_parent->left == first_node)
        printf(GREEN_COLOR"%s is %s " YELLOW_COLOR "and " RED_COLOR "%s not", first_name, common_parent->message, second_name);
    else
        printf(GREEN_COLOR"%s is %s " YELLOW_COLOR "and " RED_COLOR "%s not", second_name, common_parent->message, first_name);

    printf("\n" RESET_COLOR);
}

Node_t* FindParent(Tree_t* tree, Node_t* first_node, Node_t* second_node) {
    my_assert(tree, NULLPTR, NULL);
    my_assert(first_node, NULLPTR, NULL);
    my_assert(second_node, NULLPTR, NULL);

    int first_deep = GetDeep(tree, first_node);
    int second_deep = GetDeep(tree, second_node);
    
    if (first_deep < second_deep) {
        Node_t* add_node = first_node;
        first_node = second_node;
        second_node = add_node;
    }

    int deep_delta = abs(first_deep - second_deep);

    while (deep_delta--)
        first_node = first_node->parent;

    while (first_node != second_node) {
        first_node = GetParent(first_node);
        second_node = GetParent(second_node);
    }

    return first_node;
}

Node_t* GetParent(Node_t* root) {
    my_assert(root, NULLPTR, NULL);

    if (!root)
        return NULL;

    return root->parent;
}

int GetDeep(Tree_t* tree, Node_t* node) {
    my_assert(tree, NULLPTR, -1);
    my_assert(node, NULLPTR, -1);

    int deep = 0;

    while (node != tree->root) {
        node = node->parent;
        ++deep;
    }

    return deep;
}

Node_t* FindVertex(Node_t* root, const char* find_msg) {
    my_assert(root, NULLPTR, NULL);
    my_assert(find_msg, NULLPTR, NULL);

    if (!root || !find_msg) 
        return NULL;

    if (!strcmp(root->message, find_msg))
        return root;

    if (root->left) {
        Node_t* vertex = FindVertex(root->left, find_msg);

        if (vertex)
            return vertex;
    }

    if (root->right) {
        Node_t* vertex = FindVertex(root->right, find_msg);

        if (vertex)
            return vertex;
    }

    return NULL;
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

        node->message = (const char*)calloc(MSG_SIZE, sizeof(char));        // if size > msg_size
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
