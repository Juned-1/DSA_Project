#include"document.hpp"
#ifndef BINOMIALHEAP_H_INCLUDED
#define BINOMIALHEAP_H_INCLUDED
struct BinomialTree{
        document * doc; //document
        int frequency; //frequency will be key according to which we will extract maximum using max heap
        int k; //degree of nodes
        BinomialTree * children; //childs
        BinomialTree * sibling;
        ~BinomialTree();
}typedef BinomialTree;
BinomialTree * createBinomialTree(int,document *);
void attachTrees(BinomialTree *,BinomialTree *);
BinomialTree * mergeSortRoot(BinomialTree *, BinomialTree *);
BinomialTree * reverseChildrenNode(BinomialTree *, BinomialTree *);
void deleteDepthWise(BinomialTree *);
class BinomialHeap{
    private:
        BinomialTree * head;
        int size;
    public:
        BinomialHeap(BinomialTree * = NULL,int = 0);
        BinomialTree * getHead()const{return head;} //accessor
        void setHead(BinomialTree * value = NULL){head = value;} //mutator
        void setSize(int value = 0){size = value;} //mutator
        int getSize()const{return size;} //accessor
        void unionTree(BinomialHeap *);
        void enqueue(int,document *);
        int getMax();
        BinomialTree * dequeue();
        //destructor
        ~BinomialHeap();
};

#endif // BINOMIALHEAP_H_INCLUDED
