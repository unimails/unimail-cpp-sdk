/* c++
 * @Author: allcloud.top
 * @Date: 2025-04-17 11:29:27
 * @Last Modified by: allcloud.top
 * @Name:
 */
#include <fmt/core.h>
#include <cassert>
#include <gtest/gtest.h>
#include "allcloud/unimail-cpp-sdk.h"

inline std::string key = "";
static allcloud::Unimail unimail(key);

// UNIMAIL
TEST(UNIMAIL_TEST1, _1) {
    std::cout << ALLCLOUD_UNIMAIL_VERSION << std::endl;
    unimail.set_language("en");
    auto res = unimail.check_connection();
    std::cout << res.to_string() << std::endl;
    assert(res.is_success());
}

TEST(UNIMAIL_TEST1, _2) {
    unimail.set_language("zh");
    allcloud::UnimailReq req;
    req.receivers.push_back("i-curve@qq.com");
    req.from = "cpp";
    // req.cc = "email";
    // req.bcc = "email";
    req.subject = "title";
    req.txt_content = "this is a email from cpp sdk.";
    req.html_content = "<h1>this is a email from cpp sdk. <span style=\"color: red\">red fond.</span></h1>";
    req.appendFile("att1.txt", "./attachment.txt");
    auto res = unimail.send_email(req);
    std::cout << res.to_string() << std::endl;
    assert(res.is_success());
    assert(!res.msg.empty());
}
