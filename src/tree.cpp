#include "tree.h"

#define READ(name)  scanf("%[^\n]", name); \
                    getchar(); \
                    ClearScreen();

char* const voice_msg = (char*)calloc(MSG_SIZE, sizeof(char));

#define VOICE(...)  sprintf(voice_msg, __VA_ARGS__); \
                    txSpeak("\a%s", voice_msg);

#define PRINT_VOICE(...)    printf(__VA_ARGS__); \
                            VOICE(__VA_ARGS__);

#define WINNING() PrintVideo("winning", winning_frames, winning_delay)

#define LOSING() PrintVideo("losing", losing_frames, losing_delay)

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
    txSpeak("<speak version='1.0' xml:lang='EN'>start program</speak>");

    VOICE("Hi, that's Clash Royale Akinator\n");
    PrintVideo("openning", openning_frames, openning_delay);

    char* answer = (char*)calloc(MSG_SIZE, sizeof(char));
    int game_number = 0;

    do {
        ClearScreen();
        VOICE("Choose game mode: ");
        PrintImage("C:/Users/user/Documents/Github/Tree/animation/modes.bmp");
        txSleep(1000);
        if (!game_number)
            VOICE("f - finding, c - comparing, d - definition): ");
        READ(answer);

        while (strcmp(answer, "f") && strcmp(answer, "c") && strcmp(answer, "d")) {
            VOICE("Wrong input. Please, enter game mode again: ");
            PrintImage("C:/Users/user/Documents/Github/Tree/animation/modes.bmp");
            READ(answer);
        }

        switch (answer[0]) {
            case 'f':
                FindingWord(tree);
                break;
            case 'c':
                Comparison(tree);
                break;
            case 'd':
                Definition(tree);
                break;
            default:
                VOICE("This mode don't exist\n");
                txSleep(1000);
        }

        ClearScreen();
        VOICE("Do you want continue?\n");
        READ(answer);
        txSleep(1000);

        ++game_number;

    } while (CheckAnswer(answer));

    VOICE("Thanks for playing :)\n");
    PrintVideo("final", final_frames, final_delay);

    return NOTHING;
}

CodeError_t FindingWord(Tree_t* tree) {
    VOICE("Finding mode...\n");
    PrintVideo("finding", finding_frames, finding_delay);

    VOICE("Please, answer only yes or no");
    txSleep(3500);

    char* answer = (char*)calloc(MSG_SIZE, sizeof(char));
    Node_t* cur = tree->root;

    ClearScreen();

    int quest_cnt = 0;

    for (; cur && !(cur->left == NULL && cur->right == NULL);) {
        if (quest_cnt)  PrintRandomNode(cur);
        PRINT_VOICE("%s?\n", cur->message);
        READ(answer);

        if (CheckAnswer(answer))
            cur = cur->left;
        else
            cur = cur->right;

        ++quest_cnt;
    }

    if (!cur) return NULLPTR;

    txSleep(1000);

    VOICE("Finded: %s, ", cur->message);
    PrintCharacter(cur);
    VOICE("is it true?\n");

    READ(answer);

    if (strcmp(answer, "no") == 0) {
        LOSING();
        NewVertex(tree, cur);
    }
    else {
        WINNING();
    }

    return NOTHING;
}

void PrintRandomNode(Node_t* node) {
    if (!node)
        return;

    if (node->left == NULL && node->right == NULL) {
        ClearScreen();
        char predict[100];
        snprintf(predict, 100, "PREDICT: %s", node->message);
        PrintText(predict);
        txSleep(2000);
        PrintCharacter(node);
        ClearScreen();
        return;
    }

    srand(CalcHash((long long)node));
    if (node->left == NULL) {
        PrintRandomNode(node->right);
    }
    else if (node->right == NULL) {
        PrintRandomNode(node->left);
    }
    else {
        if (rand() % 2)
            PrintRandomNode(node->left);
        else
            PrintRandomNode(node->right);
    }

    return;
}

bool CheckAnswer(const char* answer) {
    return (!strcmp(answer, "yes") || !strcmp(answer, "yes") || !strcmp(answer, "y"));
}

CodeError_t NewVertex(Tree_t* tree, Node_t* cur) {
    ClearScreen();

    VOICE("Who it was?\n");
    char* new_msg = (char*)calloc(MSG_SIZE, sizeof(char));
    READ(new_msg);

    PRINT_VOICE("How is %s different from %s?: %s ", new_msg, cur->message, new_msg);
    char* diff_param = (char*)calloc(MSG_SIZE, sizeof(char));
    READ(diff_param);

    AddVertex(cur, diff_param, new_msg);
    ++tree->nodes_cnt;

    CreateCharacter(new_msg);

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

    PrintImage("C:/Users/user/Documents/Github/Tree/animation/comparison.bmp");
    txSleep(1500);
    ClearScreen();

    VOICE("Please, enter names of nodes, which you want compare: \n");

    char* first_name = (char*)calloc(MSG_SIZE, sizeof(char));
    char* second_name = (char*)calloc(MSG_SIZE, sizeof(char));
    READ(first_name);
    READ(second_name);

    Node_t* node = tree->root;
    if (!CheckSubtree(node, first_name)) {
        VOICE("There is no %s in base (use finding mode, to add it)\n", first_name);
        txSleep(4000);
        return VALUE_ERR;
    }
    if (!CheckSubtree(node, second_name)) {
        VOICE("There is no %s in base (use finding mode, to add it)\n", second_name);
        txSleep(4000);
        return VALUE_ERR;
    }

    VOICE("Comparing %s", first_name);
    PrintIcon(first_name);
    txSleep(1500);

    ClearScreen();

    VOICE(" and %s", second_name);
    PrintIcon(second_name);
    txSleep(1500);

    int com_res = PrintCommonPart(&node, first_name, second_name);
    if (com_res == SAME_ELEM)
        return NOTHING;

    if (com_res == NEED_REV) {
        char* add_name = first_name;
        first_name = second_name;
        second_name = add_name;
    }

    ClearScreen();

    PRINT_VOICE("First different part: %s - %s, and %s not\n", first_name, node->message, second_name);
    txSleep(5500);
    Node_t* left = node->left;
    Node_t* right = node->right;

    ClearScreen();

    PrintDescription(left, first_name);
    PrintDescription(right, second_name);

    ClearScreen();

    return NOTHING;
}

int PrintCommonPart(Node_t** node, char* first_name, char* second_name) {
    VOICE("Common part:");
    txSleep(1500);

    int common_part = 0;
    int rev = NO_NEED_REV;

    while (CheckSubtree((*node), first_name) && CheckSubtree((*node), second_name)) {
        if ((*node)->left == NULL && (*node)->right == NULL)
            break;

        if (CheckSubtree((*node)->left, first_name) && CheckSubtree((*node)->left, second_name)) {
            VOICE("%s", (*node)->message);
            ++common_part;

            (*node) = (*node)->left;
        }
        else if (CheckSubtree((*node)->right, first_name) && CheckSubtree((*node)->right, second_name)) {
            VOICE("(not) %s", (*node)->message);
            ++common_part;
            
            (*node) = (*node)->right;
        }
        else {
            if (CheckSubtree((*node)->right, first_name)) 
                rev = NEED_REV;

            break;
        }
        
        PrintVideo("common", common_frames, common_delay);
        txSleep(1500);
    }

    if (!common_part) {
        VOICE(" NOTHING");
        txSleep(1500);
    }

    if ((*node)->left == NULL && (*node)->right == NULL) {
        VOICE("The elements are the same\n");
        txSleep(2000);
        return SAME_ELEM;
    }

    return rev;
}

CodeError_t Definition(Tree_t* tree) {
    treeverify(tree);

    VOICE("Definition mode...\n");
    PrintVideo("explanation", explanation_frames, explanation_delay);
    txSleep(300);

    ClearScreen();
    VOICE("Enter the name of node, the definition of which you want to get:\n");

    char* name = (char*)calloc(MSG_SIZE, sizeof(char));
    READ(name);
    txSleep(500);

    Node_t* node = tree->root;
    if (!CheckSubtree(node, name)) {
        VOICE("There is no %s in base (use finding mode, to add it)\n", name);
        txSleep(4000);
        return VALUE_ERR;
    }

    PrintDescription(node, name);

    return NOTHING;
}

CodeError_t PrintDescription(Node_t* node, char* name) {
    my_assert(node, NULLPTR, NULLPTR);
    my_assert(name, NULLPTR, NULLPTR);
        
    ClearScreen();
    PRINT_VOICE("%s:\n", name);
    PrintIcon(name);

    int leap_cnt = 0;

    while (node->left || node->right) {
        node = NextVertex(node, name);
        ++leap_cnt;
    }

    if (!leap_cnt) {
        VOICE(" no added information");
        txSleep(1500);
    }

    txSleep(1500);

    return NOTHING;
}

Node_t* NextVertex(Node_t* node, char* name) {
    my_assert(node, NULLPTR, NULL);
    my_assert(name, NULLPTR, NULL);

    if (CheckSubtree(node->left, name)) {
        PRINT_VOICE("%s\n", node->message);
        txSleep(2000);
        return node->left;
    }

    PRINT_VOICE("(not) %s\n", node->message);
    txSleep(2500);
    return node->right;
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
