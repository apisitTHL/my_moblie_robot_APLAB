#pragma once
#include <rpos/system/io/segmented_loop_files_stream_base.h>
#include <boost/atomic.hpp>
#include <cstdint>

namespace rpos { namespace system { namespace io {

    class RPOS_CORE_API SegmentedLoopFilesReadStream
        : public SegmentedLoopFilesStreamBase
    {
    public:
        explicit SegmentedLoopFilesReadStream(const Options& options);
        virtual ~SegmentedLoopFilesReadStream();

        virtual bool isOpen();
        virtual bool canRead();
        virtual bool canWrite();
        virtual bool canSeek();

        virtual bool open(const std::string dateStr, const std::string timeStr);
        virtual void close();

        virtual bool endOfStream();

        virtual int read(void* buffer, size_t size);
        virtual int write(const void* buffer, size_t size);
        virtual size_t tell();
        virtual void seek(SeekType type, int offset);

    public:
        std::vector<boost::shared_ptr<FileInfo>> getFileList() const;
        uint64_t tellEx();
        void seekEx(SeekType type, int64_t offset);
        uint64_t size() const;

    private:
        bool checkFileLogInfoValid_();
        bool openFile_();
        std::string getFileName_(int partIndex = 0);

    private:        
        boost::atomic<bool> open_;
        boost::shared_ptr<FileInfo> currentFileInfo_;
        
        std::vector<uint64_t> segmentedFileSize_;
        boost::shared_ptr<FileStream> currentFile_;
        int currentIndex_;
    };

}}}