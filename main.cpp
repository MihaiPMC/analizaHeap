#include <bits/stdc++.h>
#include <chrono>
#include <iomanip>
#include <functional>
#include <random>
#include <sys/stat.h>

using namespace std;
using namespace std::chrono;

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


void ensure_directory_exists(const string& dirPath) {
    #ifdef _WIN32
    mkdir(dirPath.c_str());
    #else
    mkdir(dirPath.c_str(), 0777);
    #endif
}

void run_benchmark(const string &filename)
{
    ifstream fin(filename);
    if (!fin.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    int n;
    fin >> n;

    vector<pair<int, int> > operations;
    for (int i = 0; i < n; i++)
    {
        int op, val;
        fin >> op >> val;
        operations.push_back({op, val});
    }
    fin.close();

    int group_size = n / 3;
    int insert_count = group_size;
    int search_count = group_size;
    int delete_count = group_size;

    double skip_insert_time = 0;
    double skip_search_time = 0;
    double skip_delete_time = 0;

    double treap_insert_time = 0;
    double treap_search_time = 0;
    double treap_delete_time = 0;

    skipList skip_list;
    treaps treap;

    {
        auto start_time = high_resolution_clock::now();
        for (int i = 0; i < group_size; i++)
        {
            skip_list.insert(operations[i].second);
        }
        auto end_time = high_resolution_clock::now();
        skip_insert_time = duration_cast<microseconds>(end_time - start_time).count() / 1000.0;
    }

    {
        auto start_time = high_resolution_clock::now();
        for (int i = 0; i < group_size; i++)
        {
            treap.insert(operations[i].second);
        }
        auto end_time = high_resolution_clock::now();
        treap_insert_time = duration_cast<microseconds>(end_time - start_time).count() / 1000.0;
    }

    {
        auto start_time = high_resolution_clock::now();
        for (int i = group_size; i < 2 * group_size; i++)
        {
            skip_list.search(operations[i].second);
        }
        auto end_time = high_resolution_clock::now();
        skip_search_time = duration_cast<microseconds>(end_time - start_time).count() / 1000.0;
    }

    {
        auto start_time = high_resolution_clock::now();
        for (int i = group_size; i < 2 * group_size; i++)
        {
            treap.search(operations[i].second);
        }
        auto end_time = high_resolution_clock::now();
        treap_search_time = duration_cast<microseconds>(end_time - start_time).count() / 1000.0;
            }
        
            {
        auto start_time = high_resolution_clock::now();
        for (int i = 2 * group_size; i < 3 * group_size; i++)
        {
            skip_list.remove(operations[i].second);
        }
        auto end_time = high_resolution_clock::now();
        skip_delete_time = duration_cast<microseconds>(end_time - start_time).count() / 1000.0;
    }

    {
        auto start_time = high_resolution_clock::now();
        for (int i = 2 * group_size; i < 3 * group_size; i++)
        {
            treap.remove(operations[i].second);
        }
        auto end_time = high_resolution_clock::now();
        treap_delete_time = duration_cast<microseconds>(end_time - start_time).count() / 1000.0;
    }

    double skip_avg_insert = insert_count > 0 ? skip_insert_time / insert_count : 0;
    double skip_avg_search = search_count > 0 ? skip_search_time / search_count : 0;
    double skip_avg_delete = delete_count > 0 ? skip_delete_time / delete_count : 0;

    double treap_avg_insert = insert_count > 0 ? treap_insert_time / insert_count : 0;
    double treap_avg_search = search_count > 0 ? treap_search_time / search_count : 0;
    double treap_avg_delete = delete_count > 0 ? treap_delete_time / delete_count : 0;

    cout << "Test: " << filename << endl;
    cout << "Operations: " << n << " (Insert: " << insert_count
        << ", Search: " << search_count
        << ", Delete: " << delete_count << ")" << endl;

    cout << "┌────────────┬─────────────────────────────────┬─────────────────────────────────┐" << endl;
    cout << "│ Operation  │ SkipList                        │ Treap                           │" << endl;
    cout << "│            ├───────────────┬─────────────────┼───────────────┬─────────────────┤" << endl;
    cout << "│            │ Total (ms)    │ Avg (ms/op)     │ Total (ms)    │ Avg (ms/op)     │" << endl;
    cout << "├────────────┼───────────────┼─────────────────┼───────────────┼─────────────────┤" << endl;
    
    cout << "│ Insert     │ " << setw(13) << fixed << setprecision(3) << skip_insert_time 
         << " │ " << setw(15) << fixed << setprecision(6) << skip_avg_insert 
         << " │ " << setw(13) << fixed << setprecision(3) << treap_insert_time 
         << " │ " << setw(15) << fixed << setprecision(6) << treap_avg_insert << " │" << endl;
    
    cout << "│ Search     │ " << setw(13) << fixed << setprecision(3) << skip_search_time 
         << " │ " << setw(15) << fixed << setprecision(6) << skip_avg_search 
         << " │ " << setw(13) << fixed << setprecision(3) << treap_search_time 
         << " │ " << setw(15) << fixed << setprecision(6) << treap_avg_search << " │" << endl;
    
    cout << "│ Delete     │ " << setw(13) << fixed << setprecision(3) << skip_delete_time 
         << " │ " << setw(15) << fixed << setprecision(6) << skip_avg_delete 
         << " │ " << setw(13) << fixed << setprecision(3) << treap_delete_time 
         << " │ " << setw(15) << fixed << setprecision(6) << treap_avg_delete << " │" << endl;

    double skip_total = skip_insert_time + skip_search_time + skip_delete_time;
    double treap_total = treap_insert_time + treap_search_time + treap_delete_time;
    
    cout << "├────────────┼───────────────┴─────────────────┼───────────────┴─────────────────┤" << endl;
    cout << "│ TOTAL      │ " << setw(31) << fixed << setprecision(3) << skip_total 
         << " │ " << setw(31) << fixed << setprecision(3) << treap_total << " │" << endl;
    
    cout << "└────────────┴─────────────────────────────────┴─────────────────────────────────┘" << endl;
    cout << "======================================================================" << endl;
    
    ensure_directory_exists("results");
    
    string test_name = filename;
    size_t last_slash = test_name.find_last_of('/');
    if (last_slash != string::npos) {
        test_name = test_name.substr(last_slash + 1);
    }
    size_t dot_pos = test_name.find_last_of('.');
    if (dot_pos != string::npos) {
        test_name = test_name.substr(0, dot_pos);
    }
    
    string csv_filename = "results/" + test_name + ".csv";
    ofstream csv_file(csv_filename);
    
    if (csv_file.is_open()) {
        csv_file << "Test," << test_name << endl;
        csv_file << "Total Operations," << n << endl;
        csv_file << "Insert Operations," << insert_count << endl;
        csv_file << "Search Operations," << search_count << endl;
        csv_file << "Delete Operations," << delete_count << endl;
        csv_file << endl;
        
        csv_file << "Operation,SkipList Total (ms),SkipList Avg (ms/op),Treap Total (ms),Treap Avg (ms/op)" << endl;
        
        csv_file << "Insert," << fixed << setprecision(3) << skip_insert_time << "," 
                << fixed << setprecision(6) << skip_avg_insert << "," 
                << fixed << setprecision(3) << treap_insert_time << "," 
                << fixed << setprecision(6) << treap_avg_insert << endl;
        
        csv_file << "Search," << fixed << setprecision(3) << skip_search_time << "," 
                << fixed << setprecision(6) << skip_avg_search << "," 
                << fixed << setprecision(3) << treap_search_time << "," 
                << fixed << setprecision(6) << treap_avg_search << endl;
        
        csv_file << "Delete," << fixed << setprecision(3) << skip_delete_time << "," 
                << fixed << setprecision(6) << skip_avg_delete << "," 
                << fixed << setprecision(3) << treap_delete_time << "," 
                << fixed << setprecision(6) << treap_avg_delete << endl;
        
        csv_file << "Total," << fixed << setprecision(3) << skip_total << "," 
                << "," << fixed << setprecision(3) << treap_total << "," << endl;
        
        csv_file.close();
        cout << "Results saved to " << csv_filename << endl;
    } else {
        cerr << "Error: Could not create CSV file " << csv_filename << endl;
    }
}

int main()
{
    cout << "Performance Benchmark: SkipList vs Treap" << endl;
    cout << "======================================================================" << endl;

    ensure_directory_exists("results");
    
    ofstream summary_csv("results/summary.csv");
    if (summary_csv.is_open()) {
        summary_csv << "Test,DataSize,DataPattern,SkipList Insert (ms),SkipList Search (ms),SkipList Delete (ms),SkipList Total (ms),"
                   << "Treap Insert (ms),Treap Search (ms),Treap Delete (ms),Treap Total (ms)" << endl;
    } else {
        cerr << "Error: Could not create summary CSV file" << endl;
    }

    vector<string> test_sizes = {"small", "medium", "large"};
    vector<string> test_types = {"increasing", "decreasing", "random", "equal"};

    for (const auto &size: test_sizes)
    {
        for (const auto &type: test_types)
        {
            string filename = "teste/" + size + "_" + type + ".txt";
            run_benchmark(filename);
            
            if (summary_csv.is_open()) {
                string test_csv = "results/" + size + "_" + type + ".csv";
                ifstream test_file(test_csv);
                
                if (test_file.is_open()) {
                    string line;
                    for (int i = 0; i < 7; i++) {
                        getline(test_file, line);
                    }
                    
                    string insert_line, search_line, delete_line, total_line;
                    getline(test_file, insert_line);
                    getline(test_file, search_line);
                    getline(test_file, delete_line);
                    getline(test_file, total_line);
                    
                    size_t pos = insert_line.find(',');
                    pos = insert_line.find(',', pos + 1);
                    string skip_insert = insert_line.substr(7, pos - 7);
                    pos = insert_line.find(',', pos + 1);
                    pos = insert_line.find(',', pos + 1);
                    string treap_insert = insert_line.substr(pos + 1);
                    pos = treap_insert.find(',');
                    treap_insert = treap_insert.substr(0, pos);
                    pos = search_line.find(',');
                    pos = search_line.find(',', pos + 1);
                    string skip_search = search_line.substr(7, pos - 7);
                    pos = search_line.find(',', pos + 1);
                    pos = search_line.find(',', pos + 1);
                    string treap_search = search_line.substr(pos + 1);
                    pos = treap_search.find(',');
                    treap_search = treap_search.substr(0, pos);
                    
                    pos = delete_line.find(',');
                    pos = delete_line.find(',', pos + 1);
                    string skip_delete = delete_line.substr(7, pos - 7);
                    pos = delete_line.find(',', pos + 1);
                    pos = delete_line.find(',', pos + 1);
                    string treap_delete = delete_line.substr(pos + 1);
                    pos = treap_delete.find(',');
                    treap_delete = treap_delete.substr(0, pos);
                    pos = total_line.find(',');
                    pos = total_line.find(',', pos + 1);
                    string skip_total = total_line.substr(6, pos - 6);
                    pos = total_line.find(',', pos + 1);
                    string treap_total = total_line.substr(pos + 1);
                    pos = treap_total.find(',');
                    treap_total = treap_total.substr(0, pos);
                    summary_csv << size << "_" << type << "," << size << "," << type << ","
                               << skip_insert << "," << skip_search << "," << skip_delete << "," << skip_total << ","
                               << treap_insert << "," << treap_search << "," << treap_delete << "," << treap_total << endl;
                    
                    test_file.close();
                }
            }
        }
    }
    
    if (summary_csv.is_open()) {
        summary_csv.close();
        cout << "======================================================================" << endl;
        cout << "Summary of all tests saved to results/summary.csv" << endl;
    }

    return 0;
}
