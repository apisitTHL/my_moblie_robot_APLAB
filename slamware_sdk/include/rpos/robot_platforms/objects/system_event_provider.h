#pragma once

#include <string> 
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
  
#include <rpos/core/diagnosis_info.h>
#include <rpos/robot_platforms/slamware_sdp_platform_config.h>

namespace rpos { namespace robot_platforms { namespace objects {

    class SystemEventProviderImpl;

    class RPOS_SLAMWARE_API SystemEventProvider : private boost::noncopyable
    {
    public:  
        SystemEventProvider(boost::shared_ptr<SystemEventProviderImpl> impl);
        ~SystemEventProvider();

        bool readEvents(std::vector<rpos::core::DiagnosisInfoInternalSystemEvent>& events);  
    private: 
        boost::shared_ptr<SystemEventProviderImpl> m_pImpl;
    };
} } }