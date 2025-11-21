#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include "errors.h"
#include "vars.h"

#define htmldump(tree, error_code, add_info) HtmlDump(tree, VarInfo{#tree, __FILE__, __FUNCTION__, __LINE__, error_code, add_info})

#define treeverify(tree) htmldump(tree, TreeVerify(tree), "Dump after verification")

const int MSG_SIZE      =   100;
const int MAX_NODES_CNT = 10000;

struct Node_t {
    const char* message = "";

    Node_t* parent = NULL;

    Node_t* left = NULL;
    Node_t* right = NULL;
};

struct Tree_t {
    Node_t* root;

    size_t nodes_cnt;

    const char* dot_file_name;
    const char* html_file_name;
    char* buf;
};

Node_t* NodeCtor(const char* message);
Tree_t* TreeCtor();
CodeError_t TreeDtor(Node_t* root);
CodeError_t TreeVerify(Tree_t* tree);
int GetSize(Node_t* root);
int CalcHash(int p);

CodeError_t HtmlDump(Tree_t* tree, VarInfo varinfo);
void TreeImgDump(Tree_t* tree);
void RecDump(Node_t* root, FILE* dot_file);
void TextDump(Node_t* root, FILE* text_file);

CodeError_t Akinator(Tree_t* root);

CodeError_t FindingWord(Tree_t* tree);
bool CheckAnswer(const char* answer);
CodeError_t NewVertex(Tree_t* tree, Node_t* cur);
CodeError_t AddVertex(Node_t* root, const char* root_new_msg, const char* left_new_msg);

CodeError_t Comparison(Tree_t* tree);
CodeError_t Definition(Tree_t* tree);
Node_t* CheckSubtree(Node_t* node, const char* name);

CodeError_t ReadBase(Tree_t* tree, const char* file_name);
int get_file_size(const char* file_name);
Node_t* ParseBase(char** cur_pos);

#endif