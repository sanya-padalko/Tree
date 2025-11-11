#include "tree.h"
#include "folders.h"

int main(int argc, char* argv[]) {
    MakeDir();
    OpenHtml();

    Tree_t* tree = TreeCtor();

    tree->root->message = "animal";
    Node_t* node1 = NodeCtor("cat");
    Node_t* node2 = NodeCtor("have maces on its hands");
    Node_t* node3 = NodeCtor("Poltorashka");
    Node_t* node4 = NodeCtor("Skubi-du");
    Node_t* node5 = NodeCtor("Mega Knight");
    Node_t* node6 = NodeCtor("Hog rider");

    tree->root->left = node1;
    tree->root->right = node2;
    node1->left = node3;
    node1->right = node4;
    node2->left = node5;
    node2->right = node6;

    AddVertex(node6, "have splash damage", "Mag");
    AddVertex(node4, "have evolution", "King hogs");
    AddVertex(node4->right, "collected from bones", "Sceleton Dragons");

    tree->nodes_cnt = 13;

    htmldump(tree, NOTHING, "from main #1");

    Akinator(tree);

    htmldump(tree, NOTHING, "from main #2");

    TreeDtor(tree->root);
    free(tree);

    CloseHtml();
}