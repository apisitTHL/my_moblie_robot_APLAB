#pragma once
#include <rpos/core/rpos_core_config.h>
#include <map>
#include <string>
#include <vector>

namespace rpos { namespace core {

    class RPOS_CORE_API Metadata
    {
    public:
        void set(const std::string& key, const std::string& value);
        const std::string& get(const std::string& key) const;
        bool tryGet(const std::string& key, std::string& outValue) const;

        void swap(Metadata& rOth) { dict_.swap(rOth.dict_); }
        void clear(void) { dict_.clear(); }

    public:
        template< typename T >
        void set(const std::string& key, const T& value);

        template< typename T >
        T get(const std::string& key) const;

        template< typename T >
        bool tryGet(const std::string& key, T& outValue) const;

        const std::map<std::string, std::string>& dict() const;
        std::map<std::string, std::string>& dict();

    private:
        std::map<std::string, std::string> dict_;
    };

}}
