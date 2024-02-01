#pragma once

#include "parser/types.h"
#include "converters.h"

Requests generate_requests();

Rpc__FilterChain* convertNodesToFilterChain(ParsedNode* nodes);
