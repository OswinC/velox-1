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

using namespace facebook::velox::exec::test;

namespace facebook::velox::aggregate::test {

namespace {

class BitwiseAggregationTest : public AggregationTestBase {
 protected:
  std::shared_ptr<const RowType> rowType_{
      ROW({"c0", "c1", "c2", "c3", "c4"},
          {BIGINT(), TINYINT(), SMALLINT(), INTEGER(), BIGINT()})};
};

TEST_F(BitwiseAggregationTest, bitwiseOr) {
  auto vectors = makeVectors(rowType_, 10, 100);
  createDuckDbTable(vectors);

  // Global partial aggregation.
  {
    auto agg = PlanBuilder()
                   .values(vectors)
                   .partialAggregation(
                       {},
                       {"bitwise_or_agg(c1)",
                        "bitwise_or_agg(c2)",
                        "bitwise_or_agg(c3)",
                        "bitwise_or_agg(c4)"})
                   .planNode();
    assertQuery(
        agg, "SELECT bit_or(c1), bit_or(c2), bit_or(c3), bit_or(c4) FROM tmp");
  }

  // Global final aggregation.
  {
    PlanBuilder builder;
    auto partialAgg = builder.values(vectors)
                          .partialAggregation(
                              {},
                              {"bitwise_or_agg(c1)",
                               "bitwise_or_agg(c2)",
                               "bitwise_or_agg(c3)",
                               "bitwise_or_agg(c4)"})
                          .planNode();
    auto finalAgg = builder
                        .finalAggregation(
                            {},
                            {"bitwise_or_agg(a0)",
                             "bitwise_or_agg(a1)",
                             "bitwise_or_agg(a2)",
                             "bitwise_or_agg(a3)"})
                        .planNode();
    assertQuery(
        finalAgg,
        "SELECT bit_or(c1), bit_or(c2), bit_or(c3), bit_or(c4) FROM tmp");
  }

  // Group by partial aggregation.
  {
    auto agg = PlanBuilder()
                   .values(vectors)
                   .project(
                       {"c0 % 10", "c1", "c2", "c3", "c4"},
                       {"c0 % 10", "c1", "c2", "c3", "c4"})
                   .partialAggregation(
                       {0},
                       {"bitwise_or_agg(c1)",
                        "bitwise_or_agg(c2)",
                        "bitwise_or_agg(c3)",
                        "bitwise_or_agg(c4)"})
                   .planNode();
    assertQuery(
        agg,
        "SELECT c0 % 10, bit_or(c1), bit_or(c2), bit_or(c3), bit_or(c4) FROM tmp GROUP BY 1");
  }

  // Group by final aggregation.
  {
    PlanBuilder builder;
    auto partialAgg = builder.values(vectors)
                          .project(
                              {"c0 % 10", "c1", "c2", "c3", "c4"},
                              {"c0 % 10", "c1", "c2", "c3", "c4"})
                          .partialAggregation(
                              {0},
                              {"bitwise_or_agg(c1)",
                               "bitwise_or_agg(c2)",
                               "bitwise_or_agg(c3)",
                               "bitwise_or_agg(c4)"})
                          .planNode();
    auto finalAgg = builder
                        .finalAggregation(
                            {0},
                            {"bitwise_or_agg(a0)",
                             "bitwise_or_agg(a1)",
                             "bitwise_or_agg(a2)",
                             "bitwise_or_agg(a3)"})
                        .planNode();
    assertQuery(
        finalAgg,
        "SELECT c0 % 10, bit_or(c1), bit_or(c2), bit_or(c3), bit_or(c4) FROM tmp GROUP BY 1");
  }
}

TEST_F(BitwiseAggregationTest, bitwiseAnd) {
  auto vectors = makeVectors(rowType_, 10, 100);
  createDuckDbTable(vectors);

  // Global partial aggregation.
  {
    auto agg = PlanBuilder()
                   .values(vectors)
                   .partialAggregation(
                       {},
                       {"bitwise_and_agg(c1)",
                        "bitwise_and_agg(c2)",
                        "bitwise_and_agg(c3)",
                        "bitwise_and_agg(c4)"})
                   .planNode();
    assertQuery(
        agg,
        "SELECT bit_and(c1), bit_and(c2), bit_and(c3), bit_and(c4) FROM tmp");
  }

  // Global final aggregation.
  {
    PlanBuilder builder;
    auto partialAgg = builder.values(vectors)
                          .partialAggregation(
                              {},
                              {"bitwise_and_agg(c1)",
                               "bitwise_and_agg(c2)",
                               "bitwise_and_agg(c3)",
                               "bitwise_and_agg(c4)"})
                          .planNode();
    auto finalAgg = builder
                        .finalAggregation(
                            {},
                            {"bitwise_and_agg(a0)",
                             "bitwise_and_agg(a1)",
                             "bitwise_and_agg(a2)",
                             "bitwise_and_agg(a3)"})
                        .planNode();
    assertQuery(
        finalAgg,
        "SELECT bit_and(c1), bit_and(c2), bit_and(c3), bit_and(c4) FROM tmp");
  }

  // Group by partial aggregation.
  {
    auto agg = PlanBuilder()
                   .values(vectors)
                   .project(
                       {"c0 % 10", "c1", "c2", "c3", "c4"},
                       {"c0 % 10", "c1", "c2", "c3", "c4"})
                   .partialAggregation(
                       {0},
                       {"bitwise_and_agg(c1)",
                        "bitwise_and_agg(c2)",
                        "bitwise_and_agg(c3)",
                        "bitwise_and_agg(c4)"})
                   .planNode();
    assertQuery(
        agg,
        "SELECT c0 % 10, bit_and(c1), bit_and(c2), bit_and(c3), bit_and(c4) FROM tmp GROUP BY 1");
  }

  // Group by final aggregation.
  {
    PlanBuilder builder;
    auto partialAgg = builder.values(vectors)
                          .project(
                              {"c0 % 10", "c1", "c2", "c3", "c4"},
                              {"c0 % 10", "c1", "c2", "c3", "c4"})
                          .partialAggregation(
                              {0},
                              {"bitwise_and_agg(c1)",
                               "bitwise_and_agg(c2)",
                               "bitwise_and_agg(c3)",
                               "bitwise_and_agg(c4)"})
                          .planNode();
    auto finalAgg = builder
                        .finalAggregation(
                            {0},
                            {"bitwise_and_agg(a0)",
                             "bitwise_and_agg(a1)",
                             "bitwise_and_agg(a2)",
                             "bitwise_and_agg(a3)"})
                        .planNode();
    assertQuery(
        finalAgg,
        "SELECT c0 % 10, bit_and(c1), bit_and(c2), bit_and(c3), bit_and(c4) FROM tmp GROUP BY 1");
  }
}

} // namespace
} // namespace facebook::velox::aggregate::test
