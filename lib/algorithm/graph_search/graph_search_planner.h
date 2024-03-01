#pragma once
#include <spdlog/spdlog.h>

#include "bfs_planner.h"
#include "data_struct.h"
#include "dfs_planner.h"
#include "graph_search_planner.h"

namespace graph_search {

class GraphSearchPlanner {
public:
    GraphSearchPlanner() = default;
    virtual ~GraphSearchPlanner() = 0;

};    // class GraphSearchPlanner

};    // namespace graph_search
