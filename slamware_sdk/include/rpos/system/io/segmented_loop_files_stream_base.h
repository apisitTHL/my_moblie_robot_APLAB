#pragma once
#include <rpos/system/io/i_stream.h>
#include <rpos/system/io/file_stream.h>
#include <boost/make_shared.hpp>
#include <map>

namespace rpos { namespace system { namespace io {

    class RPOS_CORE_API SegmentedLoopFilesStreamBase
        : public IStream
    {
    public:
        struct Options
        {
            bool enableLoopSave;
            std::string path;
            std::string ext;
            uint64_t spaceConstraint;
            int quantityConstraint;
            int splitSize;
        };

        struct FileInfo
        {
            std::string dateStr;
            std::string timeStr;
            uint64_t totalSize;
            int splittedCount;
            int minIndex;
        };

        explicit SegmentedLoopFilesStreamBase(const Options& options);
        virtual ~SegmentedLoopFilesStreamBase();

        virtual bool isOpen() = 0;
        virtual bool canRead() = 0;
        virtual bool canWrite() =0;
        virtual bool canSeek() = 0;

        virtual void close() = 0;

        virtual bool endOfStream() = 0;

        virtual int read(void* buffer, size_t size) = 0;
        virtual int write(const void* buffer, size_t size) = 0;
        virtual size_t tell() = 0;
        virtual void seek(SeekType type, int offset) = 0;

    public:
        virtual std::string filePath() const;        
        virtual uint64_t diskAvailableSize() const;
        virtual uint64_t streamAvailableSize() const;

    protected:
        bool checkPath_();
        void scanPath_();
        bool isOverflow_();

    protected:
        Options options_;
        std::map<std::string, boost::shared_ptr<FileInfo>> filePathInfo_;
        int totalCount_;
        uint64_t totalSize_;
    };

}}}