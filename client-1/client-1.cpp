//
//  client-1.cpp
//  HW10
//
//  Created by Alyaev Roman on 15.04.2021.
//

#include <iostream>
#include <string>
#include <thread>

#include <boost/asio.hpp>

#define BOOST_DATE_TIME_NO_LIB

using namespace boost::asio;

bool flag = false;

void name_tag(ip::tcp::socket & socket)
{
    std::cout << "Enter your name: ";
    
    std::string user_name;
    
    std::cin >> user_name;

    boost::asio::write(socket, boost::asio::buffer(user_name  + ": "));
}

void write_data(ip::tcp::socket & socket)
{
    std::string message = "message";
    
    std::cin >> message;
    
    if(message == "/exit")
    {
        flag = true;
    }

    write(socket, buffer(message));
}

std::string read_data(ip::tcp::socket & socket)
{
    streambuf buffer;
    
    read(socket, buffer);
    
    std::string message;
    
    std::istream input_stream(& buffer);
    
    std::getline(input_stream, message);

    if(message == "/exit")
    {
        flag = true;
    }

    return message;
}

void write_socket(ip::tcp::endpoint endpoint, ip::tcp::socket w_socket)
{
    while (flag == false)
    {
        w_socket.connect(endpoint);
        
        write_data(w_socket);
    }
}

void read_socket(ip::tcp::endpoint endpoint, ip::tcp::socket r_socket)
{
    while (flag == false)
    {
        r_socket.connect(endpoint);
        
        std::cout << read_data(r_socket) << std::endl;
    }
}

int main(int argc, const char * argv[])
{
    std::string raw_ip_address = "127.0.0.1";

    auto port = 7711;

    try
    {
        ip::tcp::endpoint endpoint(ip::address::from_string(raw_ip_address), port);

        io_service io_service;

        ip::tcp::socket w_socket(io_service, endpoint.protocol());
        ip::tcp::socket r_socket(io_service, endpoint.protocol());

        w_socket.connect(endpoint);
        name_tag(w_socket);

        std::thread write_thread = std::thread(write_socket, endpoint, w_socket);;
        std::thread read_thread = std::thread(read_socket, endpoint, r_socket);;
            
        write_thread.join();
        read_thread.join();
    }
    catch (boost::system::system_error & e)
    {
        std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

        system("pause");

        return e.code().value();
    }

    return 0;
}
