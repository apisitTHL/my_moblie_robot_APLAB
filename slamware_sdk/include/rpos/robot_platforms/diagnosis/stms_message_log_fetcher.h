#pragma once
#include <rpos/robot_platforms/slamware_sdp_platform_config.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <cstdint>

namespace rpos { namespace robot_platforms {
    class SlamwareCorePlatform;
}}

namespace rpos { namespace robot_platforms { namespace diagnosis {
    
    class MessageLogFetcher;

    class RPOS_SLAMWARE_API StmsMessageLogFetcher : private boost::noncopyable
    {
    public:
        StmsMessageLogFetcher(SlamwareCorePlatform& corePltfm);
        ~StmsMessageLogFetcher();
    public:
        int updateSnapshot();
        size_t getSnapshotSize(int id);
        int readSnapshot(int id, size_t offset, std::uint8_t* buffer, size_t size);
        void releaseSnapshot(int id);
    private:
        boost::shared_ptr<MessageLogFetcher> logFetcher_;
    };
}}}