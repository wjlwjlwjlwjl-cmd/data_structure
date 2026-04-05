### SkipList 跳表

#### 一、What's SkipList？

**SkipList**可以认为是优化过的链表。通过一个数据域搭配多个指针域，能够实现类似与二分查找的效果，将原来链表的`O(N)`的时间复杂度，优化到`logN`

同时为了避免`logN`的时间复杂度在插入或者删除的过程中退化（各个节点一样高的话，就会退化成链表），采取随机数的方式，来决定每一个节点的高度。当然也并不是随便什么数都可以拿来当作高度，我们还引入了一个`[0,1]`的增长概率p，来保证既能防止跳表在插入、删除过程中退化为俩表，又能将节点的高度控制在合理的范围内

#### 二、跳表查询的过程

![skiplist](./skiplist.png)

比如在上面的跳表中，我们去查询11。

1. 首先从最上面一层开始，找到16，发现比11大，说明11如果存在的话，那么一定在头节点到16之间，16之后的就都不用查找了
2. 接下来找第二层，找到8，发现比11小，说明11存在的话就一定在8和16之间，头节点到8之间就不用找了
3. 然后找地三层，找到13，发现比11大，说明11存在的话就一定在8和13之间
4. 找第四层，找到11

**总结**：不难发现，跳表每次查找，都会抛弃掉一部分一定不会存在结果的查找，就像二分查找一样，每次都会抛弃一半的遍历，不过这里并不一定是一半，更多更少都有可能

#### 三、跳表的效率如何保证？

1. 前面提到过，可能会出现增加、删除节点导致跳表退化为链表的的情况，比如上面图中，删除所有高度为1、3、4的节点，就会造成这种结果；而我们增加的话，也很难去直接确定某个节点的位置最适合多高。
2. 所以我们采取了比较大胆的解法，引入一个随机数，和一个增长概率p。通过两者结合的方式，来决定一个节点的高度。用一份代码来解释的话：

```cpp
srand(time(0)); double p = 0.25; int max_level = 32;
int level = 1;
while(rand() <= RAND_MAX * p && level <= max_level)
{
    level++;
}
return level;
```

上面的p为增长概率，即一个节点从一层开始，有`p*(1-p)`的概率增长到两层，`p^2^*(1-p)`的概率增长到三层，以此类推。不难发现，越高的节点，产生的概率越低。同时我们还有`max_level`的限制，使得层数不可能无限增加。

由此我们也很容易算出，当p为0.5时，每个节点平均的层数是2；当p为0.25是，每个节点平均层数是1.33

>上面的增长概率和最大层数采取的是最新版本的Redis的取值，老版本的Redis采取的最大层数是64

#### 四、跳表的实现

```cpp
struct SkipListNode{
    int val;
    vector<SkipListNode*> nextV;
    SkipListNode(int val, int level)
        : val(val)
        , nextV(level, nullptr)
    {}
};

class Skiplist {
public:
    using Node = SkipListNode;
    Skiplist() {
        srand(time(0));
        _head = new Node(-1, 1);
    }

    int randomLevel(){
        int level = 1;
        while(rand() <= RAND_MAX * _p && level <= _maxLevel){
            level++;
        }
        return level;
    }
    
    //遍历的走法，其实只有两种：一种是往右走，说明跳过了一段不会有结果的区间；另一种是往下走，说明结果有的话，就在这次确定的区间，不会出现在当前区间后面
    bool search(int target) {
        Node* cur = _head;
        int level = _head->nextV.size() - 1;
        while(level >= 0){
            if(cur->nextV[level] && cur->nextV[level]->val < target){
                cur = cur->nextV[level];
            }
            else if(cur->nextV[level] == nullptr || cur->nextV[level]->val > target){
                level--;
            }
            else{
                return true;
            }
        }
        return false;
    }

    //无论插入还是删除，除了完成新节点的构建之外，还需要完成各层之间连接关系的调整，这里所作的就是把新节点所有会链接的节点都跳出来，后面直接根据层级去修改连接关系即可
    vector<Node*> findPrev(int num){
        int n = _head->nextV.size();
        int level = n - 1;
        vector<Node*> prevV(n, _head);
        Node* cur = _head;
        while(level >= 0){
            if(cur->nextV[level] && cur->nextV[level]->val < num){
                cur = cur->nextV[level];
            }
            else if(cur->nextV[level] == nullptr || cur->nextV[level]->val >= num){
                prevV[level] = cur;
                level--;
            }
        }
        return prevV;
    }
    
    void add(int num) {
        vector<Node*> prevV = findPrev(num);
        int level = randomLevel();
        if(_head->nextV.size() < level){
            _head->nextV.resize(level, nullptr);
            prevV.resize(level, _head); //需要注意的是，这里我们只是找出来可能需要改变连接的节点，但是我们的新节点的高度并不一定需要改变所有这些节点的连接（因为有可能比目前最高的节点要矮）
        }

        Node* newNode = new Node(num, level);
        for(int i = 0; i < level; i++){
            newNode->nextV[i] = prevV[i]->nextV[i];
            prevV[i]->nextV[i] = newNode;
        }
    }
    
    bool erase(int num) {
        vector<Node*> prevV = findPrev(num);
        if(prevV[0]->nextV[0] == nullptr || prevV[0]->nextV[0]->val != num){
            return false;
        }
        Node* del = prevV[0]->nextV[0];

        for(int i = 0; i < del->nextV.size(); i++){
            prevV[i]->nextV[i] = del->nextV[i];
        }
        delete del;
        int i = _head->nextV.size() - 1;
        while(i >= 0){
            if(_head->nextV[i] == nullptr){
                i--;
            }
            else{
                break;
            }
        }
        _head->nextV.resize(i + 1, nullptr);
        return true;
    }
private:
    Node* _head;
    double _p = 0.25;
    int _maxLevel = 32;
};
```

#### 五、跳表和平衡搜索树、哈希表的对比

##### 1. 与平衡搜索树对比

* 实现更加简单：跳表和红黑树、AVL树，都能够做到遍历出有序结果，但是跳表的实现，相比红黑树、AVL树简单很多，没有那么多旋转操作
* 暂用的额外空间更少：比如红黑树，每个节点需要保存两个子节点指针、一个父节点指针、一个颜色，而我们的跳表，在增长概率为0.5时，平均2个指针，增长概率为0.25时，平均1.33个指针

##### 2. 与红黑树相比

* 虽然哈希表在数据冲突比较少时，能做到`O(1)`的时间复杂度，但是一旦数据多起来、哈希冲突多起来，就需要借助红黑树来存储，这就回到了第一种对比
* 哈希表的空间利用率相对低。我们有一个平衡因子，一般当占用的节点已经到达70%（即平衡因子为0.7）时，就要扩容。一方面，这意味着有30%以上的空间是不会被使用的；另一方面，扩容也会造成性能损失，因为每个元素都需要重新定址
* 哈希表不支持遍历，而我们的跳表可以采取全部按照最低层遍历的方式完成全部遍历
