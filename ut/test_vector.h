#ifndef _TEST_VECTOR_H
#define _TEST_VECTOR_H

#include "../include/zrainy_vector.h"
#include "gtest/gtest.h"

TEST(TestVector, TestMemoryManager){
	ZRainySTL::zvector<int> zv(2, 9);
	EXPECT_EQ(2, zv.size());
	EXPECT_EQ(2, zv.capacity());

	zv.push_back(1);
	EXPECT_EQ(3, zv.size());
	EXPECT_EQ(4, zv.capacity());

	zv.push_back(2);
	EXPECT_EQ(4, zv.size());
	EXPECT_EQ(4, zv.capacity());

	zv.push_back(3);
	EXPECT_EQ(5, zv.size());
	EXPECT_EQ(8, zv.capacity());

	EXPECT_EQ(9, zv[0]);

	zv.pop_back();
	EXPECT_EQ(4, zv.size());
	EXPECT_EQ(8, zv.capacity());
}

#endif // end ifndef _TEST_VECTOR_H
