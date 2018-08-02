#include <limits.h>
#include "gtest/gtest.h"
#include "bst/ds_bst.h"
#include "bst/ds_bst_node.h"
#include "node/ds_node.h"
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <vector>

class BstTest : public ::testing::Test
{
protected:
    virtual void SetUp() {
        srand(time(NULL));
    }
    virtual void TearDown() {
        ;
    }

public:
    Node* generate_random_node(){
        Node *node = (Node*)malloc(sizeof(Node));
        node -> key_ = rand()%1000;
        return node;
    }
};

std::vector<long long int> v;
void save_in_array(void* node){
    v.push_back(((Node*)node)->key_);
}
void print(void* node){
    printf("%lld ", ((Node*)node)->key_);
}

TEST_F(BstTest, init_ShouldPass)
{
    Bst bst;
    Bst_Operations.init(&bst);
    ASSERT_EQ(bst.root_, (void*)NULL);
}

TEST_F(BstTest, insert_ShouldPass)
{   
    Bst bst;
    Bst_Operations.init(&bst);
    int cnt = 0;
    while(cnt<100){
        Bst_Operations.insert(&bst, generate_random_node(), Node_Operations.cmp);
        cnt++;
    }
    /*just assert size, find_ShouldPass is in detail*/
    ASSERT_EQ(Bst_Operations.size(&bst), cnt);
}

TEST_F(BstTest, size_ShouldPass)
{   
    Bst bst;
    Bst_Operations.init(&bst);
    int cnt = 0;
    while(cnt<100){
        ASSERT_EQ(Bst_Operations.size(&bst), cnt);
        Bst_Operations.insert(&bst, generate_random_node(), Node_Operations.cmp);
        cnt++;
    }
}

TEST_F(BstTest, inorder_walk_ShouldPass)
{   
    Bst bst;
    Bst_Operations.init(&bst);
    int cnt = 0;
    while(cnt<100){
        Bst_Operations.insert(&bst, generate_random_node(), Node_Operations.cmp);
        cnt++;
    }   
    v.clear();
    Bst_Operations.inorder_walk(&bst, save_in_array);
    for(int i=1; i< cnt; i++){
        ASSERT_LE(v[i-1], v[i]);
    }
}

TEST_F(BstTest, find_ShouldPass)
{   
    Bst bst;
    Bst_Operations.init(&bst);
    int cnt = 0;
    long long int save[100];
    while(cnt<100){
        Node* node = generate_random_node();
        save[cnt] = ((Node*)node)->key_;
        Bst_Operations.insert(&bst, node , Node_Operations.cmp);
        cnt++;
    }   
    for(int i=0; i<cnt; i++){
        Node* node = (Node*)Bst_Operations.find(&bst, Node_Operations.constructor(save[i]), Node_Operations.cmp);
        ASSERT_NE(node, (void*)NULL);
        ASSERT_EQ(node->key_, save[i]);
    }
}

TEST_F(BstTest, find_NotExist_ShouldPass)
{   
    Bst bst;
    Bst_Operations.init(&bst);
    int cnt = 0;
    while(cnt<100){
        Node* node = generate_random_node();
        Bst_Operations.insert(&bst, node , Node_Operations.cmp);
        cnt++;
    }   
    ASSERT_EQ(Bst_Operations.find(&bst, Node_Operations.constructor(-1), Node_Operations.cmp), (void*)NULL);
}

TEST_F(BstTest, maximum_ShouldPass)
{   
    Bst bst;
    Bst_Operations.init(&bst);
    int cnt = 0;
    long long int max = -1;
    while(cnt<100){
        Node* node = generate_random_node();
        if(((Node*)node)->key_ > max)
            max = ((Node*)node)->key_;
        Bst_Operations.insert(&bst, node , Node_Operations.cmp);
        cnt++;
    }   
    ASSERT_EQ(((Node*)Bst_Operations.maximum(&bst))->key_, max);
}

TEST_F(BstTest, minimum_ShouldPass)
{   
    Bst bst;
    Bst_Operations.init(&bst);
    int cnt = 0;
    long long int min = 10000000000;
    while(cnt<100){
        Node* node = generate_random_node();
        if(((Node*)node)->key_ < min)
            min = ((Node*)node)->key_;
        Bst_Operations.insert(&bst, node , Node_Operations.cmp);
        cnt++;
    }   
    ASSERT_EQ(((Node*)Bst_Operations.minimum(&bst))->key_, min);
}

TEST_F(BstTest, successor_ShouldPass)
{
    Bst bst;
    Bst_Operations.init(&bst);
    int cnt = 0;
    long long int backup = 10000;
    while(cnt<1000){
        Node* node = generate_random_node();
        if(Bst_Operations.find(&bst, node, Node_Operations.cmp))
            node->key_ = backup++;
        Bst_Operations.insert(&bst, node , Node_Operations.cmp);
        cnt++;
    }   
    v.clear();
    Bst_Operations.inorder_walk(&bst, save_in_array);
    for(int i=0; i<cnt-1; i++){
        Node* successor = (Node*)Bst_Operations.successor(&bst, Node_Operations.constructor(v[i]), \
                                 Node_Operations.cmp);
        if(i+1 == cnt)
            ASSERT_EQ(successor, (void*)NULL);
        else if(successor != NULL){
            ASSERT_EQ(successor->key_, v[i+1]);
        }
    }
}

TEST_F(BstTest, predecessor_ShouldPass)
{
    Bst bst;
    Bst_Operations.init(&bst);
    int cnt = 0;
    long long int backup = 10000;
    while(cnt<1000){
        Node* node = generate_random_node();
        if(Bst_Operations.find(&bst, node, Node_Operations.cmp))
            node->key_ = backup++;
        Bst_Operations.insert(&bst, node , Node_Operations.cmp);
        cnt++;
    }   
    v.clear();
    Bst_Operations.inorder_walk(&bst, save_in_array);
    for(int i=cnt-1; i<=0; i--){
        Node* successor = (Node*)Bst_Operations.successor(&bst, Node_Operations.constructor(v[i]), \
                                 Node_Operations.cmp);
        if(i == 0)
            ASSERT_EQ(successor, (void*)NULL);
        else if(successor != NULL){
            ASSERT_EQ(successor->key_, v[i-1]);
        }
    }
}

TEST_F(BstTest, erase_ShouldPass)
{
    Bst bst;
    Bst_Operations.init(&bst);
    int cnt = 0;
    long long int backup = 10000;
    while(cnt<1){
        Node* node = generate_random_node();
        if(Bst_Operations.find(&bst, node, Node_Operations.cmp))
            node->key_ = backup++;
        Bst_Operations.insert(&bst, node , Node_Operations.cmp);
        cnt++;
    }   
    v.clear();
    Bst_Operations.inorder_walk(&bst, save_in_array);
    for(int i=cnt-1; i>=0; i--){
        int size = Bst_Operations.size(&bst);
        ASSERT_NE(Bst_Operations.find(&bst, Node_Operations.constructor(v[i]), Node_Operations.cmp), (void*)NULL);
        Bst_Operations.erase(&bst, Node_Operations.constructor(v[i]), Node_Operations.cmp, Node_Operations.destructor);
        ASSERT_EQ(Bst_Operations.find(&bst, Node_Operations.constructor(v[i]), Node_Operations.cmp), (void*)NULL);
        ASSERT_EQ(Bst_Operations.size(&bst), size-1);
    }
    ASSERT_EQ(Bst_Operations.size(&bst), 0);
}

TEST_F(BstTest, inorder_next_ShouldPass)
{
    Bst bst;
    Bst_Operations.init(&bst);
    int cnt = 0;
    long long int backup = 10000;
    while(cnt<1000){
        Node* node = generate_random_node();
        if(Bst_Operations.find(&bst, node, Node_Operations.cmp))
            node->key_ = backup++;
        Bst_Operations.insert(&bst, node , Node_Operations.cmp);
        cnt++;
    }   
    v.clear();
    Bst_Operations.inorder_walk(&bst, save_in_array);
    cnt = 0;
    Node* node = (Node*)Bst_Operations.inorder_next(&bst, 1);
    while(node != NULL){
        ASSERT_EQ(node->key_, v[cnt++]);
        node = (Node*)Bst_Operations.inorder_next(&bst, 0);
    }
}























