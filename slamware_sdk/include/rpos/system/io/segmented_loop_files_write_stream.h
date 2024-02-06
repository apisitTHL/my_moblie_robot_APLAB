#pragma once
#include <rpos/system/io/segmented_loop_files_stream_base.h>
#include <boost/atomic.hpp>
#include <deque>

namespace rpos { namespace system { namespace io {

    class RPOS_CORE_API SegmentedLoopFilesWriteStream
        : public SegmentedLoopFilesStreamBase
    {
    public:
        explicit SegmentedLoopFilesWriteStream(const Options& options);
        virtual ~SegmentedLoopFilesWriteStream();

        virtual bool isOpen();
        virtual bool canRead();
        virtual bool canWrite();
        virtual bool canSeek();

        virtual bool open();
        virtual void close();

        virtual bool endOfStream();

        virtual int read(void* buffer, size_t size);
        virtual int write(const void* buffer, size_t size);
        virtual size_t tell();
        virtual void seek(SeekType type, int offset);
        virtual void markSplit();
        virtual void flush();

    public:
        virtual std::string fileNamePrefix() const;

    private:
        void split_();
        bool loopSave_();
        std::string genFileName_();
        bool createFile_();
        bool removeFile_(boost::shared_ptr<FileStream> file);

        void genFilePrefix_();
        void replaceString_(std::string & str, const std::string & oldValue, const std::string & newValue);

    private:
        std::deque<boost::shared_ptr<FileStream>> files_;
        size_t size_;
        int segmentNum_;
        boost::atomic<bool> open_;
        std::string prefix_;
    };

}}}