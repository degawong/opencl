/*
 * @Author: your name
 * @Date: 2021-08-27 17:40:16
 * @LastEditTime: 2021-08-27 20:18:20
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \arcsoft_algorithm\algorithm\opencl\opencl_test\opencl_test.cpp
 */

#include <vector>
#include <iostream>
#include <gtest/gtest.h>

#include <opencl_helloworld.hpp>

TEST(opencl_algorithm, _hello_world_) {
    auto algorithm = harpocrates::hello_world{};
    algorithm.apply();
    EXPECT_EQ(true, true);
}
