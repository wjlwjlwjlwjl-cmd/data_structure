#include <vector>
#include <time.h>
#include <random>
using namespace std;

struct SkipListNode{
    int _val;
    vector<SkipListNode*> _nextV;
    SkipListNode(int level, int val){
        _val = val;
        _nextV.resize(level, nullptr);
    }
};

class Skiplist {
public:
    Skiplist() {
        _head = new SkipListNode(1, -1);
        srand(time(0));
    }

    int randomLevel(){
        int level = 1;
        while(rand() <= RAND_MAX * _p && level <= _maxlevel){
            level++;
        }
        return level;
    }
    
    bool search(int target) {
        SkipListNode* cur = _head;
        int level = _head->_nextV.size() - 1;
        while(level >= 0){
            if(cur->_nextV[level] && cur->_nextV[level]->_val < target){
                cur = cur->_nextV[level];
            }
            else if(cur->_nextV[level] == nullptr || cur->_nextV[level]->_val > target){
                level--;
            }
            else{
                return true;
            }
        }
        return false;
    }

    vector<SkipListNode*> findPrevNodes(int num){
        int n = _head->_nextV.size();
        vector<SkipListNode*> ret(n, _head);
        int level = n - 1;
        SkipListNode* cur = _head;
        while(level >= 0){
            if(cur->_nextV[level] && cur->_nextV[level]->_val < num){
                cur = cur->_nextV[level];
            }
            else if(cur->_nextV[level] == nullptr || cur->_nextV[level]->_val >= num)
            {
                ret[level] = cur;
                level--;
            }
        }
        return ret;
    }
    
    void add(int num) {
        vector<SkipListNode*> prevV = findPrevNodes(num);
        int level = randomLevel();
        SkipListNode* newNode = new SkipListNode(level, num);
        if(_head->_nextV.size() < level){
            _head->_nextV.resize(level, nullptr);
            prevV.resize(level, _head);
        }       
        
        for(int i = 0; i < level; i++){
            newNode->_nextV[i] = prevV[i]->_nextV[i];
            prevV[i]->_nextV[i] = newNode;
        }
    }
    
    bool erase(int num) {
        vector<SkipListNode*> prevV = findPrevNodes(num);

        if(prevV[0]->_nextV[0] == nullptr || prevV[0]->_nextV[0]->_val != num){
            return false;
        }

        SkipListNode* del = prevV[0]->_nextV[0];
        for(int i = 0; i < del->_nextV.size(); i++){
            prevV[i]->_nextV[i] = del->_nextV[i];
        }
        delete del;
        
        int i = _head->_nextV.size() - 1;
        while(i >= 0){
            if(_head->_nextV[i] == nullptr){
                i--;
            }
            else{
                break;
            }
        }
        _head->_nextV.resize(i + 1);
        return true;
    }
private:
    double _p = 0.25;
    int _maxlevel = 32;
    SkipListNode* _head;
};