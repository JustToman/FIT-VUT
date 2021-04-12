//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

class NonEmptyTree : public ::testing::Test
{
protected: 
    
    BinaryTree tree;
    virtual void SetUp() 
    {
        int values[] = {-5, 10, 5, 100, 25, 45, 55, -15, 30, 40, -25, 75};
        std::pair<bool,BinaryTree::Node_t*> insert_out[12];

        for(int i = 0; i < 12; i++)
        {
            insert_out[i] = tree.InsertNode(values[i]);
        }
    }
};
class EmptyTree : public ::testing::Test
{
protected: 
    
    BinaryTree tree;
};
class TreeAxioms : public ::testing::Test
{
protected: 
    virtual void SetUp() 
    {
        int values[] = {-5, 10, 5, 100, 25, 45, 55, -15, 30, 40, -25, 75};
        std::pair<bool,BinaryTree::Node_t*> insert_out[12];

        for(int i = 0; i < 12; i++)
        {
            insert_out[i] = tree.InsertNode(values[i]);
        }
    }
    std::vector<BinaryTree::Node_t*>outLeafNodes;
    BinaryTree tree;
};

TEST_F(EmptyTree, InsertNode)
{
    std::pair<bool,BinaryTree::Node_t*> x, y;
    EXPECT_EQ(tree.GetRoot(), nullptr);
    x = tree.InsertNode(5);
    EXPECT_TRUE(x.first);
    EXPECT_EQ(x.second,tree.GetRoot());

    y = tree.InsertNode(-5);
    EXPECT_TRUE(x.first);
    EXPECT_EQ(x.second,tree.GetRoot());
}
TEST_F(EmptyTree, DeleteNode)
{
    Node_t *root = tree.GetRoot();
    EXPECT_FALSE(tree.DeleteNode(5));
    EXPECT_EQ(root, nullptr);
}
TEST_F(EmptyTree, FindNode)
{
 Node_t *node;
 EXPECT_EQ(tree.GetRoot(), nullptr);
 EXPECT_EQ(node = tree.FindNode(5),nullptr);
 EXPECT_EQ(node = tree.FindNode(-2),nullptr);
}

TEST_F(NonEmptyTree, InsertNode)
{
  std::pair<bool,BinaryTree::Node_t*> x;
  x = tree.InsertNode(-5);
  EXPECT_FALSE(x.first);  
  EXPECT_TRUE(x.second = tree.GetRoot());

  x = tree.InsertNode(250);
  EXPECT_TRUE(x.first);
  EXPECT_TRUE(x.second != nullptr);
}

TEST_F(NonEmptyTree, FindNode)
{
    int values_test[] = {-5, 10, 5, 100, 25, 45, 55, -15, 30, 40, -25, 75};
    Node_t *node;
    for(int i = 0; i < 12; i++)
    {
        EXPECT_TRUE((node = tree.FindNode(values_test[i])) != nullptr);
        EXPECT_EQ(node->key,values_test[i]);
    }
    EXPECT_TRUE((node = tree.FindNode(-1)) == nullptr);
}

TEST_F(NonEmptyTree, DeleteNode)
{
  int values_test[] = {-5, 10, 5, 100, 25, 45, 55, -15, 30, 40, -25, 75};
  for(int i = 0; i < 12; i++)
  {
      EXPECT_TRUE(tree.DeleteNode(values_test[i]));
  }
}

TEST_F(TreeAxioms, Axiom1)
{
  tree.GetLeafNodes(outLeafNodes);
  for(int i = 0; i < outLeafNodes.size(); i++)
  {
      EXPECT_EQ(outLeafNodes[i]->color, BLACK);
  }
}


TEST_F(TreeAxioms, Axiom2)
{
    tree.GetAllNodes(outLeafNodes);
    for(int i = 0; i < outLeafNodes.size(); i++)
    {
        if(outLeafNodes[i]->color == RED)
        {
            EXPECT_TRUE(outLeafNodes[i]->pLeft->color == BLACK);
            EXPECT_TRUE(outLeafNodes[i]->pRight->color == BLACK);
        }
    }
}

TEST_F(TreeAxioms, Axiom3)
{
    tree.GetLeafNodes(outLeafNodes);
    int n_of_black[outLeafNodes.size()];
    for(int i=0; i < outLeafNodes.size(); i++)
    {
        n_of_black[i] = 0;
        for(Node_t *tmp = outLeafNodes[i]; tmp != tree.GetRoot(); tmp = tmp->pParent)
        {
            if(tmp->color== BLACK)
            {
              n_of_black[i]++;
            }
        }
    }
    for(int i = 0; i < outLeafNodes.size(); i++)
    {
        EXPECT_TRUE(n_of_black[0] == n_of_black[i]);
    }


}

/*** Konec souboru black_box_tests.cpp ***/