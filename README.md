# unimail-cpp-sdk

> The current SDK version is v2. If you need to use the previous v1 version, please switch to the v1 branch.

This is a C++ SDK for Unimail. Quickly integrate into your project

[中文文档](README_zh.md)

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [unimail-cpp-sdk](#unimail-cpp-sdk)
  - [usage](#usage)
  - [api docs](#api-docs)
  - [support language](#support-language)

<!-- /code_chunk_output -->

## usage

- install

vcpkg

```shell
vcpkg install unimail-cpp-sdk
```

link in you cmake

```CMakeLists.txt
find_package(unimail-cpp-sdk CONFIG REQUIRED)
target_link_libraries(main PRIVATE allcloud::unimail)
```

- init a unimail client

you need a authorization key.

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

- send email

```cpp
    unimail.set_language("zh");
    allcloud::UnimailReq req;
    req.receivers.push_back("receive email");
    req.from = "cpp"; // options
    // req.cc = "email";
    // req.bcc = "email";
    req.subject = "title";
    // Either txt_content or html_content
    req.txt_content = "this is a email from cpp sdk.";
    req.html_content = "<h1>this is a email from cpp sdk. <span style=\"color: red\">red fond.</span></h1>";
    // add file attachment
    req.appendFile("att1.txt", "./attachment.txt");
    auto res = unimail.send_email(req);
    std::cout << res.to_string() << std::endl;
    assert(res.is_success());
    assert(!res.msg.empty());
```

## api docs

1. allcloud::UniMail(string key)

init a client by key

2. void allcloud::UniMail::set_language(std::string lang)

set language for the client,default is zh

3. bool allcloud::UniMail::check_connection();

check the host and key is ok

4.  UniMailResponse allcloud::UniMail::send_email(const UnimailReq &req)

please see usage

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
