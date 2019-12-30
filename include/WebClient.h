#ifndef LIBCURLPP_ASYNC_WEBCLIENT_H_
#define LIBCURLPP_ASYNC_WEBCLIENT_H_

/*
 *  CURLPP-Async WebClient
 *  12/29/19 20:09
 */

// CURL
#include <curl/curl.h>

// STL
#include <functional>
#include <string>

namespace CURLPPAsync
{
    class Handle;

    // WebClient is a web client that wraps around a CURL easy handle, which has its own cookies and settings.
    // Despite the name, it can be instantiated and used on its own synchronously with WebClient::{METHOD}. The
    // internal buffer can be fetched with WebClient::GetBuffer, and options can be set with WebClient::SetOpt.
    // It must be created on the same thread as the handle.
    class WebClient
    {
    public:
        using RecvCallback_t = std::function<void(const CURLcode code)>;
        // Creates a webclient with reference to a handle
        WebClient(Handle& handle) noexcept;

        WebClient(WebClient&& other) noexcept;
        WebClient& operator=(WebClient&& other) noexcept;

        WebClient(const WebClient& other) = delete;
        WebClient& operator=(const WebClient& other) = delete;

        // Cancels any async operations and calls the handler with
        ~WebClient() noexcept;

        // Starts a synchronous GET operation on a URL, and returns the result.
        CURLcode GET(const std::string& url) noexcept; 

        // Starts an Asynchronous GET operation on a URL, and calls recvCallback on completion.
        // Note that there must not already be an in-progress operation on the object.
        // std::runtime_error will be thrown in this case.
        void AsyncGET(const std::string& url, RecvCallback_t&& recvCallback);

        std::string GetData() const { return m_data; }
    private:
        CURL* m_curl;
        std::reference_wrapper<Handle> m_handle;

        std::string m_data;

        static size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata);
    };
}

#endif