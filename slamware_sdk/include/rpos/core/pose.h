/**
* pose.h
* Robot Pose
*
* Created By Tony Huang @ 2014-5-22
* Copyright (c) 2014 Shanghai SlamTec Co., Ltd.
*/

#pragma once

#include <rpos/rpos_config.h>
#include <rpos/core/rpos_core_config.h>
#include <rpos/system/types.h>
#include <Eigen/Eigen>
#include <rpos/core/geometry.h>

namespace rpos {
    namespace core {
        enum ACTION_DIRECTION
        {
            FORWARD,
            BACKWARD,
            TURNRIGHT,
            TURNLEFT,
            INVALIDDIRECTION
        };

        enum IMUBitMap
        {
            NO_IMU_READY = 0x0000,
            QUATERNION_READY = 0x0001,
            CALIBRATED_ACC_READY = 0x0002,
            CALIBRATED_GYRO_READY = 0x0004,
            CALIBRATED_COMPASS_READY = 0x0008,
            RAW_ACC_READY = 0x0010,
            RAW_GYRO_READY = 0x0020,
            RAW_COMPASS_READY = 0x0040,
            FUSION_6DOF_READY = 0x0080,
            FUSION_9DOF_READY = 0x0100,
            EULER_ANGLE_READY = 0x0200
        };

        class RPOS_CORE_API Location{
        public:
            Location();
            Location(double x, double y, double z = 0);
            Location(const Location&);
            ~Location();

        public:
            Location& operator=(const Location&);
            bool operator==(const Location&) const;

        public:
            double x() const;
            double& x();

            double y() const;
            double& y();

            double z() const;
            double& z();

            double distanceTo(const Location& that) const;

        private:
            double x_, y_, z_;
        };

        class RPOS_CORE_API Rotation{
        public:
            Rotation();
            Rotation(double yaw, double pitch = 0, double roll = 0);
            Rotation(const Rotation&);
            ~Rotation();

        public:
            Rotation& operator=(const Rotation&);
            bool operator==(const Rotation&) const;

        public:
            double yaw() const;
            double& yaw();

            double pitch() const;
            double& pitch();

            double roll() const;
            double& roll();

        private:
            double yaw_, pitch_, roll_;
        };

        class RPOS_CORE_API Pose{
        public:
            Pose();
            Pose(const Location&, const Rotation&);
            Pose(const Location&);
            Pose(const Rotation&);
            Pose(const Pose&);
            ~Pose();

        public:
            Pose& operator=(const Pose&);
            bool operator==(const Pose&) const;
            bool operator<(const Pose&) const;

        public:
            const Location& location() const;
            Location& location();

            const Rotation& rotation() const;
            Rotation& rotation();

            double x() const;
            double& x();

            double y() const;
            double& y();

            double z() const;
            double& z();

            double yaw() const;
            double& yaw();

            double pitch() const;
            double& pitch();

            double roll() const;
            double& roll();

        private:
            Location location_;
            Rotation rotation_;
        };

        class RPOS_CORE_API Quaternion
        {
        public:
            Quaternion();
            Quaternion(const Quaternion&);
            Quaternion(double x, double y, double z, double w);
        public:
            Quaternion& operator=(const Quaternion &);
        public:
            rpos::core::Vector3f convertToEulerAngle();
        public:
            double x() const;
            double& x();
            double y() const;
            double& y();
            double z() const;
            double& z();
            double w() const;
            double& w();
        private:
            double x_;
            double y_;
            double z_;
            double w_;
        };

        class RPOS_CORE_API Direction{
        public:
            Direction(ACTION_DIRECTION direction = FORWARD);
            ~Direction();

        public:
            Direction& operator=(const Direction&);

        public:
            ACTION_DIRECTION direction() const;
            ACTION_DIRECTION& direction();
            operator bool() const;
        private:
            ACTION_DIRECTION direction_;
        };

        class RPOS_CORE_API  KeyFramePose
        {
        public:
            KeyFramePose();
            KeyFramePose(uint64_t id, Pose pose);
            ~KeyFramePose(); 

        public:
            bool operator<(const KeyFramePose& that) const;
            bool operator>(const KeyFramePose& that) const;
            bool operator==(const KeyFramePose& that) const;
            KeyFramePose& operator=(const KeyFramePose& that);

        public:
            uint64_t id() const;
            uint64_t &id();
            Pose pose()const;
            Pose& pose();
        private:
            uint64_t id_;
            Pose pose_;
        };

        struct RPOS_CORE_API WeightedPose
        {
        public:
            WeightedPose() :weight_(0.f), pose_(Pose(Location(0.f, 0.f, 0.f), Rotation(0.f, 0.f, 0.f))) {}
            WeightedPose(float weight, Pose pose);
            ~WeightedPose();

        public:
            bool operator==(const WeightedPose& that) const;
            WeightedPose& operator=(const WeightedPose& that);
            bool operator<(const WeightedPose& that) const;

        public:
            float weight() const;
            float &weight();
            Pose pose()const;
            Pose& pose();
        private:
            float weight_;
            Pose pose_;
        };

        struct RPOS_CORE_API Imu
        {
        public:
            Imu();
            Imu(uint32_t timestamp, uint32_t avaliblityBitMap,
                Quaternion quaternion, rpos::core::Vector3f eulerAngle,
                rpos::core::Vector3f acc, rpos::core::Vector3f gyro, rpos::core::Vector3f compass,
                rpos::core::Vector3f rawAcc, rpos::core::Vector3f rawGyro, rpos::core::Vector3f rawCompass);
            Imu(const Imu& that);
            ~Imu();
        public:
            Imu& operator=(const Imu& that);
        public:
            uint32_t timestamp() const;
            uint32_t& timestamp();
            uint32_t availabilityBitmap() const;
            uint32_t& availabilityBitmap();
            Quaternion quaternion() const;
            Quaternion& quaternion();
            rpos::core::Vector3f eulerAngle() const;
            rpos::core::Vector3f& eulerAngle();
            rpos::core::Vector3f acc() const;
            rpos::core::Vector3f& acc();
            rpos::core::Vector3f gyro() const;
            rpos::core::Vector3f& gyro();
            rpos::core::Vector3f compass() const;
            rpos::core::Vector3f& compass();
            rpos::core::Vector3f rawAcc() const;
            rpos::core::Vector3f& rawAcc();
            rpos::core::Vector3f rawGyro() const;
            rpos::core::Vector3f& rawGyro();
            rpos::core::Vector3f rawCompass() const;
            rpos::core::Vector3f& rawCompass();
        private:
            uint32_t timestamp_;
            uint32_t availabilityBitmap_;
            Quaternion quaternion_;
            rpos::core::Vector3f eulerAngle_;
            rpos::core::Vector3f acc_;
            rpos::core::Vector3f gyro_;
            rpos::core::Vector3f compass_;
            rpos::core::Vector3f rawAcc_;
            rpos::core::Vector3f rawGyro_;
            rpos::core::Vector3f rawCompass_;
        };
    }
}
