// 2023/05/10

#include "parser/parser.hpp"
#include "gtest/gtest.h"

int main(int argc, char *argv[]) {
  EXPECT_EQ(parser::HelloWorld(), "hello, world");
}
