#pragma once

#include <iostream>
#include <algorithm>

template <class K, int M = 1024>
struct BTreeNode{
    K _keys[M]; 
    BTreeNode<K, M>* _subs[M + 1];
    BTreeNode<K, M>* _parent = nullptr;
    int _size;
    BTreeNode(){
        _parent = nullptr;
        _size = 0;
        for(int i = 0; i < M; i++){
            _subs[i] = nullptr;
            _keys[i] = K();
        }
    }
};

template <class K, int M = 1024>
class BTree{
public:
    typedef BTreeNode<K, M> BNode;
    BTree(){}

    std::pair<BNode*, int> Find(K& key){
        BNode* pCur = _root;
        BNode* pParent = nullptr;
        while(pCur){
            int i = 0;
            for(; i < pCur->_size; i++){
                if(key < pCur->_keys[i]){
                    break;
                }
                else if(key == pCur->_keys[i]){
                    return std::make_pair(pCur, i);
                }
            }
            pParent = pCur;
            pCur = pCur->_subs[i];
        }
        //没有找到才需要插入
        return std::make_pair(pParent, -1);
    }

    void _Insert(BNode* pCur, BNode* brother, K& key){
        int end = pCur->_size - 1;
        while(end >= 0){
            if(pCur->_keys[end] < key){
                break;
            }
            else{
                pCur->_keys[end + 1] = pCur->_keys[end];
                pCur->_subs[end + 2] = pCur->_subs[end + 1];
                end--;
            }
        }
        pCur->_keys[end + 1] = key;
        pCur->_subs[end + 2] = brother;
        if(brother){
            brother->_parent = pCur;
        }
        pCur->_size++;
    }

    bool Insert(K& key){
        if(_root == nullptr){
            _root = new BNode();
            _root->_keys[_root->_size++] = key;
            return true;
        }
        std::pair<BNode*, int> ret = Find(key);
        if(ret.second >= 0){
            return false;
        }

        BNode* pCur = ret.first;
        BNode* brother = nullptr;
        K newkey = key;
        while(1){
            _Insert(pCur, brother, newkey);
            if(pCur->_size < M){
                return true;
            }

            int mid = M >> 1;
            brother = new BNode();
            for(int i = mid + 1; i < pCur->_size; i++){
                brother->_subs[brother->_size] = pCur->_subs[i];
                if(pCur->_subs[i]){
                    pCur->_subs[i]->_parent = brother;
                }
                brother->_keys[brother->_size++] = pCur->_keys[i];
            }
            brother->_subs[brother->_size] = pCur->_subs[pCur->_size]; //分裂出来的兄弟要多拷贝一个右孩子
            if(pCur->_subs[pCur->_size]){
                pCur->_subs[pCur->_size]->_parent = brother;
            }
            pCur->_size -= brother->_size + 1;
            if(pCur->_parent == nullptr){
                BNode* newRoot = new BNode();
                newRoot->_keys[newRoot->_size++] = pCur->_keys[mid];
                newRoot->_subs[0] = pCur;
                newRoot->_subs[1] = brother;
                pCur->_parent = newRoot;
                brother->_parent = newRoot;
                _root = newRoot;
                return true;
            }
            else{
                newkey = pCur->_keys[mid];
                pCur = pCur->_parent;
            }
        }
        return true;
    }

    void Height(){
        int height = 1;
        BNode* pCur = _root;
        while(pCur){
            pCur = pCur->_subs[0];
            height++;
        }
        std::cout << "height: " << height << std::endl;
    }

    void Inorder(){
        _inorder(_root);
    }
private:
    void _inorder(BNode* root){
        if(root == nullptr){
            return;
        }
        for(int i = 0; i < root->_size; i++){
            _inorder(root->_subs[i]);
            std::cout << root->_keys[i] << " ";
        }
        _inorder(root->_subs[root->_size]);
    }
    BNode* _root = nullptr;
};