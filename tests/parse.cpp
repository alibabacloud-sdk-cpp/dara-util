#include "gtest/gtest.h"
#include <darabonba/util.hpp>
#include <boost/any.hpp>

using namespace std;

TEST(tests_parse, toBytes) {
  vector<uint8_t> bytes({116, 101, 115, 116});
  ASSERT_EQ(bytes, Darabonba_Util::Client::toBytes(make_shared<string>("test")));
}

TEST(tests_parse, toString) {
  vector<uint8_t> bytes({116, 101, 115, 116});
  ASSERT_EQ(string("test"), Darabonba_Util::Client::toString(make_shared<vector<uint8_t>>(bytes)));
}

template<typename T>
bool eq_boost_any(T val1, boost::any val2) {
  return val1 == boost::any_cast<T>(val2);
}

TEST(tests_parse, parseJSON) {
  string json =
      R"({"bool":true,"foo":"bar","long":9223372036854775807,"map":{"foo":"bar"},"string":"string","vector":["foo","bar"]})";
  map<string, boost::any> m = {
      {"foo", boost::any("bar")},
      {"string", boost::any(string("string"))},
      {"long", boost::any(LONG_MAX)},
      {"bool", boost::any(true)},
      {"vector", boost::any(vector<boost::any>({{boost::any("foo")}}))},
      {"map", boost::any(map<string, boost::any>({{"foo", boost::any("bar")}}))}
  };
  boost::any target = Darabonba_Util::Client::parseJSON(make_shared<string>(json));
  map<string, boost::any> data = boost::any_cast<map<string, boost::any>>(target);
  vector<boost::any> vec = boost::any_cast<vector<boost::any>>(data["vector"]);

  ASSERT_EQ(string("foo"), boost::any_cast<string>(vec[0]));
  ASSERT_EQ(string("bar"), boost::any_cast<string>(vec[1]));
}

TEST(tests_parse, toJSONString) {
  map<string, boost::any> m = {
      {"foo", boost::any("bar")},
      {"string", boost::any(string("string"))},
      {"long", boost::any(LONG_MAX)},
      {"bool", boost::any(true)},
      {"vector", boost::any(vector<boost::any>({
                                                   {boost::any("foo")}
                                               }))
      },
      {"map", boost::any(map<string, boost::any>({
                                                     {"foo", boost::any("bar")}
                                                 }))
      }
  };
  string res = Darabonba_Util::Client::toJSONString(make_shared<boost::any>(m));
  ASSERT_EQ(string(
                "{\"bool\":true,\"foo\":\"bar\",\"long\":9223372036854775807,\"map\":{\"foo\":\"bar\"},\"string\":string,\"vector\":[\"foo\"]}"),
            res);
}