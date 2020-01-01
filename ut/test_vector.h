#ifndef _TEST_VECTOR_H
#define _TEST_VECTOR_H

#include "../include/zrainy_vector.h"
#include "gtest/gtest.h"

TEST(TestVector, FirstTest){
	ZRainySTL::zvector<int> zv;
	zv.push_back(1);
	EXPECT_EQ(1, zv.size());

	zv.push_back(2);
	EXPECT_EQ(2, zv.size());
}

#endif // end ifndef _TEST_VECTOR_H
