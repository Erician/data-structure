#include <limits.h>
#include "gtest/gtest.h"
#include "rbt/ds_rbt.h"
#include "rbt/ds_rbt_node.h"
#include "node/ds_node.h"
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <vector>

class RbtTest : public ::testing::Test
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

TEST_F(RbtTest, init_ShouldPass)
{
    Rbt rbt;
    Rbt_Operations.init(&rbt);
    ASSERT_EQ(rbt.root_, rbt.nil_);
    ASSERT_EQ(rbt.size_, 0);
}

TEST_F(RbtTest, insert_ShouldPass)
{   
    Rbt rbt;
    Rbt_Operations.init(&rbt);
    int cnt = 0;
    while(cnt<100){
        Node* node = generate_random_node();
        Rbt_Operations.insert(&rbt, node, Node_Operations.cmp);
        cnt++;
    }
    Rbt_Operations.inorder_walk(&rbt, print);
    /*just assert size, find_ShouldPass is in detail*/
    ASSERT_EQ(Rbt_Operations.size(&rbt), cnt);
}

TEST_F(RbtTest, size_ShouldPass)
{   
    Rbt rbt;
    Rbt_Operations.init(&rbt);
    int cnt = 0;
    while(cnt<100){
        ASSERT_EQ(Rbt_Operations.size(&rbt), cnt);
        Rbt_Operations.insert(&rbt, generate_random_node(), Node_Operations.cmp);
        cnt++;
    }
}

TEST_F(RbtTest, inorder_walk_ShouldPass)
{   
    Rbt rbt;
    Rbt_Operations.init(&rbt);
    int cnt = 0;
    while(cnt<100){
        Rbt_Operations.insert(&rbt, generate_random_node(), Node_Operations.cmp);
        cnt++;
    }   
    v.clear();
    Rbt_Operations.inorder_walk(&rbt, save_in_array);
    for(int i=1; i< cnt; i++){
        ASSERT_LE(v[i-1], v[i]);
    }
}

TEST_F(RbtTest, find_ShouldPass)
{   
    Rbt rbt;
    Rbt_Operations.init(&rbt);
    int cnt = 0;
    long long int save[100];
    while(cnt<100){
        Node* node = generate_random_node();
        save[cnt] = ((Node*)node)->key_;
        Rbt_Operations.insert(&rbt, node , Node_Operations.cmp);
        cnt++;
    }   
    for(int i=0; i<cnt; i++){
        Node* node = (Node*)Rbt_Operations.find(&rbt, Node_Operations.constructor(save[i]), Node_Operations.cmp);
        ASSERT_NE(node, (void*)NULL);
        ASSERT_EQ(node->key_, save[i]);
    }
}

TEST_F(RbtTest, find_NotExist_ShouldPass)
{   
    Rbt rbt;
    Rbt_Operations.init(&rbt);
    int cnt = 0;
    while(cnt<100){
        Node* node = generate_random_node();
        Rbt_Operations.insert(&rbt, node , Node_Operations.cmp);
        cnt++;
    }   
    ASSERT_EQ(Rbt_Operations.find(&rbt, Node_Operations.constructor(-1), Node_Operations.cmp), (void*)NULL);
}

TEST_F(RbtTest, maximum_ShouldPass)
{   
    Rbt rbt;
    Rbt_Operations.init(&rbt);
    int cnt = 0;
    long long int max = -1;
    while(cnt<100){
        Node* node = generate_random_node();
        if(((Node*)node)->key_ > max)
            max = ((Node*)node)->key_;
        Rbt_Operations.insert(&rbt, node , Node_Operations.cmp);
        cnt++;
    }   
    ASSERT_EQ(((Node*)Rbt_Operations.maximum(&rbt))->key_, max);
}

TEST_F(RbtTest, minimum_ShouldPass)
{   
    Rbt rbt;
    Rbt_Operations.init(&rbt);
    int cnt = 0;
    long long int min = 10000000000;
    while(cnt<100){
        Node* node = generate_random_node();
        if(((Node*)node)->key_ < min)
            min = ((Node*)node)->key_;
        Rbt_Operations.insert(&rbt, node , Node_Operations.cmp);
        cnt++;
    }   
    ASSERT_EQ(((Node*)Rbt_Operations.minimum(&rbt))->key_, min);
}

TEST_F(RbtTest, successor_ShouldPass)
{
    Rbt rbt;
    Rbt_Operations.init(&rbt);
    int cnt = 0;
    long long int backup = 10000;
    while(cnt<1000){
        Node* node = generate_random_node();
        if(Rbt_Operations.find(&rbt, node, Node_Operations.cmp))
            node->key_ = backup++;
        Rbt_Operations.insert(&rbt, node , Node_Operations.cmp);
        cnt++;
    }   
    v.clear();
    Rbt_Operations.inorder_walk(&rbt, save_in_array);
    for(int i=0; i<cnt-1; i++){
        Node* successor = (Node*)Rbt_Operations.successor(&rbt, Node_Operations.constructor(v[i]), \
                                 Node_Operations.cmp);
        if(i+1 == cnt)
            ASSERT_EQ(successor, (void*)NULL);
        else if(successor != NULL){
            ASSERT_EQ(successor->key_, v[i+1]);
        }
    }
}

TEST_F(RbtTest, predecessor_ShouldPass)
{
    Rbt rbt;
    Rbt_Operations.init(&rbt);
    int cnt = 0;
    long long int backup = 10000;
    while(cnt<1000){
        Node* node = generate_random_node();
        if(Rbt_Operations.find(&rbt, node, Node_Operations.cmp))
            node->key_ = backup++;
        Rbt_Operations.insert(&rbt, node , Node_Operations.cmp);
        cnt++;
    }   
    v.clear();
    Rbt_Operations.inorder_walk(&rbt, save_in_array);
    for(int i=cnt-1; i<=0; i--){
        Node* successor = (Node*)Rbt_Operations.successor(&rbt, Node_Operations.constructor(v[i]), \
                                 Node_Operations.cmp);
        if(i == 0)
            ASSERT_EQ(successor, (void*)NULL);
        else if(successor != NULL){
            ASSERT_EQ(successor->key_, v[i-1]);
        }
    }
}

TEST_F(RbtTest, erase_ShouldPass)
{
    Rbt rbt;
    Rbt_Operations.init(&rbt);
    int cnt = 0;
    long long int backup = 10000;
    while(cnt<1){
        Node* node = generate_random_node();
        if(Rbt_Operations.find(&rbt, node, Node_Operations.cmp))
            node->key_ = backup++;
        Rbt_Operations.insert(&rbt, node , Node_Operations.cmp);
        cnt++;
    }   
    v.clear();
    Rbt_Operations.inorder_walk(&rbt, save_in_array);
    for(int i=cnt-1; i>=0; i--){
        int size = Rbt_Operations.size(&rbt);
        ASSERT_NE(Rbt_Operations.find(&rbt, Node_Operations.constructor(v[i]), Node_Operations.cmp), (void*)NULL);
        Rbt_Operations.erase(&rbt, Node_Operations.constructor(v[i]), Node_Operations.cmp, Node_Operations.destructor);
        ASSERT_EQ(Rbt_Operations.find(&rbt, Node_Operations.constructor(v[i]), Node_Operations.cmp), (void*)NULL);
        ASSERT_EQ(Rbt_Operations.size(&rbt), size-1);
    }
    ASSERT_EQ(Rbt_Operations.size(&rbt), 0);
}

TEST_F(RbtTest, inorder_next_ShouldPass)
{
    Rbt rbt;
    Rbt_Operations.init(&rbt);
    int cnt = 0;
    long long int backup = 10000;
    while(cnt<1000){
        Node* node = generate_random_node();
        if(Rbt_Operations.find(&rbt, node, Node_Operations.cmp))
            node->key_ = backup++;
        Rbt_Operations.insert(&rbt, node , Node_Operations.cmp);
        cnt++;
    }   
    v.clear();
    Rbt_Operations.inorder_walk(&rbt, save_in_array);
    cnt = 0;
    Node* node = (Node*)Rbt_Operations.inorder_next(&rbt, 1);
    while(node != NULL){
        ASSERT_EQ(node->key_, v[cnt++]);
        node = (Node*)Rbt_Operations.inorder_next(&rbt, 0);
    }
}























