#include <bits/stdc++.h>

using namespace std;

const int MAX_LEVEL = 16;
const float PROBABILITY = 0.5;

class skipList
{
private:
    struct Node
    {
        int value;
        vector<Node *> forward;

        Node(int val, int level) : value(val), forward(level + 1, nullptr)
        {
        }
    };

    Node *header;
    int level;

public:
    int randomLevel()
    {
        int lvl = 0;
        while (((float) rand() / RAND_MAX) < PROBABILITY && lvl < MAX_LEVEL - 1)
        {
            lvl++;
        }
        return lvl;
    }

    skipList()
    {
        header = new Node(-1, MAX_LEVEL);
        level = 0;
    }

    ~skipList()
    {
        Node *current = header->forward[0];
        while (current != nullptr)
        {
            Node *next = current->forward[0];
            delete current;
            current = next;
        }
        delete header;
    }

    void insert(int x)
    {
        vector<Node *> update(MAX_LEVEL + 1, nullptr);
        Node *current = header;

        for (int i = level; i >= 0; i--)
        {
            while (current->forward[i] != nullptr && current->forward[i]->value < x)
            {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && current->value == x)
        {
            return;
        }

        int newLevel = randomLevel();

        if (newLevel > level)
        {
            for (int i = level + 1; i <= newLevel; i++)
            {
                update[i] = header;
            }
            level = newLevel;
        }

        Node *newNode = new Node(x, newLevel);

        for (int i = 0; i <= newLevel; i++)
        {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }

    void remove(int x)
    {
        vector<Node *> update(MAX_LEVEL + 1, nullptr);
        Node *current = header;

        for (int i = level; i >= 0; i--)
        {
            while (current->forward[i] != nullptr && current->forward[i]->value < x)
            {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current == nullptr || current->value != x)
        {
            return;
        }

        for (int i = 0; i <= level; i++)
        {
            if (update[i]->forward[i] != current)
            {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }

        delete current;

        while (level > 0 && header->forward[level] == nullptr)
        {
            level--;
        }
    }

    bool search(int x)
    {
        Node *current = header;

        for (int i = level; i >= 0; i--)
        {
            while (current->forward[i] != nullptr && current->forward[i]->value < x)
            {
                current = current->forward[i];
            }
        }

        current = current->forward[0];

        return (current != nullptr && current->value == x);
    }

    int findLargestLessOrEqual(int x)
    {
        Node *current = header;

        for (int i = level; i >= 0; i--)
        {
            while (current->forward[i] != nullptr && current->forward[i]->value <= x)
            {
                current = current->forward[i];
            }
        }

        if (current == header)
        {
            return -1;
        }

        return current->value;
    }

    int findSmallestGreaterOrEqual(int x)
    {
        Node *current = header;

        for (int i = level; i >= 0; i--)
        {
            while (current->forward[i] != nullptr && current->forward[i]->value < x)
            {
                current = current->forward[i];
            }
        }

        current = current->forward[0];

        if (current == nullptr)
        {
            return -1;
        }

        return current->value;
    }

    void printRange(int x, int y)
    {
        if (x > y)
        {
            return;
        }

        Node *current = header;

        for (int i = level; i >= 0; i--)
        {
            while (current->forward[i] != nullptr && current->forward[i]->value < x)
            {
                current = current->forward[i];
            }
        }

        current = current->forward[0];

        while (current != nullptr && current->value <= y)
        {
            cout << current->value << " ";
            current = current->forward[0];
        }
        cout << endl;
    }
};

class treaps
{
private:
    struct Node
    {
        int value;
        int priority;
        Node *left;
        Node *right;

        Node(int val) : value(val), priority(rand()), left(nullptr), right(nullptr)
        {
        }
    };

    Node *root;

    Node *rotateRight(Node *y)
    {
        Node *x = y->left;
        Node *T2 = x->right;

        x->right = y;
        y->left = T2;

        return x;
    }

    Node *rotateLeft(Node *x)
    {
        Node *y = x->right;
        Node *T2 = y->left;

        y->left = x;
        x->right = T2;

        return y;
    }

    Node *insertNode(Node *root, int value)
    {
        if (root == nullptr)
            return new Node(value);

        if (value == root->value)
            return root;

        if (value < root->value)
        {
            root->left = insertNode(root->left, value);

            if (root->left != nullptr && root->left->priority > root->priority)
                root = rotateRight(root);
        }
        else
        {
            root->right = insertNode(root->right, value);

            if (root->right != nullptr && root->right->priority > root->priority)
                root = rotateLeft(root);
        }

        return root;
    }

    Node *deleteNode(Node *root, int value)
    {
        if (root == nullptr)
            return root;

        if (value < root->value)
            root->left = deleteNode(root->left, value);
        else if (value > root->value)
            root->right = deleteNode(root->right, value);
        else
        {
            if (root->left == nullptr && root->right == nullptr)
            {
                delete root;
                return nullptr;
            }
            else if (root->left == nullptr)
            {
                Node *temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr)
            {
                Node *temp = root->left;
                delete root;
                return temp;
            }
            else if (root->left->priority < root->right->priority)
            {
                root = rotateLeft(root);
                root->left = deleteNode(root->left, value);
            }
            else
            {
                root = rotateRight(root);
                root->right = deleteNode(root->right, value);
            }
        }

        return root;
    }

    bool searchNode(Node *root, int value)
    {
        if (root == nullptr)
            return false;

        if (root->value == value)
            return true;

        if (value < root->value)
            return searchNode(root->left, value);
        else
            return searchNode(root->right, value);
    }

    int findLargestLENode(Node *root, int x, int &result)
    {
        if (root == nullptr)
            return result;

        if (root->value <= x)
        {
            result = max(result, root->value);
            return findLargestLENode(root->right, x, result);
        }
        else
        {
            return findLargestLENode(root->left, x, result);
        }
    }

    int findSmallestGENode(Node *root, int x, int &result)
    {
        if (root == nullptr)
            return result;

        if (root->value >= x)
        {
            result = (result == -1) ? root->value : min(result, root->value);
            return findSmallestGENode(root->left, x, result);
        }
        else
        {
            return findSmallestGENode(root->right, x, result);
        }
    }

    void inorderRange(Node *root, int x, int y, vector<int> &result)
    {
        if (root == nullptr)
            return;

        if (root->value > x)
            inorderRange(root->left, x, y, result);

        if (root->value >= x && root->value <= y)
            result.push_back(root->value);

        if (root->value < y)
            inorderRange(root->right, x, y, result);
    }

    void cleanUp(Node *root)
    {
        if (root == nullptr)
            return;

        cleanUp(root->left);
        cleanUp(root->right);
        delete root;
    }

public:
    treaps() : root(nullptr)
    {
    }

    ~treaps()
    {
        cleanUp(root);
    }

    void insert(int x)
    {
        root = insertNode(root, x);
    }

    void remove(int x)
    {
        root = deleteNode(root, x);
    }

    bool search(int x)
    {
        return searchNode(root, x);
    }

    int findLargestLessOrEqual(int x)
    {
        int result = -1;
        findLargestLENode(root, x, result);
        return result;
    }

    int findSmallestGreaterOrEqual(int x)
    {
        int result = -1;
        findSmallestGENode(root, x, result);
        return result;
    }

    void printRange(int x, int y)
    {
        vector<int> result;
        inorderRange(root, x, y, result);

        for (int num: result)
            cout << num << " ";
        cout << endl;
    }
};

int main()
{
    /*
    treaps list;
    int q, op, x, y;

    srand(time(NULL));

    cin >> q;
    while (q--) {
        cin >> op;
        switch(op) {
            case 1:
                cin >> x;
                list.insert(x);
                break;

            case 2:
                cin >> x;
                list.remove(x);
                break;

            case 3:
                cin >> x;
                cout << (list.search(x) ? 1 : 0) << endl;
                break;

            case 4:
                cin >> x;
                y = list.findLargestLessOrEqual(x);
                if (y != -1) {
                    cout << y << endl;
                }
                break;

            case 5:
                cin >> x;
                y = list.findSmallestGreaterOrEqual(x);
                if (y != -1) {
                    cout << y << endl;
                }
                break;

            case 6:
                cin >> x >> y;
                list.printRange(x, y);
                break;
        }
    }*/

    /*skipList list;
    int q, op, x, y;

    srand(time(NULL));

    cin >> q;
    while (q--)
    {
        cin >> op;
        switch (op)
        {
            case 1:
                cin >> x;
                list.insert(x);
                break;

            case 2:
                cin >> x;
                list.remove(x);
                break;

            case 3:
                cin >> x;
                cout << (list.search(x) ? 1 : 0) << endl;
                break;

            case 4:
                cin >> x;
                y = list.findLargestLessOrEqual(x);
                if (y != -1)
                {
                    cout << y << endl;
                }
                break;

            case 5:
                cin >> x;
                y = list.findSmallestGreaterOrEqual(x);
                if (y != -1)
                {
                    cout << y << endl;
                }
                break;

            case 6:
                cin >> x >> y;
                list.printRange(x, y);
                break;
        }
    }*/

    return 0;
}
