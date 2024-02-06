#pragma once

#include <rpos/core/pose.h>
#include <rpos/system/types.h>

#include <rpos/core/metadata.h>
#include <rpos/robot_platforms/objects/map_layer.h>

#include <map>
#include <string>
#include <vector>

#define RPOS_COMPOSITEMAP_POSE_FLAG_HEADING_SENSITIVE          0x01

namespace rpos { namespace robot_platforms { namespace objects {

    struct PoseEntry
    {
        std::string name;
        std::vector<std::string> tags;
        core::Pose pose;
        rpos::system::types::_u8 flags;
        rpos::core::Metadata metadata;
    };
    typedef std::map<std::string, PoseEntry> PoseEntryMap;

    class RPOS_SLAMWARE_API PoseMapLayer : public MapLayer
    {
    public:
        static const char* const Type;

    public:
        virtual void clear(void);

    public:
        const PoseEntryMap& poses() const;
        PoseEntryMap& poses();

    private:
        PoseEntryMap poses_;
    };

}}}