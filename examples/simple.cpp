/**
 * 
 * 简单的演示样例
 * 
 */
#include <iostream>
#include <string>

#include "sigslot.h"

/** 
 * 接收端
 * slot函数（被回调的函数）就是普通的成员函数，但有以下限制：
 * 1、返回值必须为void
 * 2、Slot参数个数范围为0-8个
 * 3、实现slot的类必须继承自has_slots<>
 */
class Error: public sigslot::has_slots<>
{
public:
    void onError(std::string msg, int code)
    {
        std::cout << code << ":" << msg << std::endl;
    }
};

/** 
 * 发送端
 * 函数的返回值类型和个数必须和接收端slot函数匹配
 * 调用方可以是一般函数，也可以是类中普通成员函数
 */
sigslot::signal2<std::string, int> ReportError;

class Reporter
{
public:
    sigslot::signal2<std::string, int> reportError;
};

int main(int argc, const char *argv[]) {

    Error err;
    Reporter reporter;
    // 运行时绑定
    ReportError.connect(&err, &Error::onError);
    reporter.reportError.connect(&err, &Error::onError);

    ReportError("Everything is OK", 0);
    ReportError("Something is Wrong", 1);

    reporter.reportError("Everything is OK", 0);
    reporter.reportError("Something is Wrong", 1);

    // 解绑
    ReportError.disconnect(&err);

    // 当发送一个没有连接的信号时，不做任何处理，也不会有错误发出
    ReportError("Everything is OK", 0);
    ReportError("Something is Wrong", 1);

    return 0;
}