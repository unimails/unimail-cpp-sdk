/* c++
 * @Author: i-curve
 * @Date: 2025-04-17 14:55:49
 * @Last Modified by: i-curve
 * @Name:
 */
#include "icurve/unimail-cpp-sdk.h"
#include <nlohmann/json.hpp>
#include <restclient-cpp/restclient.h>
#include "restclient-cpp/connection.h"

namespace icurve {
void UniMail::generate_rest() {
    this->rest = std::make_shared<RestClient::Connection>(this->host);
    concat_header();
}

void UniMail::concat_header() {
    RestClient::HeaderFields headers;
    headers["Content-Type"] = "application/json"; // 设置 Content-Type
    headers["Authorization"] = this->key;         // 添加 Authorization 头
    headers["accept-language"] = this->lang;      // 添加自定义头
    std::static_pointer_cast<RestClient::Connection>(rest)->SetHeaders(headers);
}

UniMail::UniMail(std::string key, std::string host) : key(key), host(host) {
    // 创建一个连接对象
    generate_rest();
}
UniMail::~UniMail(){};

void UniMail::set_host(std::string host) {
    this->host = host;
    generate_rest();
}
void UniMail::set_key(std::string key) {
    this->key = key;
    concat_header();
}
bool UniMail::set_language(std::string lang) {
    if (support_lang.count(lang) == 0) {
        return false;
    }
    this->lang = lang;
    concat_header();
    return true;
}
bool UniMail::check_connection() {
    RestClient::init();
    // 创建一个连接对象
    // RestClient::Connection *conn = new RestClient::Connection(this->host);

    // // 设置自定义请求头
    // RestClient::HeaderFields headers;
    // headers["Content-Type"] = "application/json"; // 设置 Content-Type
    // headers["Authorization"] = this->key;         // 添加 Authorization 头
    // headers["accept-language"] = this->lang;      // 添加自定义头
    // conn->SetHeaders(headers);

    // 发送 POST 请求
    RestClient::Response r =
        std::static_pointer_cast<RestClient::Connection>(rest)->post(
            "/api/email/checkConnection", "{}");
    if (r.code != 200)
        return false;
    RestClient::disable();
    return true;
}

UniMailResponse UniMail::send_email(std::string receiver, std::string subject,
                                    std::string content) {
    // RestClient::Connection *conn = new RestClient::Connection(this->host);

    // // 设置自定义请求头
    // RestClient::HeaderFields headers;
    // headers["Content-Type"] = "application/json"; // 设置 Content-Type
    // headers["Authorization"] = this->key;         // 添加 Authorization 头
    // headers["accept-language"] = this->lang;      // 添加自定义头
    // conn->SetHeaders(headers);
    nlohmann::json data;
    data["receiver"] = receiver;
    data["title"] = subject;
    data["content"] = content;

    // 发送 POST 请求
    RestClient::Response r =
        std::static_pointer_cast<RestClient::Connection>(rest)->post(
            "/api/email/sendEmail", data.dump());
    return UniMailResponse(r.code, r.body);
}

UniMailResponse UniMail::batch_send_email(std::vector<std::string> receivers,
                                          std::string subject,
                                          std::string content) {
    // RestClient::Connection *conn = new RestClient::Connection(this->host);

    // // 设置自定义请求头
    // RestClient::HeaderFields headers;
    // headers["Content-Type"] = "application/json"; // 设置 Content-Type
    // headers["Authorization"] = this->key;         // 添加 Authorization 头
    // headers["accept-language"] = this->lang;      // 添加自定义头
    // conn->SetHeaders(headers);
    nlohmann::json data;
    data["receivers"] = receivers;
    data["title"] = subject;
    data["content"] = content;
    // 发送 POST 请求
    RestClient::Response r =
        std::static_pointer_cast<RestClient::Connection>(rest)->post(
            "/api/email/batchSendEmail", data.dump());
    return UniMailResponse(r.code, r.body);
}

UniMailResponse::UniMailResponse(const int code, const std::string data)
    : data(data) {
    if (code != 200) {
        this->code = 400;
        this->msg = "network error";
        return;
    }
    nlohmann::json temp = nlohmann::json::parse(data);
    this->code = temp["code"];
    this->msg = temp["msg"];
}
}; // namespace icurve