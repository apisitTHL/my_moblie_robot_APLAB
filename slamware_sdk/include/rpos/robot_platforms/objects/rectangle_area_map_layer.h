#pragma once

#include <rpos/core/pose.h>
#include <rpos/system/types.h>
#include <rpos/features/location_provider/rectangle_area_map.h>

#include <rpos/core/metadata.h>
#include <rpos/robot_platforms/objects/map_layer.h>

#include <vector>

namespace rpos { namespace robot_platforms { namespace objects {

    class RPOS_SLAMWARE_API RectangleAreaMapLayer : public MapLayer
    {
    public:
        static const char* const Type;
        static const int Version;

        RectangleAreaMapLayer();
        virtual ~RectangleAreaMapLayer();

    public:
        virtual void clear(void);

        std::string getId() const;
        void setId(const std::string & id);
    public:
        const std::vector<rpos::features::artifact_provider::RectangleArea> & areas() const;
        std::vector<rpos::features::artifact_provider::RectangleArea> & areas();

    private:
        std::vector<rpos::features::artifact_provider::RectangleArea> areas_;
        std::string Id_;
    };

}}}
