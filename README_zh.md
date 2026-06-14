# unimail-cpp-sdk

> 当前sdk的版本是v2, 如果你需要用以前的v1版本, 请切换v1分支

unimail 的 c++ 语言 sdk, 快速集成到你的项目

[english docs](README.md)

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [unimail-cpp-sdk](#unimail-cpp-sdk)
  - [使用](#使用)
  - [api docs](#api-docs)
  - [支持的语言](#支持的语言)

<!-- /code_chunk_output -->

## 使用

- 安装

vcpkg

```shell
vcpkg install unimail-cpp-sdk
```

集成到 cmake 文件

```CMakeLists.txt
find_package(unimail-cpp-sdk CONFIG REQUIRED)
target_link_libraries(main PRIVATE allcloud::unimail)
```

- 初始化客户端

你需要申请一个 key

main.cpp

```cpp
#include <allcloud/unimail-cpp-sdk.h>

using namespace std;

int main() {
    fmt::print("hello aaa unimail-test\n");
    auto client = std::shared_ptr<allcloud::Unimail>(
        new allcloud::Unimail("your sdk key"));
    auto res = client.check_connection();
    std::cout << res.to_string() << std::endl;
    assert(res.is_success());
    return 0;
}
```

- 发邮件

```cpp
    unimail.set_language("zh");
    allcloud::UnimailReq req;
    req.receivers.push_back("receive email");
    req.from = "cpp"; // 可选项
    // req.cc = "email";
    // req.bcc = "email";
    req.subject = "title";
    // txt_content 和 html_content 二选一
    req.txt_content = "this is a email from cpp sdk.";
    req.html_content = "<h1>this is a email from cpp sdk. <span style=\"color: red\">red fond.</span></h1>";
    // 添加那文件附件
    req.appendFile("att1.txt", "./attachment.txt");
    auto res = unimail.send_email(req);
    std::cout << res.to_string() << std::endl;
    assert(res.is_success());
    assert(!res.msg.empty());
```

## api docs

. allcloud::UniMail(string key)

init a client by key

2. void allcloud::UniMail::set_language(std::string lang)

set language for the client,default is zh

3. bool allcloud::UniMail::check_connection();

check the host and key is ok

4.  UniMailResponse allcloud::UniMail::send_email(const UnimailReq &req)

please see usage

## 支持的语言

sdk 默认返回的 msg 为中文

- [x] english (en)
- [x] simple chinese (zh)
- [x] vietnamese (vi)
- [x] idonesian (id)
- [x] thai (th)
- [x] gujarati (gu)

如果你需要添加了更多语言，欢迎提 issue

- 提示

如果想要使用 unimail sdk, 请通过邮件联系作者 admin@allcloud.top
