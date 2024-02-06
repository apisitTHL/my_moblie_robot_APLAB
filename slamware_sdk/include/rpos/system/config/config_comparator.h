#pragma once

#include <rpos/core/rpos_core_config.h>
#include <json/json.h>


namespace rpos { namespace system { namespace config {

    enum RPOS_CORE_API ConfigItemCompareState
    {
        ConfigItemCompareStateSame,
        ConfigItemCompareStateDifferent,
        ConfigItemCompareStateMonopolyOfFirst,
        ConfigItemCompareStateMonopolyOfSecond,
        ConfigItemCompareStateUnknown
    };

    class RPOS_CORE_API ConfigComparator
    {
    public:
        /* compare reference configs of "first" and "second"
        @first: the one of compared configs
        @second: the another of compared configs
        @result: combine all config items of compared configs with ConfigItemCompareState
        
        e.g.
        first:  { "item_a": 1, "item_c": 1, "item_d": 1 }
        second: { "item_b": 2, "item_c": 1, "item_d": 2 }
        result: { 
                    "item_a": ConfigItemCompareStateMonopolyOfFirst,
                    "item_b": ConfigItemCompareStateMonopolyOfSecond,
                    "item_c": ConfigItemCompareStateSame,
                    "item_d": ConfigItemCompareStateDifferent
                }
        */
        static bool compareConfig(const Json::Value & first, const Json::Value & second, Json::Value & result);
    };

} } }