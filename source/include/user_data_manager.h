#ifndef USER_DATA_MANAGER_H
#define USER_DATA_MANAGER_H

#include <unordered_map>
#include <QString>
#include "money.h"

namespace Didi
{
    typedef struct USER_DATA
    {
        QString password;
        Money money;  // 用户现有金额，单位：分

        USER_DATA() : money(Money("0"))
        {
            password = "";
        }

        USER_DATA(const QString& password, Money money) : money(money)
        {
            this->password = password;
        }

        bool operator!=(const USER_DATA &other) const
        {
            // 实现比较逻辑
            return  ((this->password != other.password) || (this->money != other.money));
        }

        bool operator==(const USER_DATA &other) const
        {
            // 实现比较逻辑
            return ((this->password == other.password) && (this->money != other.money));
        }
        
    } USER_DATA;


    class UserDataManager
    {
    public:
        static UserDataManager* Instance();
        ~UserDataManager();

        bool LoadUserData();
        bool AddUserData(const QString& user_name, const USER_DATA& user_data);
        bool SaveUserData();
        bool RemoveUserData(const QString& user_name);
        bool IsExist(const QString& user_name);
        USER_DATA GetUserData(const QString& user_name);
        bool UpdateUserData(const QString& user_name, const USER_DATA& user_data);
        
    private:
        UserDataManager();

        std::unordered_map<QString, USER_DATA> m_user_data_map;
    };

    
}


#endif
