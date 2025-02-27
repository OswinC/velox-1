/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "velox/exec/tests/utils/PlanBuilder.h"
#include "velox/functions/prestosql/aggregates/tests/AggregationTestBase.h"

using namespace facebook::velox::exec;
using namespace facebook::velox::exec::test;

namespace facebook::velox::aggregate::test {
namespace {
class ApproxDistinctTest : public AggregationTestBase {
 protected:
  static const std::vector<std::string> kFruits;
  static const std::vector<std::string> kVegetables;

  void testGlobalAgg(
      const VectorPtr& values,
      double maxStandardError,
      int64_t expectedResult) {
    auto op =
        PlanBuilder()
            .values({makeRowVector({values})})
            .singleAggregation(
                {},
                {fmt::format("approx_distinct(c0, {})", maxStandardError)},
                {BIGINT()})
            .planNode();
    ASSERT_EQ(readSingleValue(op), expectedResult);

    op = PlanBuilder()
             .values({makeRowVector({values})})
             .partialAggregation(
                 {},
                 {fmt::format("approx_distinct(c0, {})", maxStandardError)},
                 {},
                 {VARBINARY()})
             .finalAggregation({}, {"approx_distinct(a0)"}, {BIGINT()})
             .planNode();
    ASSERT_EQ(readSingleValue(op), expectedResult);

    op = PlanBuilder()
             .values({makeRowVector({values})})
             .singleAggregation(
                 {},
                 {fmt::format("approx_set(c0, {})", maxStandardError)},
                 {VARBINARY()})
             .project({"cardinality(a0)"})
             .planNode();
    ASSERT_EQ(readSingleValue(op), expectedResult);

    op = PlanBuilder()
             .values({makeRowVector({values})})
             .partialAggregation(
                 {},
                 {fmt::format("approx_set(c0, {})", maxStandardError)},
                 {},
                 {VARBINARY()})
             .finalAggregation({}, {"approx_set(a0)"}, {VARBINARY()})
             .project({"cardinality(a0)"})
             .planNode();
    ASSERT_EQ(readSingleValue(op), expectedResult);
  }

  void testGlobalAgg(const VectorPtr& values, int64_t expectedResult) {
    auto op = PlanBuilder()
                  .values({makeRowVector({values})})
                  .singleAggregation({}, {"approx_distinct(c0)"}, {BIGINT()})
                  .planNode();
    ASSERT_EQ(readSingleValue(op), expectedResult);

    op = PlanBuilder()
             .values({makeRowVector({values})})
             .partialAggregation({}, {"approx_distinct(c0)"}, {}, {VARBINARY()})
             .finalAggregation({}, {"approx_distinct(a0)"}, {BIGINT()})
             .planNode();
    ASSERT_EQ(readSingleValue(op), expectedResult);

    op = PlanBuilder()
             .values({makeRowVector({values})})
             .singleAggregation({}, {"approx_set(c0)"}, {VARBINARY()})
             .project({"cardinality(a0)"})
             .planNode();
    ASSERT_EQ(readSingleValue(op), expectedResult);

    op = PlanBuilder()
             .values({makeRowVector({values})})
             .partialAggregation({}, {"approx_set(c0)"}, {}, {VARBINARY()})
             .finalAggregation({}, {"approx_set(a0)"}, {VARBINARY()})
             .project({"cardinality(a0)"})
             .planNode();
    ASSERT_EQ(readSingleValue(op), expectedResult);
  }

  template <typename T, typename U>
  RowVectorPtr toRowVector(const std::unordered_map<T, U>& data) {
    std::vector<T> keys(data.size());
    transform(data.begin(), data.end(), keys.begin(), [](auto pair) {
      return pair.first;
    });

    std::vector<U> values(data.size());
    transform(data.begin(), data.end(), values.begin(), [](auto pair) {
      return pair.second;
    });

    return makeRowVector({makeFlatVector(keys), makeFlatVector(values)});
  }

  void testGroupByAgg(
      const VectorPtr& keys,
      const VectorPtr& values,
      const std::unordered_map<int32_t, int64_t>& expectedResults) {
    auto expected = toRowVector(expectedResults);

    auto op = PlanBuilder()
                  .values({makeRowVector({keys, values})})
                  .singleAggregation({0}, {"approx_distinct(c1)"}, {BIGINT()})
                  .planNode();
    assertQuery(op, expected);

    op =
        PlanBuilder()
            .values({makeRowVector({keys, values})})
            .partialAggregation({0}, {"approx_distinct(c1)"}, {}, {VARBINARY()})
            .finalAggregation({0}, {"approx_distinct(a0)"}, {BIGINT()})
            .planNode();
    assertQuery(op, expected);

    op = PlanBuilder()
             .values({makeRowVector({keys, values})})
             .singleAggregation({0}, {"approx_set(c1)"}, {VARBINARY()})
             .project({"c0", "cardinality(a0)"})
             .planNode();
    assertQuery(op, expected);

    op = PlanBuilder()
             .values({makeRowVector({keys, values})})
             .partialAggregation({0}, {"approx_set(c1)"}, {}, {VARBINARY()})
             .finalAggregation({0}, {"approx_set(a0)"}, {VARBINARY()})
             .project({"c0", "cardinality(a0)"})
             .planNode();
    assertQuery(op, expected);
  }
};

const std::vector<std::string> ApproxDistinctTest::kFruits = {
    "apple",
    "banana",
    "cherry",
    "dragonfruit",
    "grapefruit",
    "melon",
    "orange",
    "pear",
    "pineapple",
    "unknown fruit with a very long name",
    "watermelon"};

const std::vector<std::string> ApproxDistinctTest::kVegetables = {
    "cucumber",
    "tomato",
    "potato",
    "squash",
    "unknown vegetable with a very long name"};

TEST_F(ApproxDistinctTest, groupByIntegers) {
  vector_size_t size = 1'000;
  auto keys = makeFlatVector<int32_t>(size, [](auto row) { return row % 2; });
  auto values = makeFlatVector<int32_t>(
      size, [](auto row) { return row % 2 == 0 ? row % 17 : row % 21 + 100; });

  testGroupByAgg(keys, values, {{0, 17}, {1, 21}});
}

TEST_F(ApproxDistinctTest, groupByStrings) {
  vector_size_t size = 1'000;

  auto keys = makeFlatVector<int32_t>(size, [](auto row) { return row % 2; });
  auto values = makeFlatVector<StringView>(size, [&](auto row) {
    return StringView(
        row % 2 == 0 ? kFruits[row % kFruits.size()]
                     : kVegetables[row % kVegetables.size()]);
  });

  testGroupByAgg(keys, values, {{0, kFruits.size()}, {1, kVegetables.size()}});
}

TEST_F(ApproxDistinctTest, groupByHighCardinalityIntegers) {
  vector_size_t size = 1'000;
  auto keys = makeFlatVector<int32_t>(size, [](auto row) { return row % 2; });
  auto values = makeFlatVector<int32_t>(size, [](auto row) { return row; });

  testGroupByAgg(keys, values, {{0, 500}, {1, 500}});
}

TEST_F(ApproxDistinctTest, groupByVeryLowCardinalityIntegers) {
  vector_size_t size = 1'000;
  auto keys = makeFlatVector<int32_t>(size, [](auto row) { return row % 2; });
  auto values = makeFlatVector<int32_t>(
      size, [](auto row) { return row % 2 == 0 ? 27 : row % 3; });

  testGroupByAgg(keys, values, {{0, 1}, {1, 3}});
}

TEST_F(ApproxDistinctTest, globalAggIntegers) {
  vector_size_t size = 1'000;
  auto values =
      makeFlatVector<int32_t>(size, [](auto row) { return row % 17; });

  testGlobalAgg(values, 17);
}

TEST_F(ApproxDistinctTest, globalAggStrings) {
  vector_size_t size = 1'000;

  auto values = makeFlatVector<StringView>(size, [&](auto row) {
    return StringView(kFruits[row % kFruits.size()]);
  });

  testGlobalAgg(values, kFruits.size());
}

TEST_F(ApproxDistinctTest, globalAggHighCardinalityIntegers) {
  vector_size_t size = 1'000;
  auto values = makeFlatVector<int32_t>(size, [](auto row) { return row; });

  testGlobalAgg(values, 997);
}

TEST_F(ApproxDistinctTest, globalAggVeryLowCardinalityIntegers) {
  vector_size_t size = 1'000;
  auto values = makeFlatVector<int32_t>(size, [](auto /*row*/) { return 27; });

  testGlobalAgg(values, 1);
}

TEST_F(ApproxDistinctTest, globalAggIntegersWithError) {
  vector_size_t size = 1'000;
  auto values = makeFlatVector<int32_t>(size, [](auto row) { return row; });

  testGlobalAgg(values, 0.01, 1000);
  testGlobalAgg(values, 0.1, 1008);
  testGlobalAgg(values, 0.2, 930);
}

} // namespace
} // namespace facebook::velox::aggregate::test
