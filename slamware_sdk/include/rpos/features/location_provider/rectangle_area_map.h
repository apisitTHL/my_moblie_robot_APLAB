#pragma once

#include <rpos/core/geometry.h>
#include <rpos/features/location_provider/map.h>
#include <rpos/features/artifact_provider.h>

#include <cstdint> 

namespace rpos { namespace features { namespace location_provider {  

    class RectangleAreaMap;
    namespace detail {
        class RectangleAreaMapImpl;
        template<>
        struct RPOS_CORE_API map_caster < RectangleAreaMap > {
            static RectangleAreaMap cast(const Map& map);
        };
    }

    class RPOS_CORE_API RectangleAreaMap: public Map
    {
    public:
        typedef detail::RectangleAreaMapImpl impl_t;

        RPOS_OBJECT_CTORS_WITH_BASE(RectangleAreaMap, Map);
        RectangleAreaMap(boost::shared_ptr<detail::RectangleAreaMapImpl> impl);
        virtual ~RectangleAreaMap();
        static RectangleAreaMap createMap(rpos::system::types::_u64 timestamp = 0);

        virtual bool readFromStream(rpos::system::io::IStream& in);
        virtual bool writeToStream(rpos::system::io::IStream& out) const;

        std::string Id() const;
        std::string& Id();
    public:
        void clear();
        size_t size() const;
        void resize(size_t N);
        void addRectangleAreas(const std::vector<artifact_provider::RectangleArea>& areas);
        const artifact_provider::RectangleArea& operator[](size_t i) const;
        artifact_provider::RectangleArea& operator[](size_t i);
        static const uint32_t Version;
    };

}}}
