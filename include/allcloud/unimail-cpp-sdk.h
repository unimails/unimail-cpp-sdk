#pragma once
#include "allcloud/unimail-version.h"
#include <string>
#include <set>
#include <memory>
#include <vector>

namespace allcloud {
enum class AttachmentType {
    None,
    File,
    Url,
};

class UniResponse {
public:
    int code;
    std::string data;
    std::string msg;

    UniResponse(int code, std::string data, std::string msg = std::string(""));
    bool is_success() const { return code == 0; }
    std::string to_string() const {
        return "code: " + std::to_string(code) + ", msg: " + msg +
               ", data: " + data;
    }
};

class EmailAttachment {
public:
    /**
     * @brief 附件名称, 收件人显示的附件名字
     *
     */
    std::string name;
    /**
     * @brief 文件附件字节内容，url附件链接地址
     *
     */
    std::string file_attachment;
    /**
     * @brief uri附件链接地址，type为AttachmentType::Url时必填
     *
     */
    std::string url_attachment;
    AttachmentType type = AttachmentType::None;

    EmailAttachment() {}
    /**
     * @brief Construct a new Email Attachment object
     *
     * @param name 附件名称
     * @param attachment 附件内容，文件类型为文件内容，url类型为链接地址
     * @param type 附件类型:
     * 默认是文件，url类型需要设置type为AttachmentType::Url
     */
    EmailAttachment(const std::string &name, const std::string &attachment,
                    AttachmentType type = AttachmentType::File)
        : name(name), type(type) {
        if (type == AttachmentType::File) {
            file_attachment = attachment;
        } else if (type == AttachmentType::Url) {
            url_attachment = attachment;
        }
    }
};

class UnimailReq {
public:
    /**
     * @brief 发件人邮箱地址，必须是已验证的发件人地址
     *
     */
    std::string from;
    /**
     * @brief 收件人, 支持多个收件人
     *
     */
    std::vector<std::string> receivers;
    /**
     * @brief 抄送人
     *
     */
    std::string cc;
    /**
     * @brief 密送人
     *
     */
    std::string bcc;
    /**
     * @brief 主题
     *
     */
    std::string subject;
    /**
     * @brief 文本邮件, 和html_content 二选一即可, 客户优先显示html_content
     *
     */
    std::string txt_content;
    /**
     * @brief html邮件, 和txt_content 二选一即可, 客户优先显示html_content
     *
     */
    std::string html_content;
    /**
     * @brief 附件
     *
     */
    std::vector<EmailAttachment> attachments;

    /**
     * @brief 添加附件
     *
     * @param attachment 附件
     */
    void appendAttachment(const EmailAttachment &attachment);
    /**
     * @brief 添加文件附件
     *
     * @param name 附件名
     * @param path 附件文件路径
     */
    void appendFile(const std::string &name, const std::string &path);
    /**
     * @brief 添加uri附件
     *
     * @param name 附件名
     * @param uri 附件链接地址
     */
    void appendUri(const std::string &name, const std::string &uri);
};

class Unimail {
public:
    Unimail(std::string key,
            std::string host = std::string("https://uniapi.allcloud.top"));
    virtual ~Unimail();

    void set_host(std::string host);
    void set_key(std::string key);
    /**
     * @brief 设置语言，目前支持中文和英文，默认为中文
     * support list: zh, en, id, vi, th, gu
     */
    bool set_language(std::string lang);
    /**
     * 检查连接是否正常
     * @return UniResponse  .is_success() true/false
     */
    UniResponse check_connection();
    /**
     * 发送邮件
     * @param req 发送邮件请求体
     * @return UniMailResponse
     */
    UniResponse send_email(const UnimailReq &req);

private:
    std::string host;
    std::string key;
    std::string lang = "zh";
    std::shared_ptr<void> rest;
    std::set<std::string> support_lang = {"zh", "en", "id", "vi", "th", "gu"};

    void generate_rest();
    void concat_header(const std::string &content_type =
                           std::string("application/json")) const;
};
}; // namespace allcloud