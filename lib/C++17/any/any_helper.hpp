#pragma once

#include <string>
#include <boost/any.hpp>

namespace any_helper {

template <typename MsgType>
static bool Any2Type(const boost::any& input, MsgType& msg) {
    if (input.empty()) {
        return false;
    }
    bool ret = true;
    try {
        msg = boost::any_cast<MsgType>(input);
    } catch (const std::exception& e) {
        ret = false;
    }
    return ret;
}

template <typename MsgType>
static boost::any Type2Any(MsgType& msg) {
    boost::any tmp_any;
    try {
        tmp_any = boost::any_cast<MsgType>(msg);
    } catch (const std::exception& e) {
        return tmp_any;
    }
    return tmp_any;
}

static std::string GetAnyTypeName(const boost::any& input) {
    if (input.empty()) {
        return "";
    }
    std::string tmp = std::string(input.type().name());
    std::string::iterator it = tmp.begin();
    while (it != tmp.end()) {
        if ((*it >= '1') && (*it <= '9')) {
            it = tmp.erase(it);
        } else {
            it++;
        }
    }
    return tmp;
}

};    // namespace any_helper
