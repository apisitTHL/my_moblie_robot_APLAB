/**
* udp_client.h
*
* Copyright (c) 2020 Shanghai SlamTec Co., Ltd.
* Created By Jason @ 2020-06-03
*/

/**
* Usage
*
* class SomeUdpClientHandler : public UdpClient<SomeUdpClientHandler>::IUdpClientHandler
* {
*     // Implement pure virutal functions
* };
*
* class SomeUdpClient : public UdpClient<SomeUdpClientHandler> {
*     // Write ctor & dtor
* };
*
* boost::shared_ptr<SomeUdpClient> client(new SomeUdpClient());
* client->connectTo(someHost, somePort); 
* client->start();
*/
#pragma once

#include <rpos/rpos_config.h>
#include <rpos/system/util/log.h>
#include <rpos/system/util/string_utils.h>
#include <rpos/system/target_info.h>
#include <rpos/system/this_thread.h>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <list>
#include <vector>
#include <string.h>

#define RPOS_SYSTEM_UTIL_UDPCLIENT_DEFAULT_RX_BUFFER_SIZE 1024

namespace rpos { namespace system { namespace util {


    template <class UdpClientHandlerT, int RxBufferSize = RPOS_SYSTEM_UTIL_UDPCLIENT_DEFAULT_RX_BUFFER_SIZE>
    class UdpClient : public boost::enable_shared_from_this<UdpClient<UdpClientHandlerT, RxBufferSize>>, private boost::noncopyable {
    public:
        typedef boost::shared_ptr<UdpClient<UdpClientHandlerT, RxBufferSize>> Pointer;

    public:
        class IUdpClientHandler {
        public:
            virtual ~IUdpClientHandler() {};
            virtual void onHostResolved(Pointer client, boost::asio::ip::basic_resolver_iterator<boost::asio::ip::udp> begin, boost::asio::ip::basic_resolver_iterator<boost::asio::ip::udp> end) = 0;
            virtual void onHostResolveFailure(Pointer client, const boost::system::error_code& ec) = 0; 
            virtual void onSendError(Pointer client, const boost::system::error_code& ec) = 0;
            virtual void onSendComplete(Pointer client) = 0;
            virtual void onReceiveError(Pointer client, const boost::system::error_code& ec) = 0;
            virtual void onReceiveComplete(Pointer client, const unsigned char* buffer, size_t readBytes) = 0; 
        };

        class EmptyUdpClientHandler : public IUdpClientHandler {
        public:
            virtual void onHostResolved(Pointer client, boost::asio::ip::basic_resolver_iterator<boost::asio::ip::udp> begin, boost::asio::ip::basic_resolver_iterator<boost::asio::ip::udp> end)
            {}

            virtual void onHostResolveFailure(Pointer client, const boost::system::error_code& ec)
            {} 

            virtual void onSendError(Pointer client, const boost::system::error_code& ec)
            {}

            virtual void onSendComplete(Pointer client)
            {}

            virtual void onReceiveError(Pointer client, const boost::system::error_code& ec)
            {}

            virtual void onReceiveComplete(Pointer client, const unsigned char* buffer, size_t readBytes)
            {} 
        };

    public:
        UdpClient()
            : io_()
            , resolver_(io_)
            , socket_(io_)
            , handler_(new UdpClientHandlerT())
        {
            sending_ = false;
            receiving_ = false;
        }

        virtual ~UdpClient()
        {
            stop();

            if (ioThread_.joinable())
                ioThread_.join();

            handler_.reset();
        }

    public:
        void start()
        {
            boost::lock_guard<boost::mutex> guard(lock_);

            if (ioThread_.joinable())
                return;

            ioThread_ = boost::move(boost::thread(boost::bind(&UdpClient::worker_, this->shared_from_this()))); 
        }

        void stop()
        { 
            boost::lock_guard<boost::mutex> guard(lock_);
            if (!socket_.is_open())
                return;

            boost::system::error_code ec;
            socket_.shutdown(boost::asio::socket_base::shutdown_both, ec);
            socket_.close(ec); 
        }

        void send(const std::vector<unsigned char>& buffer)
        {
            if (buffer.empty())
                return;

            {
                boost::lock_guard<boost::mutex> guard(sendLock_);

                if (txBuffer_.empty())
                {
                    txBuffer_ = buffer;
                }
                else
                {
                    size_t offset = txBuffer_.size();
                    txBuffer_.resize(offset + buffer.size());
                    memcpy(&txBuffer_[offset], &buffer[0], buffer.size());
                }
            }
            startTransmit_();
        }

        void stopWorkerThread()
        {
            if (ioThread_.joinable())
                ioThread_.join();
        }

        boost::asio::ip::udp::socket& socket()
        {
            return socket_;
        }

    private:
        typedef boost::asio::ip::basic_resolver<boost::asio::ip::udp> resolver;

    public:
        bool connect(const std::string& host, int port)
        {  
            std::string portString = rpos::system::util::to_string(port);

#ifdef RPOS_TARGET_ANDROID
            resolver::query query(host, portString);
#else
            resolver::query query(host, portString, boost::asio::ip::resolver_query_base::all_matching);
#endif
            boost::system::error_code ec;
            resolver::iterator it = resolver_.resolve(query,ec);
            if (ec)
            {
                handler_->onHostResolveFailure(this->shared_from_this(), ec);
            }
            else
            {
                handler_->onHostResolved(this->shared_from_this(), it, resolver::iterator());
                boost::lock_guard<boost::mutex> guard(lock_);   
                auto destination = it->endpoint();
                socket_.open(destination.protocol()); 
                socket_.connect(destination); 
                startReceive_(); 
            }
            return true;
        } 
    private:
        // Send
        void startTransmit_()
        {
            {
                boost::lock_guard<boost::mutex> guard(sendLock_);

                if (sending_)
                    return;

                if (!txBuffer_.size())
                    return;

                sending_ = true;
            }

            transmit_();
        }

        void transmit_()
        {
            boost::lock_guard<boost::mutex> guard(sendLock_);

            transferredInBuffer_ = 0;
            transferingBuffer_ = txBuffer_;
            nativeTxBuffer_ = &(transferingBuffer_)[0];
            txBuffer_.clear();

            doTransmit_();
        }

        void doTransmit_()
        {
            socket_.async_send(
                boost::asio::buffer(nativeTxBuffer_ + transferredInBuffer_, transferingBuffer_.size() - transferredInBuffer_),
                boost::bind(&UdpClient::onTransmitComplete_, this->shared_from_this(), _1, _2)
                );
        }

        void onTransmitComplete_(const boost::system::error_code& ec, size_t written)
        {
            if (ec)
            {
                handler_->onSendError(this->shared_from_this(), ec);  
                return;
            }

            transferredInBuffer_ += written;

            if (transferredInBuffer_ == transferingBuffer_.size())
            {
                {
                    boost::lock_guard<boost::mutex> guard(sendLock_);
                    sending_ = false;
                }

                if (txBuffer_.size())
                {
                    return startTransmit_();
                }

                handler_->onSendComplete(this->shared_from_this());

                startTransmit_();
            }
            else
            {
                doTransmit_();
            }
        }

    private:
        // Receive
        void startReceive_()
        {
            bool expected = false;
            if(receiving_.compare_exchange_strong(expected,true))
                receive_(); 
        }

        void receive_()
        {
            socket_.async_receive(
                boost::asio::buffer(rxBlock_, RxBufferSize),
                boost::bind(&UdpClient::onReceiveComplete_, this->shared_from_this(), _1, _2)
                );
        }

        void onReceiveComplete_(const boost::system::error_code& ec, size_t readBytes)
        {
            if (ec)
            {
                //send data to unreachable destination, then async_receive will return this error
                //UDP is connectionless, so ignore this error
                if (ec != boost::asio::error::connection_refused)
                {
                    handler_->onReceiveError(this->shared_from_this(), ec);  
                    return;
                }
            } 
            else if (readBytes)
            {
                handler_->onReceiveComplete(this->shared_from_this(), rxBlock_, readBytes);
            }

            receive_();
        }

    private:
        void worker_()
        {
            rpos::system::this_thread::setCurrentThreadName("UdpClient");
            while(1)
            {
                try
                {
                    io_.run();
                    break;
                }
                catch (const boost::system::system_error& )
                {
                    printf("UDP client IO error in udp client\n");
                }
            }
        }

    private:
        boost::asio::io_service io_;
        boost::thread ioThread_;
        resolver resolver_;
        boost::asio::ip::udp::socket socket_; 
        boost::mutex lock_;

        boost::scoped_ptr<UdpClientHandlerT> handler_;

        boost::atomic_bool sending_;
        boost::atomic_bool receiving_;

        boost::mutex sendLock_;
        std::vector<unsigned char> txBuffer_;
        std::vector<unsigned char> transferingBuffer_;
        unsigned char* nativeTxBuffer_;
        size_t transferredInBuffer_;

        unsigned char rxBlock_[RxBufferSize]; 
    };

} } }
