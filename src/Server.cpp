#include "Server.h"

#include "handlers/Factory.h"

#include <iostream>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServerParams.h>

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionPool.h>

#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>

#include <Poco/Data/RecordSet.h>
#include <Poco/Data/MySQL/MySQLException.h>

Server::Server() : _helpRequested(false)
{
}

Server::~Server()
{
}

void Server::initialize(Application &self)
{
	loadConfiguration();
	ServerApplication::initialize(self);
}

void Server::uninitialize()
{
	ServerApplication::uninitialize();
}

void Server::defineOptions(Poco::Util::OptionSet &options)
{
	ServerApplication::defineOptions(options);

	options.addOption(
		Poco::Util::Option("host", "h", "set ip address for dtabase")
			.required(false)
			.repeatable(false)
			.argument("value")
			.callback(Poco::Util::OptionCallback<Server>(this, &Server::handleHost)));
	options.addOption(
            Poco::Util::Option("port", "po", "set mysql port")
                .required(false)
                .repeatable(false)
                .argument("value")
                .callback(Poco::Util::OptionCallback<Server>(this, &Server::handlePort)));
        options.addOption(
            Poco::Util::Option("login", "lg", "set mysql login")
                .required(false)
                .repeatable(false)
                .argument("value")
                .callback(Poco::Util::OptionCallback<Server>(this, &Server::handleLogin)));
        options.addOption(
            Poco::Util::Option("password", "pw", "set mysql password")
                .required(false)
                .repeatable(false)
                .argument("value")
                .callback(Poco::Util::OptionCallback<Server>(this, &Server::handlePassword)));
        options.addOption(
            Poco::Util::Option("database", "db", "set mysql database")
                .required(false)
                .repeatable(false)
                .argument("value")
                .callback(Poco::Util::OptionCallback<Server>(this, &Server::handleDatabase)));
}

void Server::handleHost([[maybe_unused]] const std::string &name, [[maybe_unused]] const std::string &value)
{
	std::cout << "host:" << value << std::endl;
	_host = value;
	// Config::get().host() = value;
}
void Server::handleLogin([[maybe_unused]] const std::string &name, [[maybe_unused]] const std::string &value)
{
	std::cout << "login:" << value << std::endl;
	_login = value;
	// Config::get().host() = value;
}
void Server::handlePassword([[maybe_unused]] const std::string &name, [[maybe_unused]] const std::string &value)
{
	std::cout << "password:" << value << std::endl;
	_password = value;
	// Config::get().host() = value;
}
void Server::handleDatabase([[maybe_unused]] const std::string &name, [[maybe_unused]] const std::string &value)
{
	std::cout << "database:" << value << std::endl;
	_database = value;
	// Config::get().host() = value;
}
void Server::handlePort([[maybe_unused]] const std::string &name, [[maybe_unused]] const std::string &value)
{
	std::cout << "port:" << value << std::endl;
	_port = value;
	// Config::get().host() = value;
}

int Server::main([[maybe_unused]] const std::vector<std::string> &args)
{
	if (!_helpRequested)
	{
		unsigned short port = (unsigned short)config().getInt("HTTPWebServer.port", 8080);

		Poco::Data::MySQL::Connector::registerConnector();
		//std::format()
		std::stringstream ss;
		ss << "host=" << "127.0.0.1" << ";"
			<< "port=" << "6033" << ";"
			<< "db=" << _database << ";"
			<< "user=" << _login << ";"
			<< "password=" << _password << ";";

		std::string connection = ss.str() + "compress=true;auto-reconnect=true";

		std::cout << connection << std::endl;

        //Poco::Data::SessionPool pool("MySQL", "host=localhost;port=3306;db=stud;user=person;password=1234;compress=true;auto-reconnect=true");
       	Poco::Data::SessionPool pool("MySQL", connection);

		Poco::Data::Session _ses(pool.get());
		
		Poco::Data::Statement select(_ses);

		// std::string l = "biba";
		// std::string _l;

		// select << "SELECT * FROM Person WHERE login=?",
        //     Poco::Data::Keywords::into(_l),
        //     Poco::Data::Keywords::use(l),
        //     Poco::Data::Keywords::now;
        
        // select.execute();
        
        // Poco::Data::RecordSet rs(select);

        // bool first_row = rs.moveFirst();

        // if(!first_row)
        //     throw Poco::Data::MySQL::StatementException("row is empty");

		//std::cout << _l << std::endl;

		Poco::Net::ServerSocket svs(Poco::Net::SocketAddress("0.0.0.0", port));
		Poco::Net::HTTPServer srv(new handlers::Factory(pool), svs, new Poco::Net::HTTPServerParams);

		std::cout << "Started server on port: 8080" << std::endl;
		srv.start();

		waitForTerminationRequest();
		srv.stop();
	}
	return Application::EXIT_OK;
}
//sudo ./build/hl_mai_lab_01 --host=localhost --port=3306 --login=stud --password=stud --database=stud
