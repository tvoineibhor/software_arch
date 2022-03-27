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

        void GetLogin(int login);
        Poco::JSON::Array LookingFor(std::string first_name, std::string last_name);
        Poco::JSON::Object toJson();
        void InsertJson(std::string jstr);
        void te();

    private:
        int _login;
        std::string _first_name;
        std::string _last_name;
        int _age;

        Poco::Data::Session _ses;

    };
} // namespace database
