# unimail-cpp-sdk

> The current SDK version is v2. If you need to use the previous v1 version, please switch to the v1 branch.

This is a C++ SDK for Unimail. Quickly integrate into your project

[中文文档](README_zh.md)

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [unimail-cpp-sdk](#unimail-cpp-sdk)
  - [simple usage](#simple-usage)
  - [api docs](#api-docs)
  - [support language](#support-language)

<!-- /code_chunk_output -->

## simple usage

- install

vcpkg

```shell
vcpkg install unimail-cpp-sdk
```

link in you cmake

```CMakeLists.txt
find_package(unimail-cpp-sdk CONFIG REQUIRED)
target_link_libraries(main PRIVATE icurve::unimail)
```

- init a unimail client

you need a authorization key.

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

- send email

example
receiver: aaa@gmail.com  
email subject: email subject  
email content: this is a email content

```cpp
client->send_email("email", "test", "test from unimailCppSdk");
```

- batch send email

example
receivers: aaa@gmail.com,bbb@gmail.com  
email subject: email subject  
email content: this is a email content

```cpp
client->batch_send_email(std::vector<std::string>{"email"}, "test", "test from unimailCppSdk");
```

## api docs

1. icurve::UniMail(string key)

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

## support language

chinese is the default language for the sdk.

- [x] english (en)
- [x] simple chinese (zh)
- [x] vietnamese (vi)
- [x] idonesian (id)
- [x] thai (th)
- [x] gujarati (gu)

if you want to support other language, please open a issue.

- tips

If you want to use this SDK, please contact the author via admin@allcloud.top
