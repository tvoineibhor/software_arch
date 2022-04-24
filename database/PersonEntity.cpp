#include "PersonEntity.h"

#include <Poco/Data/RecordSet.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <iostream>

using namespace Poco::Data::Keywords;

namespace database
{   
    Person::Person(Poco::Data::Session ses) : _ses(ses)
    {
    }
    
    Person::~Person()
    {
    }

    int Person::GetShards() 
    {
		return 2;
	}

    int Person::CalcShardNum(std::string target)
    {
        return std::hash<std::string>{}(target) % GetShards();
    }

    std::string Person::GetShardHint(size_t shard_number)
    {
        std::string result = "-- sharding:";
        result += std::to_string(shard_number);

        return result;
    }

    Poco::JSON::Object Person::toJson()
    {
        Poco::JSON::Object jobj;
        jobj.set("login", _login);
        jobj.set("first_name", _first_name);
        jobj.set("last_name", _last_name);
        jobj.set("age", _age);

        return jobj;

        //Poco::JSON::Stringifier::stringify(obj, out);
    }
    
    void Person::GetLogin(std::string login)
    {
        Poco::Data::Statement select(_ses);

        std::string sharding_hint = GetShardHint(CalcShardNum(login));

        select << "SELECT * FROM Person WHERE login=?" + sharding_hint,
            into(_login),
            into(_first_name),
            into(_last_name),
            into(_age),
            use(login),
            now;
        
        select.execute();
        
        Poco::Data::RecordSet rs(select);

        bool first_row = rs.moveFirst();

        if(!first_row)
            throw Poco::Data::MySQL::StatementException("row is empty");
    }

    Poco::JSON::Array Person::LookingForByMask(std::string first_name, std::string last_name)
    {
        Poco::Data::Statement select(_ses);

        Poco::JSON::Array jarr;
        
        first_name += "%";
        last_name += "%";

        for (int shard = 0; shard < 2; shard++)
        {
            std::string sharding_hint = GetShardHint(shard);

            select << "SELECT * FROM Person where first_name like ? and last_name like ?" + sharding_hint,
            into(_login),
            into(_first_name),
            into(_last_name),
            into(_age),
            use(first_name),
            use(last_name),
            range(0, 1);

            while (!select.done())
            {
                select.execute();

                Poco::Data::RecordSet rs(select);

                bool first_row = rs.moveFirst();

                if(!first_row)
                    throw Poco::Data::MySQL::StatementException("row is empty");

                jarr.add(Person::toJson());
            }
        }
        return jarr;
    }

    void Person::InsertJson(std::string jstr)
    {
        std::cout << jstr << std::endl;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(jstr);

        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        _login = object->getValue<std::string>("login");
        _first_name = object->getValue<std::string>("first_name");
        _last_name = object->getValue<std::string>("last_name");
        _age = object->getValue<int>("age");
        
        Poco::Data::Statement insert(_ses);

        std::string sharding_hint = GetShardHint(CalcShardNum(_login));
        
        insert << "INSERT INTO Person (login, first_name, last_name, age) VALUES (?, ?, ?, ?)" + sharding_hint,
            use(_login),
            use(_first_name),
            use(_last_name),
            use(_age);
        
        insert.execute();
    }
    
} // namespace database
