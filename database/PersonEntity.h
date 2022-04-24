#pragma once

#include <Poco/Data/Session.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>

namespace database
{
    class Person
    {

    public:
        Person(Poco::Data::Session ses);
        Person();
        ~Person();

        void GetLogin(std::string login);
        Poco::JSON::Array LookingForByMask(std::string first_name, std::string last_name);
        Poco::JSON::Object toJson();
        void InsertJson(std::string jstr);
        void te();
        int CalcShardNum(std::string target);
        std::string GetShardHint(size_t shard_number);
        int GetShards();

    private:
        std::string _login;
        std::string _first_name;
        std::string _last_name;
        int _age;

        Poco::Data::Session _ses;

    };
} // namespace database
