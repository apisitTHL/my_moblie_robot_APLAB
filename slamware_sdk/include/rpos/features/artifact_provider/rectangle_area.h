#pragma once

#include <rpos/rpos_config.h> 
#include <vector>

namespace rpos { namespace features { namespace artifact_provider {
       
        enum RectangleAreaType
        {
            MaintenanceArea = 0,
            UnidirectionalSchedulingArea = 1,
            BidirectionalSchedulingArea = 2,
            HomeStackSchedulingArea = 3, 
            //add new type here
            RectangleAreaTypeMax
        };

        static const char MetaDataKey_RectangleAreaType[] = "rectangle_area_type";
        static const char MetaDataKey_EscapeDistance[] = "escape_distance"; 
} } }
