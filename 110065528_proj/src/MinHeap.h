#ifndef MINHEAP_H
#define MINHEAP_H

#include <iostream>
using namespace std;


template <class T>
class MinHeap
{
    friend class Request_mgmt;
    friend class User;
    friend class Bike_mgmt;
protected:
    class TreeNode
    {
    public:
        T data;
        TreeNode *left;
        TreeNode *right;
        TreeNode *parent;
        TreeNode();
        TreeNode(T d);
        ~TreeNode();
    };

    TreeNode *root;
    TreeNode *last;
    int num; // # of nodes in heap
public:
    MinHeap();
    ~MinHeap();
    void DeleteTree(TreeNode *tmp);
    void Insert(T n);
    TreeNode *FindParent(); // find the parent node of the new node
    T Top();
    void Ins_Heapify(TreeNode *cur); // pure virtual function, it depends on which ds of your TreeNode
    void Pop();
    bool isEmpty();
    int getNum();
    virtual void Print(TreeNode *tmp);
    TreeNode *getRoot();
};

template <class T>
MinHeap<T>::TreeNode::TreeNode()
{
    T init;
    data = init;
    left = NULL;
    right = NULL;
    parent = NULL;
}

template <class T>
MinHeap<T>::TreeNode::TreeNode(T d)
{
    data = d;
    left = NULL;
    right = NULL;
    parent = NULL;
}

template <class T>
MinHeap<T>::TreeNode::~TreeNode()
{
    T init;
    data = init;
    left = NULL;
    right = NULL;
    parent = NULL;
}

template <class T>
MinHeap<T>::MinHeap()
{
    root = NULL;
    last = NULL;
    num = 0;
}

template <class T>
MinHeap<T>::~MinHeap()
{
    DeleteTree(root);
}

template <class T>
void MinHeap<T>::DeleteTree(TreeNode *tmp)
{
    if (tmp != NULL)
    {
        DeleteTree(tmp->left);
        DeleteTree(tmp->right);
        delete tmp;
        tmp = NULL;
    }
}

template <class T>
typename MinHeap<T>::TreeNode *MinHeap<T>::FindParent()
{
    if (root == NULL)
    {
        return root;
    }
    else // root != NULL
    {
        int p_id = num / 2;

        if (p_id == 0)
        {
            return NULL;
        }

        TreeNode *p_node = root;

        // turn p_id into binary number
        int bin[30];
        for (int i = 0; i < 30; i++)
        {
            bin[i] = -1;
        }

        int idx = 0;
        while (p_id > 0)
        {
            bin[idx++] = p_id % 2;
            p_id /= 2;
        }
        idx--;

        while ((--idx) >= 0)
        {
            if (bin[idx] == 0)
            {
                p_node = p_node->left;
            }
            else
            {
                p_node = p_node->right;
            }
        }

        return p_node;
    }
}

template <class T>
void MinHeap<T>::Insert(T n)
{
    TreeNode *node = new TreeNode(n);
    last = node;
    num++;

    if (root == NULL)
    {
        root = node;
    }
    else
    {
        TreeNode *p_node = FindParent();
        if (p_node != NULL)
        {
            if (p_node->left == NULL)
            {
                p_node->left = node;
            }
            else
            {
                p_node->right = node;
            }
            node->parent = p_node;

            Ins_Heapify(node);
        }
    }

}

template <class T>
T MinHeap<T>::Top()
{
    return root->data;
}

template <class T>
void MinHeap<T>::Ins_Heapify(TreeNode *cur)
{
    TreeNode *p_node = cur->parent;

    T tmp = cur->data;
    while (p_node != NULL)
    {
        if (p_node->data > tmp)
        {
            cur->data = p_node->data;
            cur = p_node;
            p_node = p_node->parent;
        }
        else
        {
            break;
        }
    }
    cur->data = tmp;
}

template <class T>
void MinHeap<T>::Pop()
{
    TreeNode *del = last;
    TreeNode *del_p = last->parent;

    if (del_p != NULL)
    {
        // move the del node outside the heap
        if (del == del_p->left)
        {
            del_p->left = NULL;
        }
        else
        {
            del_p->right = NULL;
        }
        del->parent = NULL;
    }

    // define new last node
    num--;
    TreeNode *new_last_p = FindParent();
    if (new_last_p != NULL)
    {
        if (new_last_p->right != NULL)
        {
            last = new_last_p->right;
        }
        else
        {
            last = new_last_p->left;
        }
    }
    else
    {
        if (num > 0)
        {
            last = root;
        }
        else // delete root
        {
            last = NULL;
        }
    }
    
    // do the heapify form top to down
    TreeNode *cur = root;
    TreeNode *cur_p = NULL;

    if (cur == del)
    {
        delete del;
        root = NULL;
    }
    else
    {
        while (cur != NULL)
        {
            // define the min child
            TreeNode *min_child = NULL;
            if (cur->right == NULL)
            {
                min_child = cur->left;
            }
            else if (cur->left != NULL) // both left child and right child are not NULL
            {
                if (cur->left->data < cur->right->data)
                {
                    min_child = cur->left;
                }
                else
                {
                    min_child = cur->right;
                }
            }

            // compare "del->data" and "min_child->data"
            if (min_child != NULL)
            {
                if (min_child->data < del->data)
                {
                    cur->data = min_child->data;
                    cur = min_child;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        // put the data into the right node
        if (cur != NULL)
        {
            cur->data = del->data;
        }

        // delete the old last node
        delete del;
        del = NULL;
    }
}

template <class T>
bool MinHeap<T>::isEmpty()
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
int MinHeap<T>::getNum()
{
    return num;
}

template <class T>
void MinHeap<T>::Print(TreeNode *tmp)
{
    if (tmp != NULL)
    {
        Print(tmp->left);
        cout << tmp->data << " ";
        Print(tmp->right);
    }
}

template <class T>
typename MinHeap<T>::TreeNode *MinHeap<T>::getRoot()
{
    return root;
}

#endif
