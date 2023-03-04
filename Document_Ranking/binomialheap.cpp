#include"binomialheap.hpp"
BinomialTree::~BinomialTree(){
    delete doc;
    frequency = 0;
    k = 0;
    children = NULL;
    sibling = NULL;
}
BinomialTree * createBinomialTree(int freq,document * doc){
    BinomialTree * temp = new BinomialTree;
    temp->children = NULL;
    temp->sibling = NULL;
    temp->k = 0;
    temp->frequency = freq;
    temp->doc = doc;
    return temp;
}
void attachTrees(BinomialTree * first,BinomialTree * second){//O(1)
    //We attach the nodes having same degree. It is easy
    //second become child of current or first tree
    second->sibling = first->children;
    first->children = second;
    first->k++;
}
BinomialTree * mergeSortRoot(BinomialTree * first, BinomialTree * second){
    BinomialTree * result = createBinomialTree(0,NULL); //dummy node
    BinomialTree * tail = result;
    //creating root list of all binomial tree in non-decreasing order of k
    while(first != NULL || second != NULL){
        if(first == NULL){
            tail->sibling = second;
            break;
        }
        else if(second == NULL){
            tail->sibling = first;
            break;
        }
        else if(second->k < first->k){
            tail->sibling = second;
            second = second->sibling;
        }
        else{
            tail->sibling = first;
            first = first->sibling;
        }
        //We insert the node having smaller degree
        //tail helps us to insert node woth O(1) time
        tail = tail->sibling;
    }
    return result->sibling; //returning nodes with original document avoiding first dummy node we created
    //result was temporary nodes;
}
BinomialTree * reverseChildrenNode(BinomialTree * curr, BinomialTree * prev){
    if(curr != NULL){
        BinomialTree * temp = curr->sibling;
        curr->sibling = prev;
        return reverseChildrenNode(temp,curr);
    }
    return prev;
    //Reversing the children is important because each binomial tree has decreasing order
    //children. If we delete max element we need reverse list of its children so that we
    //union the two heap
}
void deleteDepthWise(BinomialTree * node){
    if(node == NULL) return;
    while(node->children != NULL){
        deleteDepthWise(node->children);
    }
    while(node->sibling != NULL){
        deleteDepthWise(node->sibling);
    }
    delete node;
}
BinomialHeap::BinomialHeap(BinomialTree * bt,int sz):head(bt){
    size = sz;
}
BinomialHeap::~BinomialHeap(){
    deleteDepthWise(head);
    size = 0;
}

void BinomialHeap::unionTree(BinomialHeap * h){
    if(h->getHead() == NULL){return;} //If heap2 is empty then return heap1
    this->size += h->getSize(); //In our implementation we want to keep the size of the heap
    BinomialTree * uniqueKs = mergeSortRoot(this->head,h->getHead());
    //Resulting tree node after merging forests. It is non decreasing order
    //but there could be multiple tree nodes having the same degree so
    //in following code will we find these nodes and we will merge them
    h->setHead();
    h->setSize();
    //we delete heap2
    BinomialTree * prev = NULL; //x
    BinomialTree * curr = uniqueKs; //n-x
    BinomialTree * next = curr->sibling; //s-n-x
    //Thses nodes helps us to detect consecutive nodes having the same degree k

    while(next != NULL){
        if((next->k != curr->k) || (next->sibling != NULL && curr->k == next->k && next->k==next->sibling->k)){
            prev = curr;
            curr = next;
            //If x != n-x then move all x, n-x and s-n-x by one step
            //If x == n-x == s-n-x then move by one step to get binomial combination of higher degree at later
        }
        //The node having bigger value should be parent. The other shoulod be child.(MAX HEAP)
        else{
            //max-binomial heap
            if(curr->frequency > next->frequency){
                curr->sibling = next->sibling;
                attachTrees(curr,next);
                //Current becomes the root of the next
            }
            else{
                if(prev == NULL){uniqueKs = next;}
                else{prev->sibling = next;}
                //when current becomes child, the sibling of prev should be next,
                //otherwise prev would point dangerous node(o_o)
                attachTrees(next,curr);
                curr = next;
            }
        }
        next = curr->sibling;
    }
    this->head = uniqueKs;
    //The binomial tree nodes are in non-decreasing order and unique
}
void BinomialHeap::enqueue(int freq,document * doc){ //O(logn)
    BinomialTree * bt = createBinomialTree(freq,doc);
    BinomialHeap temp(bt,1);
    unionTree(&temp);
}
int BinomialHeap::getMax(){//O(logn)
    //To get maximum frequency in heap
    int max = INT_MIN;
    BinomialTree * maxFinder = this->head;
    while(maxFinder != NULL){
        max = max < maxFinder->frequency ? maxFinder->frequency : max;
        maxFinder = maxFinder->sibling;
    }
    return max;
}
BinomialTree * BinomialHeap::dequeue(){//O(logn)
    int maxValue = getMax();

    BinomialTree * maxNode = this->head;
    BinomialTree * tempHead = createBinomialTree(0,NULL);

    tempHead->sibling = maxNode;
    BinomialTree * prev = tempHead;

    while(maxNode->frequency != maxValue){
        maxNode = maxNode->sibling;
        prev = prev->sibling;
    }
    //Both max node and previous of max node is found
    prev->sibling = maxNode->sibling;
    //max node has been removed from from root list. But we should put its children as
    //reverse listed heap during union
    this->head = tempHead->sibling; //getting back head
    BinomialTree * childrenOfMin = reverseChildrenNode(maxNode->children,NULL);
    //children are reversed, now they are in non-decreasing order

    BinomialHeap bhTempMerge(childrenOfMin);
    //A temporary heap to merge two heap;
    unionTree(&bhTempMerge);
    //children have been put back in the heap
    this->size--;
    return maxNode;
}
