/* c++
 * @Author: allcloud.top
 * @Date: 2025-04-17 14:55:49
 * @Last Modified by: allcloud.top
 * @Name:
 */
#include "allcloud/unimail-cpp-sdk.h"
#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <mutex>
#include <sstream>
#include <nlohmann/json.hpp>
#include <restclient-cpp/restclient.h>
#include "restclient-cpp/connection.h"

namespace allcloud {
namespace {
void ensure_restclient_ready() {
    static std::once_flag init_flag;
    std::call_once(init_flag, []() {
        RestClient::init();
        std::atexit([]() { RestClient::disable(); });
    });
}

bool read_binary_file(const std::string &path, std::string &out_content) {
    std::ifstream in(path.c_str(), std::ios::binary);
    if (!in.is_open()) {
        return false;
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    out_content = ss.str();
    return true;
}

std::string join_with_semicolon(const std::vector<std::string> &items) {
    std::ostringstream ss;
    for (size_t i = 0; i < items.size(); ++i) {
        if (i != 0) {
            ss << ";";
        }
        ss << items[i];
    }
    return ss.str();
}

void append_field(std::string &body, const std::string &boundary,
                  const std::string &name, const std::string &value) {
    body += "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"" + name + "\"\r\n\r\n";
    body += value + "\r\n";
}

void append_file_field(std::string &body, const std::string &boundary,
                       const std::string &name, const std::string &filename,
                       const std::string &content) {
    body += "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"" + name +
            "\"; filename=\"" + filename + "\"\r\n";
    body += "Content-Type: application/octet-stream\r\n\r\n";
    body += content;
    body += "\r\n";
}
} // namespace

void UnimailReq::appendAttachment(const EmailAttachment &attachment) {
    attachments.push_back(attachment);
}

void UnimailReq::appendFile(const std::string &name, const std::string &path) {
    std::string file_content;
    if (!read_binary_file(path, file_content)) {
        throw std::runtime_error("unimail: failed to read attachment file: " +
                                 path);
    }
    EmailAttachment attachment;
    attachment.name = name;
    attachment.file_attachment = file_content;
    attachment.type = AttachmentType::File;
    attachments.push_back(attachment);
}

void UnimailReq::appendUri(const std::string &name, const std::string &uri) {
    EmailAttachment attachment;
    attachment.name = name;
    attachment.url_attachment = uri;
    attachment.type = AttachmentType::Url;
    attachments.push_back(attachment);
}

void Unimail::generate_rest() {
    ensure_restclient_ready();
    this->rest = std::make_shared<RestClient::Connection>(this->host);
}

void Unimail::concat_header(const std::string &content_type) const {
    RestClient::HeaderFields headers;
    headers["Content-Type"] = content_type;
    headers["Accept-Language"] = this->lang;
    std::static_pointer_cast<RestClient::Connection>(rest)->SetHeaders(headers);
}

Unimail::Unimail(std::string key, std::string host) : key(key), host(host) {
    // 创建一个连接对象
    generate_rest();
}

Unimail::~Unimail() {}

void Unimail::set_host(std::string host) {
    this->host = host;
    generate_rest();
}

void Unimail::set_key(std::string key) { this->key = key; }

bool Unimail::set_language(std::string lang) {
    if (support_lang.count(lang) == 0) {
        return false;
    }
    this->lang = lang;
    return true;
}

UniResponse Unimail::check_connection() {
    ensure_restclient_ready();
    nlohmann::json data;
    data["authorization"] = this->key;

    concat_header();
    // 发送 POST 请求
    RestClient::Response r =
        std::static_pointer_cast<RestClient::Connection>(rest)->post(
            "/v2/checkConnection", data.dump());
    return UniResponse(r.code, r.body);
}

UniResponse Unimail::send_email(const UnimailReq &req) {
    ensure_restclient_ready();
    if (req.receivers.empty()) {
        return UniResponse(200,
                           "{\"code\":400,\"msg\":\"receivers is required\"}");
    }
    if (req.subject.empty()) {
        return UniResponse(200,
                           "{\"code\":400,\"msg\":\"subject is required\"}");
    }
    if (req.txt_content.empty() && req.html_content.empty()) {
        return UniResponse(200, "{\"code\":400,\"msg\":\"txtContent and "
                                "htmlContent cannot both be empty\"}");
    }

    const std::string boundary = "----unimailcppsdkboundary";
    std::string body;
    body.reserve(4096);

    append_field(body, boundary, "authorization", this->key);
    append_field(body, boundary, "receiver",
                 join_with_semicolon(req.receivers));
    append_field(body, boundary, "from", req.from);

    if (!req.cc.empty()) {
        append_field(body, boundary, "cc", req.cc);
    }
    if (!req.bcc.empty()) {
        append_field(body, boundary, "bcc", req.bcc);
    }
    append_field(body, boundary, "subject", req.subject);
    if (!req.txt_content.empty()) {
        append_field(body, boundary, "txtContent", req.txt_content);
    }
    if (!req.html_content.empty()) {
        append_field(body, boundary, "htmlContent", req.html_content);
    }

    for (size_t i = 0; i < req.attachments.size(); ++i) {
        const EmailAttachment &attachment = req.attachments[i];
        const std::string index = std::to_string(i);
        append_field(body, boundary, "attachments[" + index + "].name",
                     attachment.name);

        if (attachment.type == AttachmentType::File) {
            append_file_field(body, boundary,
                              "attachments[" + index + "].fileAttachment",
                              attachment.name, attachment.file_attachment);
        } else if (attachment.type == AttachmentType::Url &&
                   !attachment.url_attachment.empty()) {
            append_field(body, boundary,
                         "attachments[" + index + "].urlAttachment",
                         attachment.url_attachment);
        } else {
            return UniResponse(
                200, "{\"code\":400,\"msg\":\"attachment is empty\"}");
        }
    }

    body += "--" + boundary + "--\r\n";

    concat_header("multipart/form-data; boundary=" + boundary);

    // 发送 POST 请求
    RestClient::Response r =
        std::static_pointer_cast<RestClient::Connection>(rest)->post(
            "/v2/sendEmail", body);

    concat_header();
    return UniResponse(r.code, r.body);
}

UniResponse::UniResponse(const int code, const std::string data,
                         std::string msg)
    : code(code), msg(msg), data(data) {
    if (code != 200) {
        this->code = 400;
        this->msg = "network error";
        return;
    }
    try {
        nlohmann::json temp = nlohmann::json::parse(data);
        this->code = temp.value("code", 500);
        this->msg = temp.value("msg", std::string("invalid response"));
    } catch (...) {
        this->code = 500;
        this->msg = "invalid response";
    }
}
}; // namespace allcloud
