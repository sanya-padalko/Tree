#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "errors.h"
#include "vars.h"

const int MSG_SIZE = 100;

struct Node_t {
    const char* message = "";

    Node_t* left = NULL;
    Node_t* right = NULL;
};

struct Tree_t {
    Node_t* root;

    size_t nodes_cnt;
};

#define htmldump(tree, error_code, add_info) HtmlDump(tree, VarInfo{#tree, __FILE__, __FUNCTION__, __LINE__, error_code, add_info})

#define treeverify(tree) htmldump(tree, TreeVerify(tree), "Dump after verification")

Node_t* NodeCtor(const char* message);
Tree_t* TreeCtor();
CodeError_t TreeDtor(Node_t* root);
CodeError_t TreeVerify(Tree_t* tree);
int GetSize(Node_t* root, int max_size);
int CalcHash(int p);
void HtmlDump(Tree_t* tree, VarInfo varinfo);
void TreeDump(Node_t* root, FILE* dot_file);
void TextDump(Node_t* root);
CodeError_t Akinator(Tree_t* root);
CodeError_t NewVertex(Tree_t* tree, Node_t* cur);
CodeError_t AddVertex(Node_t* root, const char* root_new_msg, const char* left_new_msg);
void OpenHtml();
void CloseHtml();

#endif