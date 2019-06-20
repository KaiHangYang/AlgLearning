#include <vector>
#include <iostream>
using namespace std;
class DisjointSet {
public:
    void AddElms(int sum);
    int Find(int elm);
    void Union(int set1, int set2);

    int GetNumElms() const;
    int GetNumSets() const;
private:
    struct Node {
        int index;
        int rank;
        Node *parent;
        Node(int index): parent(NULL), index(index), rank(0) {}
    };

    std::vector<Node *> nodes;
    int numElms = 0;
    int numSets = 0;
};

void DisjointSet::AddElms(int sum) {
    for (int i = numElms; i < numElms + sum; ++i) {
        nodes.push_back(new Node(i));
    }
    numElms += sum;
    numSets += sum;
}

int DisjointSet::Find(int elm) {
    Node *root = nodes[elm];
    
    while (root->parent != NULL) {
        root = root->parent;
    }
    Node *setIter = nodes[elm];
    // point all the elms along the path to the root
    while (setIter != root) {
        Node *parent = setIter->parent;
        setIter->parent = root;
        setIter = parent;
    }
    return root->index;
}
void DisjointSet::Union(int set1, int set2) {
    Node *set1_root = nodes[Find(set1)];
    Node *set2_root = nodes[Find(set2)];
    if (set1_root == set2_root) {
        return;
    }
    if (set1_root->rank < set2_root->rank) {
        set1_root->parent = set2_root;
    }
    else if (set1_root->rank > set2_root->rank) {
        set2_root->parent = set1_root;
    }
    else {
        set2_root->parent = set1_root;
        set1_root->rank += 1;
    }
    --numSets;
}

int DisjointSet::GetNumElms() const {
    return this->numElms;
}
int DisjointSet::GetNumSets() const {
    return this->numSets;
}

int main() {
    DisjointSet s;
    s.AddElms(10);
    cout << s.GetNumElms() << " " << s.GetNumSets() << endl;
    s.Union(1 ,2);
    cout << s.GetNumElms() << " " << s.GetNumSets() << endl;
    s.Union(3, 4);
    cout << s.GetNumElms() << " " << s.GetNumSets() << endl;

    cout << s.Find(1) << " " << s.Find(2) << endl;
    cout << s.Find(3) << " " << s.Find(4) << endl;
    return 0;
}