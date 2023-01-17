#include <iostream>
using namespace std;

template <class T>
class AVLTree
{
    friend class Bike_mgmt;

    class TreeNode
    {
    public:
        T data;
        TreeNode *left;
        TreeNode *right;
        TreeNode *parent;
        int height;
        TreeNode();
        TreeNode(T d);
        ~TreeNode();
        void UpdateHeight();
        int bf();
    };

public:
    TreeNode *root;
    int num;

    AVLTree();
    ~AVLTree();
    TreeNode *Insert(TreeNode *root, T data);
    TreeNode *Delete(TreeNode *root, T data);
    TreeNode *Rebalance(TreeNode *node);
    TreeNode *rightRotate(TreeNode *node);
    TreeNode *leftRotate(TreeNode *node);
    TreeNode *Insert_with_ID_First(TreeNode* root, T data);
    void Decending(TreeNode *root);
    void Acending(TreeNode *root, AVLTree<T>* t);
    void Write_Station_Status(TreeNode* root, string result_path, ofstream& file);
    void Write_File(TreeNode* root, string result_path, ofstream& file);
    bool isEmpty();
    T getMaxBike(TreeNode* root, int s_time);
};

template <class T>
AVLTree<T>::TreeNode::TreeNode()
{
    T init;
    data = init;
    left = NULL;
    right = NULL;
    parent = NULL;
    height = 0;
}

template <class T>
AVLTree<T>::TreeNode::TreeNode(T d)
{
    data = d;
    left = NULL;
    right = NULL;
    parent = NULL;
    height = 0;
}

template <class T>
AVLTree<T>::TreeNode::~TreeNode() {}

template <class T>
void AVLTree<T>::TreeNode::UpdateHeight()
{
    int lh = 0, rh = 0;
    if (left != NULL)
    {
        lh = (*left).height;
    }

    if (right != NULL)
    {
        rh = (*right).height;
    }

    if (lh >= rh)
    {
        height = lh + 1;
    }
    else
    {
        height = rh + 1;
    }
}

template <class T>
int AVLTree<T>::TreeNode::bf()
{
    int lh = 0, rh = 0;
    if (left != NULL)
    {
        lh = (*left).height;
    }

    if (right != NULL)
    {
        rh = (*right).height;
    }

    return lh - rh;
}

template <class T>
AVLTree<T>::AVLTree()
{
    root = NULL;
    num = 0;
}

template <class T>
AVLTree<T>::~AVLTree() {}

template <class T>
typename AVLTree<T>::TreeNode *AVLTree<T>::Insert(TreeNode *root, T data)
{
    if (root == NULL)
    {
        root = new TreeNode(data);
        num++;
    }
    else
    {
        // descending order!!!
        if (root->data > data)
        {
            root->left = Insert(root->left, data);
            root->left->parent = root;
        }
        else if (root->data < data)
        {
            root->right = Insert(root->right, data);
            root->right->parent = root;
        }
    }

    root->UpdateHeight();

    return Rebalance(root);
}

template <class T>
typename AVLTree<T>::TreeNode *AVLTree<T>::Delete(TreeNode *node, T data)
{
    if (node != NULL)
    {
        if (node->data > data)
        {
            node->left = Delete(node->left, data);
            if (node->left != NULL)
            {
                node->left->parent = node;
            }
        }
        else if (node->data < data)
        {
            node->right = Delete(node->right, data);
            if (node->right != NULL)
            {
                node->right->parent = node;
            }
        }
        else // node->data == data
        {
            if (node->left == NULL && node->right == NULL)
            {
                num--;
                delete node;
                node = NULL;
            }
            else if (node->left != NULL && node->right == NULL)
            {
                num--;
                TreeNode *del = node;
                node = node->left;
                delete del;
                del = NULL;
            }
            else if (node->right != NULL && node->left == NULL)
            {
                num--;
                TreeNode *del = node;
                node = node->right;
                delete del;
                del = NULL;
            }
            else // have two child
            {
                TreeNode *tmp = node->right;
                while (tmp->left != NULL)
                {
                    tmp = tmp->left;
                }

                node->data = tmp->data;

                node->right = Delete(node->right, tmp->data);
                if (node->right != NULL)
                {
                    node->right->parent = node;
                }
            }
        }
    }

    if (node != NULL)
    {
        node->UpdateHeight();
    }

    return node;
}

template <class T>
typename AVLTree<T>::TreeNode *AVLTree<T>::Rebalance(TreeNode *node)
{
    // LL Case
    if (node->bf() > 1 && node->left->bf() > 0)
    {
        node = rightRotate(node);
    }

    // LR Case
    if (node->bf() > 1 && node->left->bf() < 0)
    {
        node->left = leftRotate(node->left);
        node = rightRotate(node);
    }

    // RR Case
    if (node->bf() < -1 && node->right->bf() < 0)
    {
        node = leftRotate(node);
    }

    // RL Case
    if (node->bf() < -1 && node->right->bf() > 0)
    {
        node->right = rightRotate(node->right);
        node = leftRotate(node);
    }

    return node;
}

template <class T>
typename AVLTree<T>::TreeNode *AVLTree<T>::rightRotate(TreeNode *node)
{
    TreeNode *nl = node->left;
    TreeNode *nlr = nl->right;

    node->left = nlr;
    node->UpdateHeight();

    nl->right = node;
    nl->UpdateHeight();

    return nl;
}

template <class T>
typename AVLTree<T>::TreeNode *AVLTree<T>::leftRotate(TreeNode *node)
{
    TreeNode *nr = node->right;
    TreeNode *nrl = nr->left;

    node->right = nrl;
    node->UpdateHeight();

    nr->left = node;
    nr->UpdateHeight();

    return nr;
}

template <class T>
void AVLTree<T>:: Decending(TreeNode *root)
{
    if (root != NULL)
    {
        Decending(root->right);
        cout << root->data << endl;
        Decending(root->left);
    }
}

template <class T>
void AVLTree<T>:: Acending(TreeNode *root, AVLTree<T>* t)
{
    if (root != NULL)
    {
        Acending(root->left, t);
        (*t).root = (*t).Insert_with_ID_First((*t).root, root->data);
        Acending(root->right, t);
    }
}

template <class T>
void AVLTree<T>:: Write_Station_Status(TreeNode* root, string result_path, ofstream& file)
{
    if(root != NULL)
    {
        Write_Station_Status(root->left, result_path, file);

        // write into file
        file.open(result_path + "/station_status.txt", ios::app);
        // station_id, bike_id, bike_type, rental_price, and rental_count
        string sid = "S" + to_string(root->data.station);
        string bid = to_string(root->data.id);
        string type = "B" + to_string(root->data.type);

        // rounding the float price
        float rounding_price = root->data.price;
        int ir_price = rounding_price * 100;
        if((ir_price % 10) >= 5)
        {
            ir_price += 10;
        }
        rounding_price = ir_price;
        rounding_price /= 100.0;

        string price = to_string(rounding_price);
        for(int i=0; i<price.size(); i++)
        {
            if(price[i] == '.' && price[i+1] != '0')
            {
                price = price.substr(0, i+2);
            }
            else if(price[i] == '.' && price[i+1] == '0')
            {
                price = price.substr(0, i);
            }
        }
        
        string count = to_string(root->data.count);
        string res = sid + " " + bid + " " + type + " " + price + " " + count + "\n";
        file << res;
        file .close();

        Write_Station_Status(root->right, result_path, file);

    }
}

template <class T>
void AVLTree<T>:: Write_File(TreeNode* root, string result_path, ofstream& file)
{
    if(root != NULL)
    {
        Write_File(root->left, result_path, file);

        // write into file
        file.open(result_path, ios::app);
        file << root->data.str;
        file.close();

        Write_File(root->right, result_path, file);
    }
}

template <class T>
bool AVLTree<T>::isEmpty()
{
    if (num == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <class T>
void Print(T *A, T len)
{
    for (int i = 0; i < len; i++)
    {
        cout << A[i] << " ";
    }
    cout << endl;
}

template <class T>
T AVLTree<T>:: getMaxBike(TreeNode* root, int s_time)
{
    Bike init;

    if(root != NULL)
    {
        init = getMaxBike(root->right, s_time);
        if(init.id < 0)
        {
            if(root->data.s_time <= s_time)
            {
                init = root->data;
            }
            else
            {
                init = getMaxBike(root->left, s_time);
            }
        }
    }

    return init;
}

template <class T>
typename AVLTree<T>::TreeNode * AVLTree<T>:: Insert_with_ID_First(TreeNode* root, T data)
{
    if (root == NULL)
    {
        root = new TreeNode(data);
        num++;
    }
    else
    {
        // ascending order!!!
        if (root->data.id > data.id)
        {
            root->left = Insert_with_ID_First(root->left, data);
            root->left->parent = root;
        }
        else if (root->data.id < data.id)
        {
            root->right = Insert_with_ID_First(root->right, data);
            root->right->parent = root;
        }
    }

    root->UpdateHeight();

    return Rebalance(root);
}

// ### test code ###
// int main()
// {

//     int *A = new int[10];
//     for (int i = 0; i < 10; i++)
//     {
//         A[i] = 9 - i;
//     }
//     cout << "Initial: ";
//     Print(A, 10);

//     AVLTree<int> t;
//     for (int i = 0; i < 10; i++)
//     {
//         cout << "Insert : " << A[i] << endl;
//         t.root = t.Insert(t.root, A[i]);
//         cout << "root = " << t.root->data << endl;
//         cout << "Inorder: " << endl;
//         t.Inorder(t.root);
//         cout << "\n"
//              << endl;
//     }

//     t.root = t.Delete(t.root, 1);
//     cout << "Inorder: ";
//     t.Inorder(t.root);
//     cout << endl;

//     return 0;
// }