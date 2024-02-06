#pragma once
#include <rpos/core/pose.h>
#include <rpos/features/location_provider/map.h>

namespace rpos { namespace features { namespace location_provider {
    
    struct FeatureDescriptor
    {
        uint32_t wordId;
        float wordValue;
    };
    
    struct ImageFeaturesObservation
    {
        unsigned int ID;
        rpos::core::Pose cameraPose;
        std::vector<uint8_t> features; 

        ImageFeaturesObservation(): ID(0)
        {}
    };

    class ImageFeaturesMap;
    namespace detail {
        class ImageFeaturesMapImpl;
        template<>
        struct RPOS_CORE_API map_caster < ImageFeaturesMap > {
            static ImageFeaturesMap cast(const Map& map);
        };
    }

    class RPOS_CORE_API ImageFeaturesMap : public Map {

    public: 

        enum FeatureType
        {
            FeatureTypeUnknown = 0,
            FeatureTypeDBowORB,
            FeatureTypeVSlamAprilTag,
            FeatureTypeSnapshot,
            //Add new type here
            FeatureTypeMax
        };

        typedef detail::ImageFeaturesMapImpl impl_t;

        RPOS_OBJECT_CTORS_WITH_BASE(ImageFeaturesMap, Map);

        ImageFeaturesMap(boost::shared_ptr<detail::ImageFeaturesMapImpl> impl);
        virtual ~ImageFeaturesMap();
        static ImageFeaturesMap createMap(rpos::system::types::_u64 timestamp = 0);

        virtual bool readFromStream(rpos::system::io::IStream &in);
        virtual bool writeToStream(rpos::system::io::IStream &out) const;
         
        FeatureType getFeatureType()const;
        void setFeatureType(FeatureType type);
        void copyFrom(const ImageFeaturesMap& that);

        void clear();
        size_t size() const;
        void resize(size_t N);
        const ImageFeaturesObservation& operator[](size_t i) const;
        ImageFeaturesObservation& operator[](size_t i);
    };


}}}