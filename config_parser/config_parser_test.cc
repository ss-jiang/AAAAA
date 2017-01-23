#include <sstream>
#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "config_parser.h"

// Simple working example
TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigTest, ToString) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");
	EXPECT_EQ(statement.ToString(0), "foo bar;\n");
}

// Fixture to share state between tests
class NginxStringConfigTest : public ::testing::Test {
    protected:
    	// shared method that both TEST_F methods use
        bool ParseString(const std::string config_string) {
    		std::stringstream config_stream(config_string);
    		return parser_.Parse(&config_stream, &out_config_);
    	}
    	NginxConfigParser parser_;
    	NginxConfig out_config_;
};

// Tests contents of vector after parsing simple string
TEST_F(NginxStringConfigTest, AnotherSimpleConfig) {
	EXPECT_TRUE(ParseString("foo bar;"));
	EXPECT_EQ(1, out_config_.statements_.size());
	EXPECT_EQ("foo", out_config_.statements_[0]->tokens_[0]);
}

// Tests an invalid example config
TEST_F(NginxStringConfigTest, InvalidConfig) {
	EXPECT_FALSE(ParseString("foo bar"));
}

// Tests a properly balanced statement
TEST_F(NginxStringConfigTest, BalancedConfig) {
	EXPECT_TRUE(ParseString("server { listen 80; }"));
    EXPECT_EQ(1, out_config_.statements_.size());
}


// Tests config with missing matched bracket
// Test case used to fail since ParseString incorrectly 
// returned True on the config with imbalanced {}
TEST_F(NginxStringConfigTest, ImbalancedConfig) {
    EXPECT_FALSE(ParseString("server { listen 80;"));
}

// Tests simple nested config
// Test case used to fail since ParseString incorrectly
// returned false on nested string
TEST_F(NginxStringConfigTest, NestedConfig) {
    EXPECT_TRUE(ParseString("first { second { center; } }"));
} 
