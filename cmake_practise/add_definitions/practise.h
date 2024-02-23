#pragma once

#ifdef PIG_TESTER
#include <iostream>
#endif

class AddDefinitionsTester {
public:
    AddDefinitionsTester() {}
    ~AddDefinitionsTester() {}

    bool init();

private:
    bool m_isInit{false};
};  // AddDefinitionsTester