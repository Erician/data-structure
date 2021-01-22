#include <limits.h>
#include "gtest/gtest.h"
#include "bpt/ds_bpt.h"
#include "bpt/ds_bpt_node.h"
#include "node/ds_node.h"
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <vector>

class BptTest : public ::testing::Test
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

TEST_F(BptTest, init_ShouldPass)
{
    Bpt bpt;
    Bpt_Operations.init(&bpt);
    ASSERT_EQ(bpt.root_, (void*)NULL);
}

TEST_F(BptTest, insert_ShouldPass)
{
    Bpt bpt;
    Bpt_Operations.init(&bpt);
    int cnt = 0;
    while(cnt<100){
        Bpt_Operations.insert(&bpt, generate_random_node(), Node_Operations.cmp);
        cnt++;
    }
    /*just assert size, find_ShouldPass is in detail*/
    ASSERT_EQ(Bpt_Operations.size(&bpt), cnt);
}

TEST_F(BptTest, size_ShouldPass)
{   
    Bpt bpt;
    Bpt_Operations.init(&bpt);
    int cnt = 0;
    while(cnt<100){
        ASSERT_EQ(Bpt_Operations.size(&bpt), cnt);
        Bpt_Operations.insert(&bpt, generate_random_node(), Node_Operations.cmp);
        cnt++;
    }
}

TEST_F(BptTest, find_ShouldPass)
{   
    Bpt bpt;
    Bpt_Operations.init(&bpt);
    int cnt = 0;
    long long int save[100];
    while(cnt<100){
        Node* node = generate_random_node();
        save[cnt] = ((Node*)node)->key_;
        Bpt_Operations.insert(&bpt, node , Node_Operations.cmp);
        cnt++;
    }   
    for(int i=0; i<cnt; i++){
        Node* node = (Node*)Bpt_Operations.find(&bpt, Node_Operations.constructor(save[i]), Node_Operations.cmp);
        ASSERT_NE(node, (void*)NULL);
        ASSERT_EQ(node->key_, save[i]);
    }
}

TEST_F(BptTest, find_NotExist_ShouldPass)
{   
    Bpt bpt;
    Bpt_Operations.init(&bpt);
    int cnt = 0;
    while(cnt<100){
        Node* node = generate_random_node();
        Bpt_Operations.insert(&bpt, node , Node_Operations.cmp);
        cnt++;
    }   
    ASSERT_EQ(Bpt_Operations.find(&bpt, Node_Operations.constructor(-1), Node_Operations.cmp), (void*)NULL);
}

TEST_F(BptTest, erase_ShouldPass)
{
    Bpt bpt;
    Bpt_Operations.init(&bpt);
    int cnt = 0;
    long long int backup = 10000;
    v.clear();

    while(cnt<4) {
        Node* node = generate_random_node();
        while(Bpt_Operations.size(&bpt) && Bpt_Operations.find(&bpt, node, Node_Operations.cmp))
            node->key_ = backup++;
        Bpt_Operations.insert(&bpt, node , Node_Operations.cmp);
        save_in_array(node);
        cnt++;
    }
    for(int i=cnt-1; i>=0; i--){
        ASSERT_EQ(Bpt_Operations.size(&bpt), i+1);
        ASSERT_NE(Bpt_Operations.find(&bpt, Node_Operations.constructor(v[i]), Node_Operations.cmp), (void*)NULL);
        Bpt_Operations.erase(&bpt, Node_Operations.constructor(v[i]), Node_Operations.cmp, Node_Operations.destructor);
        if (i != 0) {
            ASSERT_EQ(Bpt_Operations.find(&bpt, Node_Operations.constructor(v[i]), Node_Operations.cmp), (void*)NULL);
        }
        ASSERT_EQ(Bpt_Operations.size(&bpt), i);
    }
    ASSERT_EQ(Bpt_Operations.size(&bpt), 0);
}