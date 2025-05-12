# unimail-cpp-sdk

unimail 的 c++ 语言 sdk, 快速集成到你的项目

[english docs](README.md)

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [unimail-cpp-sdk](#unimail-cpp-sdk)
  - [简单使用](#简单使用)
  - [api docs](#api-docs)
  - [支持的语言](#支持的语言)

<!-- /code_chunk_output -->

## 简单使用

- 安装

vcpkg

```shell
vcpkg install unimail-cpp-sdk
```

集成到 cmake 文件

```CMakeLists.txt
find_package(unimail-cpp-sdk CONFIG REQUIRED)
target_link_libraries(main PRIVATE icurve::unimail)
```

- 初始化客户端

你需要申请一个 key

main.cpp

```cpp
#include <icurve/unimail-cpp-sdk.h>

using namespace std;

int main() {
    fmt::print("hello aaa unimail-test\n");
    auto client = std::shared_ptr<icurve::UniMail>(
        new icurve::UniMail("your sdk key"));
    return 0;
}
```

- 发邮件

例如
收件人: aaa@gmail.com  
邮件标题: email subject  
邮件正文: this is a email content

```cpp
client->send_email("email", "test", "test from unimailCppSdk");
```

- 批量发送邮件

例如
收件人: aaa@gmail.com,bbb@gmail.com  
邮件标题: email subject  
邮件正文: this is a email content

```cpp
client->batch_send_email(std::vector<std::string>{"email"}, "test", "test from unimailCppSdk");
```

## api docs

. icurve::UniMail(string key)

init a client by key

2. void icurve::UniMail::set_language(std::string lang)

set language for the client,default is zh

3. bool icurve::UniMail::check_connection();

check the host and key is ok

4.  UniMailResponse icurve::UniMail::send_email(const std::string receiver,
    const std::string subject,
    const std::string content)

send email to receiver. if you have many receiver, you can concat the receiver by ";" or use BatchSendEmail

5. UniMailResponse icurve::UniMail::batch_send_email(const std::vector<std::string> receivers,
   const std::string subject,
   const std::string content);

like send_email, but receivers is a vector

## 支持的语言

sdk 默认返回的 msg 为中文

- [x] english (en)
- [x] simple chinese (zh)
- [x] vietnamese (vi)
- [x] idonesian (id)
- [x] thai (th)
- [x] gujarati (gu)

如果你需要添加了更多语言，欢迎提 issue
