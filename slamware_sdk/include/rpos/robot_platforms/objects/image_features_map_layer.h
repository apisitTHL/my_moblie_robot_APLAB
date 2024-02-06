#pragma once

#include <rpos/core/pose.h>
#include <rpos/system/types.h>
#include <rpos/features/location_provider/image_features_map.h>
#include <rpos/robot_platforms/objects/map_layer.h>

#include <vector>


namespace rpos { namespace robot_platforms { namespace objects {


    class RPOS_SLAMWARE_API ImageFeaturesMapLayer : public MapLayer
    {
    public:
        static const char* const Type;

        ImageFeaturesMapLayer();
        ~ImageFeaturesMapLayer();

    public:
        virtual void clear(void);

        rpos::features::location_provider::ImageFeaturesMap::FeatureType getFeatureType() const;
        void setFeatureType(rpos::features::location_provider::ImageFeaturesMap::FeatureType type);
    public:
        const std::vector<rpos::features::location_provider::ImageFeaturesObservation>& featureObs() const;
        std::vector<rpos::features::location_provider::ImageFeaturesObservation>& featureObs();

    private:
        std::vector<rpos::features::location_provider::ImageFeaturesObservation> featureObs_;
        rpos::features::location_provider::ImageFeaturesMap::FeatureType type_;
    };

}}}