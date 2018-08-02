#include <limits.h>
#include "gtest/gtest.h"
#include "cdll/ds_cdll.h"
#include "node/ds_node.h"
#include <time.h>
#include <string.h>
#include <malloc.h>

class CdllTest : public ::testing::Test
{
protected:
    virtual void SetUp() {
         srand(time(NULL));;
    }
    virtual void TearDown() {
        ;
    }
public:
    CdllNode* generate_random_cdll_node(){
        Node *node = (Node*)malloc(sizeof(Node));
        node->key_ = rand()%1000;

        CdllNode *cdll_node = (CdllNode*)malloc(sizeof(CdllNode));
        cdll_node->node_ = node;
        cdll_node->prev_ = NULL;
        cdll_node->next_ = NULL;
        return cdll_node;
    }
};

TEST_F(CdllTest, init_ShouldPass)
{
    Cdll cdll;
    Cdll_Operations.init(&cdll);
    ASSERT_NE(cdll.nil_, (void*)NULL);
    ASSERT_EQ(cdll.ptr_, cdll.nil_);
    ASSERT_EQ(cdll.size_, 0);
}

TEST_F(CdllTest, push_back_ShouldPass)
{   
    Cdll cdll;
    Cdll_Operations.init(&cdll);
    int cnt = 0;
    while(cnt<100){
        Cdll_Operations.push_back(&cdll, generate_random_cdll_node());
        cnt++;
    }
    ASSERT_EQ(Cdll_Operations.size(&cdll), cnt);
}

TEST_F(CdllTest, next_ShouldPass)
{
    Cdll cdll;
    Cdll_Operations.init(&cdll);
    long long int save[100];

    int cnt = 0;
    while(cnt<100){
        CdllNode *cdll_node = generate_random_cdll_node();
        save[cnt] = ((Node*)cdll_node -> node_) -> key_;
        Cdll_Operations.push_back(&cdll, cdll_node);
        cnt++;
    }
    ASSERT_EQ(Cdll_Operations.size(&cdll), cnt);

    cnt = 0;
    Node* node = (Node*)Cdll_Operations.next(&cdll, 1);
    while(node != NULL){
        ASSERT_EQ(node->key_, save[cnt]);
        cnt++;
        node = (Node*)Cdll_Operations.next(&cdll, 0);
    }
}



