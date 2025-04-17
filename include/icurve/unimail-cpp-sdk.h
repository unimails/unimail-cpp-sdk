#pragma once
#include <string>
#include <set>
#include <vector>
#include <memory>

namespace icurve {
class UniMailResponse {
public:
    int code;
    std::string data;
    std::string msg;

    UniMailResponse(int code, std::string data);
    bool is_success() { return code == 0; }
};

class UniMail {
public:
    UniMail(std::string key, std::string host = std::string(
                                 "https://unimail-back.allcloud.top"));
    ~UniMail();

    void set_host(std::string host);
    void set_key(std::string key);
    /**
     * 设置语言，目前支持中文和英文，默认为中文
     * support list: zh, en, id, vi, th, gu
     */
    bool set_language(std::string lang);
    /**
     * 检查连接是否正常
     * @return true if connection is ok, false otherwise
     */
    bool check_connection();
    /**
     * 发送邮件
     * @param receiver 接收者邮箱
     * @param subject 邮件主题
     * @param content 邮件内容
     * @return UniMailResponse
     */
    UniMailResponse send_email(const std::string receiver,
                               const std::string subject,
                               const std::string content);
    /**
     * 批量发送邮件
     * @param receivers 接收者邮箱列表
     * @param subject 邮件主题
     * @param content 邮件内容
     * @return UniMailResponse
     */
    UniMailResponse batch_send_email(const std::vector<std::string> receivers,
                                     const std::string subject,
                                     const std::string content);

private:
    std::string host;
    std::string key;
    std::string lang = "zh";
    std::shared_ptr<void> rest;
    std::set<std::string> support_lang = {"zh", "en", "id", "vi", "th", "gu"};

    void generate_rest();
    void concat_header();
};
}; // namespace icurve