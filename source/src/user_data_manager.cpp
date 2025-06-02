#include <iostream>
#include <fstream>
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include "log.h"
#include "global.h"
#include "user_data_manager.h"
#include "money.h"

namespace Didi
{
    UserDataManager::UserDataManager()
    {
        //m_user_data_map.clear();
        // USER_DATA administrator_data{ "admin", 1000000 };
        // m_user_data_map["admin"] = administrator_data;
    }

    UserDataManager* UserDataManager::Instance()
    {
        static UserDataManager instance;
        return &instance;
    }

    UserDataManager::~UserDataManager()
    {

    }

    bool UserDataManager::SaveUserData()
    {
        // 创建一个 JSON 文档
        QString user_data_folder = USER_DATA_FOLDER_NAME;
        QString user_data_file = USER_DATA_FILE_NAME;
        QString user_data_file_path = QString(QApplication::instance()->applicationDirPath() + "/" + user_data_folder + "/" + user_data_file);
        
        rapidjson::Document json_doc;
        json_doc.SetObject();

        if(!m_user_data_map.empty())
        {
            // 创建一个数组，存放多组信息
            rapidjson::Value infoArray(rapidjson::kArrayType);

            // 遍历m_user_data_map将成员变量添加到 JSON 文档中
            for(auto it = m_user_data_map.begin(); it!= m_user_data_map.end(); ++it)
            {
                rapidjson::Value info(rapidjson::kObjectType);  // 创建一个对象
                
                std::string user_name_str = it->first.toStdString();
                std::string password_str = it->second.password.toStdString();
                std::string money_str = it->second.money.toStdString();
                rapidjson::Value user_name(user_name_str.c_str(), user_name_str.length(), json_doc.GetAllocator());
                rapidjson::Value password(password_str.c_str(), password_str.length(), json_doc.GetAllocator());
                rapidjson::Value money(money_str.c_str(), money_str.length(), json_doc.GetAllocator());
                info.AddMember("用户名", user_name, json_doc.GetAllocator());
                info.AddMember("密码", password, json_doc.GetAllocator());
                info.AddMember("金额", money, json_doc.GetAllocator());
                infoArray.PushBack(info, json_doc.GetAllocator());
            }

            json_doc.AddMember("用户信息", infoArray, json_doc.GetAllocator());
        }
 
        // 创建一个内存缓冲区
        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        json_doc.Accept(writer);
		
        std::ofstream outFile(user_data_file_path.toStdString());
        if (!outFile.is_open()) 
        {
            return false; // 文件打开失败
        }

        outFile << buffer.GetString();
        outFile.close();
        return true;    
    }

    bool UserDataManager::LoadUserData()
    {
        QLOG_INFO() << "尝试载入用户信息...";

        // 检查设置目录是否存在，不存在则创建
        QString strDataDir = USER_DATA_FOLDER_NAME;
        QDir dir(QApplication::instance()->applicationDirPath() + "/" + strDataDir);
        if (!dir.exists())
        {
            QLOG_INFO() << "用户数据目录不存在，创建目录";
            dir.mkpath(".");
        }

        // 检查USER_DATA_FILE_NAME文件是否存在，不存在则创建
        QString strUserDataFile = QApplication::instance()->applicationDirPath() + "/" + strDataDir + "/" + USER_DATA_FILE_NAME;
        if (!QFile::exists(strUserDataFile))
        {
            QLOG_INFO() << "用户数据文件不存在，创建文件";
            QFile file(strUserDataFile);
            if (file.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&file);
                stream << "{\n}";
                file.close();
            }
            return SaveUserData();
        }
        else
        {
            // 尝试解析用户文件
            std::ifstream inFile(strUserDataFile.toStdString());
            if (!inFile.is_open()) 
            {
                QLOG_WARN() << "用户文件打开失败: " << strUserDataFile;
                return false; // 文件打开失败
            }

            // 读取文件内容到字符串
            std::string json_content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
            inFile.close();

            // 解析 JSON 内容
            rapidjson::Document json_doc;
            json_doc.Parse(json_content.c_str());

            if (json_doc.HasParseError())
            {
                QLOG_WARN() << "文件解析失败";
                return false; // JSON 解析失败
            }

            if (json_doc.HasMember("用户信息") && json_doc["用户信息"].IsArray())
            {
                const rapidjson::Value& infoArray = json_doc["用户信息"];
                std::string user_name_str;
                std::string password_str;
                std::string money_str;
                for(int i = 0; i < json_doc["用户信息"].Size(); i++)
                {
                    const rapidjson::Value& info = infoArray[i];

                    if((info.HasMember("用户名") && info["用户名"].IsString()) &&
                        (info.HasMember("密码") && info["密码"].IsString()) &&
                        (info.HasMember("金额") && info["金额"].IsString()))
                    {
                        user_name_str = info["用户名"].GetString();
                        password_str = info["密码"].GetString();
                        money_str = info["金额"].GetString();
                        Money money = Money::fromStdString(money_str);
                        auto user_data = USER_DATA(QString::fromStdString(password_str), money);
                        m_user_data_map[QString::fromStdString(user_name_str)] = USER_DATA(QString::fromStdString(password_str), money);
                    }
                    else
                    {
                        QLOG_WARN() << "用户信息格式错误";
                        QMessageBox::information(nullptr, "错误", "用户信息格式错误");
                        return false;
                    }
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    bool UserDataManager::AddUserData(const QString& user_name, const USER_DATA& user_data)
    {
        m_user_data_map[user_name] = user_data;
        return SaveUserData();
    }

    bool UserDataManager::RemoveUserData(const QString& user_name)
    {
        if (m_user_data_map.find(user_name) == m_user_data_map.end())
        {
            return false;
        }
        m_user_data_map.erase(user_name);
        return SaveUserData();
    }

    bool UserDataManager::IsExist(const QString& user_name)
    {
        if (m_user_data_map.find(user_name) == m_user_data_map.end())
        {
            return false;
        }
        return true;
    }

    USER_DATA UserDataManager::GetUserData(const QString& user_name)
    {
        if (m_user_data_map.find(user_name) == m_user_data_map.end())
        {
            QLOG_WARN() << "用户" << user_name << "不存在";
            return USER_DATA();
        }
        return m_user_data_map[user_name];
    }

    bool UserDataManager::UpdateUserData(const QString& user_name, const USER_DATA& user_data)
    {
        if (m_user_data_map.find(user_name) == m_user_data_map.end())
        {
            return false;
        }
        m_user_data_map[user_name] = user_data;
        return SaveUserData();
    }
}
