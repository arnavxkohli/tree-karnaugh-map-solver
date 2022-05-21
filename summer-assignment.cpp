#include <iostream>
#include <vector>
#include <string>

struct BNode {
    std::string val;
    BNode* left;
    BNode* right;
};

BNode* cons_B(std::string e, BNode* l, BNode* r) {
    BNode* tmp;
    tmp = new BNode;
    tmp->val = e;
    tmp->left = l;
    tmp->right = r;
    return tmp;
}

void createSkeletonB(BNode*& b, int level, int current_level) {
    if (b == NULL) {
        if (current_level == level + 1) {
            b = cons_B("0", NULL, NULL);
        }
        else {
            b = cons_B("x" + std::to_string(current_level), NULL, NULL);
        }
    }
    if (current_level <= level) {
        createSkeletonB(b->left, level, current_level + 1);
        createSkeletonB(b->right, level, current_level + 1);
    }
}

BNode* traverseBNode(BNode* b, const std::string& eval_in, int index) {
    if ((eval_in[index] - '0') == 1) {
        return traverseBNode(b->right, eval_in, index + 1);
    }
    else if ((eval_in[index] - '0') == 0) {
        return traverseBNode(b->left, eval_in, index + 1);
    }
    return b;
}

void fill1s(BNode*& b, const std::vector<std::string>& fvalues) {
    BNode* tmp;
    for (int i = 0; i < fvalues.size(); i++) {
        tmp = traverseBNode(b, fvalues[i], 0);
        tmp->val = "1";
    }
}

void subNodeReduction(BNode*& b) {
    if (b->left->val == "0" || b->left->val == "1") {
        if (b->left->val == b->right->val) {
            b->val = b->left->val;
            b->left = NULL;
            b->right = NULL;
        }
        return;
    }
    subNodeReduction(b->left);
    subNodeReduction(b->right);
}

bool mirrorCompare(BNode* tmp1, BNode* tmp2) {
    if (tmp1->val == tmp2->val) {
        if (tmp1->val == "0" || tmp1->val == "1") {
            return true;
        }
        return mirrorCompare(tmp1->left, tmp2->left) && mirrorCompare(tmp1->right, tmp2->right);
    }
    return false;
}

void mirrorReduction(BNode*& b) {
    if (mirrorCompare(b->left, b->right) == true) {
        b = b->left;
    }
}

void deallocate_BNode(BNode* b) {
    if (b != NULL) {
        deallocate_BNode(b->left);
        deallocate_BNode(b->right);
        delete b;
    }
}

BNode* build_bt(const std::vector<std::string>& fvalues) {
    BNode* tmp = NULL;
    int height = fvalues[0].length();

    createSkeletonB(tmp, height, 1);
    fill1s(tmp, fvalues);

    subNodeReduction(tmp);
    mirrorReduction(tmp);

    return tmp;
}

int label_to_idx(const std::string& label) {

    std::string out;

    for (int i = 1; i < label.size(); i++) {
        out.push_back(label[i]);
    }

    return std::stoi(out) - 1;
}

std::string eval_bt(BNode* bt, const std::string& input) {

    if ((bt->left == NULL) && (bt->right == NULL)) {
        return bt->val;
    }
    else {
        int idx = label_to_idx(bt->val);
        std::string input_idx;
        input_idx.push_back(input[idx]);

        if (input_idx == "0") {
            return eval_bt(bt->left, input);
        }
        else {
            return eval_bt(bt->right, input);
        }
    }
}

int n_nodes_bt(BNode* t) {
    if (t == NULL) {
        return 0;
    }
    else {
        return 1 + n_nodes_bt(t->left) + n_nodes_bt(t->right);
    }
}

class BoolTree {
public:

    BoolTree(const std::vector<std::string>& fvalues) {
        t = build_bt(fvalues);
    }

    std::string eval(const std::string& s) {
        return eval_bt(t, s);
    }

    int n_nodes() {
        return n_nodes_bt(t);
    }

    ~BoolTree() {
        deallocate_BNode(t);
    }

private:
    BNode* t;
};

int main() {

    std::vector<std::string> fv;
    std::string row;

    row = "11";
    fv.push_back(row);

    BoolTree ft1(fv);
    // as in the second assignment we give as input only the rows
    // of the truth table whose value is 1
    // (this is an example with the boolean "and" function)

    fv.clear();

    row = "010";
    fv.push_back(row);
    row = "011";
    fv.push_back(row);
    row = "101";
    fv.push_back(row);
    row = "111";
    fv.push_back(row);

    BoolTree ft2(fv);
    // this corresponds to the f(x1, x2, x3) example shown above

    std::cout << ft1.n_nodes() << std::endl;
    // we expect this to print 5

    std::cout << ft2.n_nodes() << std::endl;
    // if the algorithm is such that it builds the smallest possible corresponding tree
    // for the boolean function we are considering
    // then this will print 3 (see tree diagram in the example above)

    std::cout << ft1.eval("01") << std::endl;
    // this should print "0" 

    std::cout << ft1.eval("11") << std::endl;
    // this should print "1"

    std::cout << ft2.eval("001") << std::endl;
    // this should print "0"

    std::cout << ft2.eval("110") << std::endl;
    // this should print "0"

}

