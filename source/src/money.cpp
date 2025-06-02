#include <regex>
#include "money.h"
#include "log.h"

namespace Didi
{
    static bool is_number(const std::string &str)
    {
        int dot_count = 0;
        for(char c : str)
        {
            if (c == '.')
            {
                dot_count++;
                if (dot_count > 1)
                {
                    return false;
                }
            }
            if ((!isdigit(c)) && (c != '.'))
            {
                return false;
            }
        }
        return true;
    }

    static std::string money_str_preprocess(const std::string &str)
    {
        std::string result = str;
        // 检查字符串是否以小数点结尾
        if (!result.empty() && result.back() == '.')
        {
            result.pop_back();
            return result;
        }

        // 检查是否有小数点
        size_t dotPos = result.find('.');
        if (dotPos != std::string::npos)
        {
            // 检查字符串末尾是否有0
            if (!result.empty() && result.back() == '0')
            {
                // 去除末尾的0
                size_t lastNonZeroPos = result.find_last_not_of('0');
                if (lastNonZeroPos != std::string::npos)
                {
                    result.erase(lastNonZeroPos + 1, std::string::npos);
                    // 如果小数点后面没有数字，去除小数点
                    if (lastNonZeroPos > 0 && result[lastNonZeroPos] == '.')
                    {
                        result.pop_back();
                    }
                }
            }
        }
        return result;
    }

    static int cal_decimal_places_without_preprocess(const std::string &str)
    {
        size_t dotPos = str.find('.');
        if (dotPos != std::string::npos)
        {
            return str.size() - dotPos - 1;
        }
        else
        {
            return 0;
        }
    }

    static int cal_decimal_places(const std::string &str)
    {
        std::string result = money_str_preprocess(str);
        return cal_decimal_places_without_preprocess(result);
    }

    static int64_t str_to_fen(const std::string &str)
    {
        std::string processed_str = money_str_preprocess(str);
        size_t dot_pos = str.find('.');
        std::string integer_part, fractional_part;

        // 分割整数部分和小数部分
        if (dot_pos != std::string::npos)
        {
            integer_part = str.substr(0, dot_pos);
            fractional_part = str.substr(dot_pos + 1);

            // 截断超过两位的小数（假设输入合法，最多两位）
            if (fractional_part.length() > 2)
            {
                fractional_part = fractional_part.substr(0, 2);
            }
            // 不足两位则补零
            else if (fractional_part.length() < 2)
            {
                fractional_part.append(2 - fractional_part.length(), '0');
            }
        }
        else
        {
            integer_part = str;
            fractional_part = "00";
        }

        // 处理空整数部分（如 ".99" -> 视为 "0.99"）
        if (integer_part.empty() || integer_part == "-" || integer_part == "+")
        {
            integer_part += "0";
        }

        // 合并整数和小数部分，转换为长整型
        std::string combined = integer_part + fractional_part;
        return stoll(combined);
    }

    Money::Money(int64_t amount) : m_fen(amount)
    {
    }

    Money::Money(QString str)
    {
        if (!isValid(str))
        {
            QLOG_ERROR() << "Money format error: " << str;
        }
        m_fen = str_to_fen(str.toStdString());
    }

    Money Money::operator+(const Money &other) const
    {
        return Money(m_fen + other.m_fen);
    }

    Money Money::operator-(const Money &other) const
    {
        return Money(m_fen - other.m_fen);
    }

    Money &Money::operator+=(const Money &other)
    {
        m_fen += other.m_fen;
        return *this;
    }

    Money &Money::operator-=(const Money &other)
    {
        m_fen -= other.m_fen;
        return *this;
    }

    Money::Money(const Money &other)
    {
        m_fen = other.m_fen;
    }

    bool Money::operator==(const Money &other) const
    {
        return m_fen == other.m_fen;
    }

    bool Money::operator!=(const Money &other) const
    {
        return m_fen != other.m_fen;
    }

    bool Money::operator>(const Money &other) const
    {
        return m_fen > other.m_fen;
    }

    bool Money::operator>=(const Money &other) const
    {
        return m_fen >= other.m_fen;
    }

    bool Money::operator<(const Money &other) const
    {
        return m_fen < other.m_fen;
    }

    bool Money::operator<=(const Money &other) const
    {
        return m_fen <= other.m_fen;
    }

    Money Money::fromQString(const QString &str)
    {
        // 检查字符串是否合法
        bool ok = false;
        double d = str.toDouble(&ok);
        if (!ok)
        {
            // 格式不正常
            QLOG_FATAL() << "Money format error: " << str;
            return Money(0);
        }
        else
        {
            // 判断d *100 是否为整数
            if ((d * 100) != (int64_t)(d * 100))
            {
                QLOG_FATAL() << "Money format error: " << str;
                return Money(0);
            }

            return Money((int64_t)(d * 100));
        }
    }

    Money Money::fromStdString(const std::string &str)
    {
        if (!isValid(str))
        {
            QLOG_ERROR() << "Money format error: " << str;
        }
        return Money(str_to_fen(str));
    }

    Money Money::fromInt(int64_t amount)
    {
        return Money(amount);
    }

    QString Money::toQString() const
    {
        return QString::number(m_fen / 100.0, 'f', 2);
    }

    std::string Money::toStdString() const
    {
        return toQString().toStdString();
    }

    int64_t Money::toInt() const
    {
        return m_fen; // 返回单位为分的金额整数值
    }

    qreal Money::toQreal() const
    {
        return m_fen / 100;
    }

    bool Money::isValid(qreal input)
    {
        // 判断金额是否有效，输入参数为单位为元的数字
        return ((input * 100) == (int64_t)(input * 100));
    }

    Money Money::operator*(int64_t input) const
    {
        // 金额乘以整数，返回单位为分的金额整数值
        return Money(m_fen * input);
    }

    bool Money::isValid(QString str)
    {
        std::string str_std = str.toStdString();
        return isValid(str_std);
    }

    bool Money::isValid(std::string str)
    {
        if(!is_number(str))
        {
            return false;
        }
        // 判断小数位数是否大于2
        if (cal_decimal_places(str) > 2)
        {
            return false;
        }
        return true;
    }
}
