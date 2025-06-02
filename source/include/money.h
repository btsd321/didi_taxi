//***************************************************
//*******Copyright (C),2019-2099,comen.Co.,Ltd*******
//*
//*	文件名			：	money.h
//*	文件功能		：	金额类头文件
//*	编程者			：	XinhaoLi
//*	日期			：	2025.05.30
//*	编号---作者---------日期----注释
//*	00	XinhaoLi	2025.05.30	创建
//*	说明：金额类头文件
//***************************************************
#ifndef MONEY_H
#define MONEY_H

#include <iostream>
#include <QString>

namespace Didi
{
    class Money
    {
    public:
        //=============================================================
        // Function：Money::Money
        // Descripetion：Money类构造函数，从元为单位的字符串构造Money对象
        // Input：str：金额字符串
        // Output：无
        // Return：无
        // Others：无
        //=============================================================
        Money(QString str);
        //=============================================================
        // Function：Money::Money
        // Descripetion：拷贝构造函数
        // Input：other：拷贝对象
        // Output：无
        // Return：无
        // Others：无
        //=============================================================
        Money(const Money& other);
        //=============================================================
        // Function：Money::toQString
        // Descripetion：将金额转换成QString字符串
        // Input：无
        // Output：无
        // Return：QString: 输出的Qstring字符串，单位为元
        // Others：无
        //=============================================================
        QString toQString() const;
        //=============================================================
        // Function：Money::toQreal
        // Descripetion：硬件延时
        // Input：s32Ms：需要延时的时间，单位：us
        // Output：无
        // Return：无
        // Others：无
        //=============================================================
        qreal toQreal() const;// 返回单位为元的金额浮点数值
        //=============================================================
        // Function：BspDelayUs
        // Descripetion：硬件延时
        // Input：s32Ms：需要延时的时间，单位：us
        // Output：无
        // Return：无
        // Others：无
        //=============================================================
        std::string toStdString() const;//返回单位为元的金额字符串
        //=============================================================
        // Function：BspDelayUs
        // Descripetion：硬件延时
        // Input：s32Ms：需要延时的时间，单位：us
        // Output：无
        // Return：无
        // Others：无
        //=============================================================
        int64_t toInt() const;//返回单位为分的金额整数值
        //=============================================================
        // Function：BspDelayUs
        // Descripetion：硬件延时
        // Input：s32Ms：需要延时的时间，单位：us
        // Output：无
        // Return：无
        // Others：无
        //=============================================================
        static Money fromQString(const QString& str);//从元为单位的字符串构造Money对象
        //=============================================================
        // Function：BspDelayUs
        // Descripetion：硬件延时
        // Input：s32Ms：需要延时的时间，单位：us
        // Output：无
        // Return：无
        // Others：无
        //=============================================================
        static Money fromStdString(const std::string& str);//从元为单位的字符串构造Money对象
        //=============================================================
        // Function：BspDelayUs
        // Descripetion：硬件延时
        // Input：s32Ms：需要延时的时间，单位：us
        // Output：无
        // Return：无
        // Others：无
        //=============================================================
        static Money fromInt(int64_t amount);//从分为单位的金额整数构造Money对象

        Money operator*(int64_t) const;
        // 删除或禁用乘法操作符
        Money operator*(const Money&) const = delete;
        
        Money operator*(QString) const = delete;

        // 删除或禁用除法操作符
        Money operator/(const Money&) const = delete;
        Money operator/(qreal) const = delete;
        Money operator/(QString) const = delete;

        Money operator+(const Money&) const;
        Money operator-(const Money&) const;
        Money& operator+=(const Money&);
        Money& operator-=(const Money&);

        bool operator<(const Money&) const;
        bool operator<=(const Money&) const;
        bool operator>(const Money&) const;
        bool operator>=(const Money&) const;

        bool operator==(const Money&) const;
        bool operator!=(const Money&) const;

        static bool isValid(qreal);//判断金额是否有效，输入数字单位为元
        static bool isValid(QString);//判断金额是否有效，输入数字单位为元
        static bool isValid(std::string);//判断金额是否有效，输入数字单位为元

    private:
        Money(int64_t amount = 0);
        int64_t m_fen; // 金额，单位：分
    };

}

#endif // MONEY_H
