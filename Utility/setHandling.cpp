//
// Created by Hammad Rehman on 2020-11-14.
//

#include <set>
#include <algorithm>
#include "setHandling.h"

std::vector<std::string> getDifference(std::vector<std::string> oldVec, std::vector<std::string> newVec) {
    std::set<std::string> oldSet(oldVec.begin(), oldVec.end());
    std::set<std::string> newSet(newVec.begin(), newVec.end());
    std::set<std::string> result;

    std::set_difference(newSet.begin(), newSet.end(), oldSet.begin(), oldSet.end(), std::inserter(result, result.end()));
    std::vector<std::string> toReturn(result.size());
    std::copy(result.begin(), result.end(), toReturn.begin());
    return toReturn;

}