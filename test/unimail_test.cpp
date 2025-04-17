/* c++
 * @Author: i-curve
 * @Date: 2025-04-17 11:29:27
 * @Last Modified by: i-curve
 * @Name:
 */
#include <fmt/core.h>
#include <cassert>
#include <gtest/gtest.h>
#include "icurve/unimail-cpp-sdk.h"

inline std::string key = "";
static icurve::UniMail unimail(key);

// UNIMAIL
TEST(UNIMAIL_TEST1, _1) {
    unimail.set_language("en");
    auto res = unimail.check_connection();
    assert(res);
}

TEST(UNIMAIL_TEST1, _2) {
    unimail.set_language("zh");
    auto res = unimail.send_email("i-curve@qq.com", "title",
                                  "this is a email from cpp sdk.");
    std::cout << res.msg << std::endl;
    assert(res.is_success());
    assert(!res.msg.empty());
}

TEST(UNIMAIL_TEST1, _3) {
    unimail.set_language("zh");
    auto res = unimail.batch_send_email(
        std::vector<std::string>{"i-curve@qq.com", "i_curve@qq.com"}, "title",
        "this is a email from cpp batch sdk.");
    std::cout << res.msg << std::endl;
    assert(res.is_success());
    assert(!res.msg.empty());
}