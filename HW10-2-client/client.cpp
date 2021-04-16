//
//  client.cpp
//  HW10-2-client
//
//  Created by Alyaev Roman on 15.04.2021.
//

#include <iostream>
#include <string>

#include <boost/asio.hpp>

#define BOOST_DATE_TIME_NO_LIB

void write_data(boost::asio::ip::tcp::socket & socket, std::string user_name)
{
    std::string message = "/exit";
    
    do
    {
        boost::asio::write(socket, boost::asio::buffer(user_name + ": " + message));
    }
    while(message != "/exit");
}

int main(int argc, char ** argv)
{
    std::string raw_ip_address = "127.0.0.1";
    
    std::string user_name = "user";
    
    std::cout << "Enter your name: ";
    
    std::cin >> user_name;
    
    std::cout << "Enter IP: ";
    
    std::cin >> raw_ip_address;

    auto port = 7711;
    
    std::cout << "Port: " << port << std::endl;

    try
    {
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::address::from_string(raw_ip_address), port);

        boost::asio::io_service io_service;

        boost::asio::ip::tcp::socket socket(io_service, endpoint.protocol());

        socket.connect(endpoint);

        write_data(socket, user_name);
    }
    catch (boost::system::system_error & e)
    {
        std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

        system("pause");

        return e.code().value();
    }

    system("pause");

    return EXIT_SUCCESS;
}
