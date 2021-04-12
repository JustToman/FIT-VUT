//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Roman Popelka
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

class Matrix_no_param : public ::testing::Test
{
    protected:
        Matrix matrixx;
        std::vector<std::vector<double>> array;
};
class Matrix_param : public ::testing::Test
{
    protected:
        Matrix matrixx1 = Matrix(5, 4);
        Matrix matrixx2 = Matrix(3, 3);
        std::vector<std::vector<double>> array;
};

TEST_F(Matrix_no_param,set_exact_value)
{
    EXPECT_FALSE(matrixx.set(1,1,12.0));
    EXPECT_FALSE(matrixx.set(-5,-3,12.0));
    EXPECT_TRUE(matrixx.set(0,0,12.0));
}

TEST_F(Matrix_no_param,set_all_values)
{
   array = std::vector<std::vector< double > >(1, std::vector<double>(1, 0));
   array[0][0] = 5.0;
   EXPECT_TRUE(matrixx.set(array)); 
   EXPECT_EQ(matrixx.get(0,0),5.0);
   array = std::vector<std::vector< double > >(2, std::vector<double>(2, 0));
   array = {{2,3},{4,5}};
   EXPECT_FALSE(matrixx.set(array)); 
}

TEST_F(Matrix_no_param, get_value)
{
  array = std::vector<std::vector< double > >(1, std::vector<double>(1, 0));
  array[0][0] = -1.0;
  matrixx.set(array);
  EXPECT_EQ(matrixx.get(0,0),-1.0);
  EXPECT_ANY_THROW(matrixx.get(-1,2));
}

TEST_F(Matrix_no_param, matrix_equality)
{
   Matrix m;
   EXPECT_TRUE(matrixx.operator==(m));
   m.set(0,0,1.0);
   EXPECT_FALSE(matrixx.operator==(m));
   Matrix m2(1,2);
   EXPECT_ANY_THROW(matrixx.operator==(m2)); 
}

TEST_F(Matrix_no_param, sum_of_matrix)
{
    Matrix m;
    m.set(0,0,134.0);
    Matrix sum = matrixx.operator+(m);
    EXPECT_EQ(sum.get(0,0),134.0);
    Matrix m2(2,3);
    EXPECT_ANY_THROW(matrixx.operator+(m2));    
}

TEST_F(Matrix_no_param, matrix_multiplication)
{
    Matrix m;
    Matrix multiplication;
    for(int i = 0, j = 0; i < 10; i++, j--)
    {
    matrixx.set(0,0,j*1.0);
    m.set(0,0, i*1.0);
    multiplication = matrixx.operator*(m);
    EXPECT_EQ(multiplication.get(0,0), (double)(j*i));
    }
}
TEST_F(Matrix_no_param, scalar_multiplication)
{
    matrixx.set(0,0,12);
    matrixx = matrixx.operator*(0.5);
    EXPECT_EQ(matrixx.get(0,0),6.0);
}
TEST_F(Matrix_no_param, equation_solve)
{
    std::vector<double> b = std::vector<double>(1,0);
    std::vector<double> result = std::vector<double>(1,0);
    b[0] = 5.0;
    matrixx.set(0,0,5.0);
    result = matrixx.solveEquation(b);
    EXPECT_EQ(result[0],1.0);
    b = std::vector<double>(2,0);
    b = {2.0, 3.0};
    EXPECT_ANY_THROW(result = matrixx.solveEquation(b));
}
TEST_F(Matrix_param, constructor)
{
    EXPECT_ANY_THROW(Matrix m = Matrix(-1,2));
    EXPECT_NO_THROW(Matrix m = Matrix(1,5));
}
TEST_F(Matrix_param, constructor_fill)
{
    for(int i = 0; i < 5; i ++)
    {
        for(int j = 0; j < 4; j++)
        {
            EXPECT_EQ(matrixx1.get(i,j),0);
        }
    }
}
TEST_F(Matrix_param, set_exact_value)
{
    EXPECT_TRUE(matrixx1.set(4,3,12.5));
    EXPECT_EQ(matrixx1.get(4,3),12.5);

    EXPECT_FALSE(matrixx1.set(4,5,15.5));
    EXPECT_FALSE(matrixx1.set(-2,-3,12));
}
TEST_F(Matrix_param, set_all_values)
{
    double values[] = {5, 10, 20, 30, 40, 50, 60, 90, 100, -5, -10, -20, -30, - 40, - 50, -60, - 70, -80, -90, -100};
    array = std::vector<std::vector< double > >(5, std::vector<double>(3,0));
    EXPECT_FALSE(matrixx1.set(array));
    array = std::vector<std::vector< double > >(5, std::vector<double>(4, 0));
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            array[i][j] = values[i*4 + j];
        }
    }
    EXPECT_TRUE(matrixx1.set(array));
}
TEST_F(Matrix_param, get_value)
{
    array = std::vector<std::vector< double > >(5, std::vector<double>(4, 0));
    double values[] = {5, 10, 20, 30, 40, 50, 60, 90, 100, -5, -10, -20, -30, - 40, - 50, -60, - 70, -80, -90, -100};
      for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            array[i][j] = values[i * 4 + j];
            matrixx1.set(i,j,values[i * 4 + j]);
            EXPECT_EQ(matrixx1.get(i,j),values[i*4 + j]);
        }
    }
}
TEST_F(Matrix_param,matrix_equality)
{
    EXPECT_ANY_THROW(matrixx1.operator==(matrixx2));
    
    Matrix m = Matrix(3,3);
    EXPECT_TRUE(matrixx2.operator==(m));

    m.set(2,2,15.0);
    EXPECT_FALSE(matrixx2.operator==(m));
}

TEST_F(Matrix_param, sum_of_matrix)
{
    double values[] = {5,10,15,20,30,-12,-9,0,13};
    Matrix m = Matrix(3,3);
    array = std::vector<std::vector< double > >(3, std::vector<double>(3, 0));
    Matrix sum;
    
    EXPECT_NO_THROW(sum = matrixx2.operator+(m));
    for(int i = 0;i < 3;i++)
    {
        for(int j = 0; j < 3; j++)
        {
            EXPECT_EQ(sum.get(i,j),0.0);
        }
    }
    
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            m.set(i,j,values[i*3 + j]);
            matrixx2.set(i,j,values[i*3 + j]);
        }
    }
    EXPECT_NO_THROW(sum = matrixx2.operator+(m));
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 0; j++)
        {
            EXPECT_EQ(sum.get(i,j),values[i*3 +j] * 2);
        }
    }


    EXPECT_ANY_THROW(matrixx1.operator==(matrixx2));
}

TEST_F(Matrix_param, matrix_multiplication)
{
    double values1[] = {5,10,15,20,30,-12,-9,0,13};
    double values2[] = {5, 10, 20, 30, 40, 50, 60, 90, 100, -5, -10, -20};
    double result_val[] = {1925, 475, 550, 750, 100, 1760, 2320, 3540, 1255, -155, -310, -530};
    EXPECT_ANY_THROW(Matrix m = matrixx1.operator*(matrixx2));
    Matrix m = Matrix(3,4);
    Matrix result;

    EXPECT_NO_THROW(result = matrixx2.operator*(m));
    for(int i = 0;i < 3;i++)
    {
        for(int j = 0; j < 3; j++)
        {
            EXPECT_EQ(result.get(i,j),0.0);
        }
    }
    
    for(int i=0; i < 3; i++)
    {
        for(int j =0; j < 3; j++)
        {
            matrixx2.set(i,j,values1[i*3 + j]);
        }
    }
    for(int i=0; i < 3; i++)
    {
        for(int j =0; j < 4; j++)
        {
            m.set(i,j,values2[i*4 + j]);
        }
    }
    EXPECT_NO_THROW(result = matrixx2.operator*(m));
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            EXPECT_EQ(result.get(i,j),result_val[i*4 +j]);
        }
    }
}
TEST_F(Matrix_param, scalar_multiplication)
{
    double values[] = {5,10,15,20,30,-12,-9,0,13};
    
    for(int i=0; i < 3; i++)
    {
      for(int j =0; j < 3; j++)
      {
        matrixx2.set(i,j,values[i*3 + j]);
      }
    }
    matrixx2 = matrixx2.operator*(2.5);
    
    for(int i=0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
           EXPECT_EQ(matrixx2.get(i,j),values[i*3 + j] * 2.5);
        }
    }
}
TEST_F(Matrix_param, equation_solve)
{
    double values[] = {5,10,15,20,30,-12,-9,0,13};
    double values2[] = {5, 10, 20, 30, 40, 50, 60, 90, 100, -5, -10, -20, -30, - 40, - 50, -60, - 70, -80, -90, -100};
    std::vector<double> b = std::vector<double>(1,0);
    std::vector<double> result = std::vector<double>(1,0);
    b = {1};
    EXPECT_ANY_THROW(result = matrixx1.solveEquation(b));
    
    b = std::vector<double>(5,0);
    b = {1,2,3,4,5};
    for(int i = 0; i < 5; i++)
    {
      for(int j = 0; j < 4; j++)
      {
            matrixx1.set(i,j,values2[i * 4 + j]);
      }
    }
    EXPECT_ANY_THROW(result = matrixx1.solveEquation(b));


    b = std::vector<double>(3,0);
    b = {1,2,3};
    EXPECT_ANY_THROW(result = matrixx2.solveEquation(b));
    
    double results_expected[] = {-79/224.0,83/280.0,-3/224.0};
    for(int i=0; i < 3; i++)
    {
      for(int j =0; j < 3; j++)
      {
        matrixx2.set(i,j,values[i*3 + j]);
      }
    }
     EXPECT_NO_THROW(result = matrixx2.solveEquation(b));
     for(int i = 0; i < 3; i++)
     {
         EXPECT_EQ(result[i],results_expected[i]);
     }
}
//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

/*** Konec souboru white_box_tests.cpp ***/
