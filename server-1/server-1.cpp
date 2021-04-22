//
//  server-1.cpp
//  HW10
//
//  Created by Alyaev Roman on 15.04.2021.
//

#include <iostream>
#include <string>
#include <thread>
#include <mutex>

#include <boost/asio.hpp>

#define BOOST_DATE_TIME_NO_LIB

using namespace boost::asio;

std::mutex g_mutex;

void write_data(std::string user_name, ip::tcp::socket & socket_for_name, ip::tcp::socket & socket_for_message)
{
    std::string message = "message";
    
    bool flag_1 = false;
    
    while(flag_1 == false)
    {
        std::getline(std::cin, message);
        
        if(message == "/exit")
        {
            flag_1 = true;
        }
            
        write(socket_for_name, buffer(user_name + ':'));
            
        write(socket_for_message, buffer(message + '#'));
    }
}

void read_data(std::string own_name, ip::tcp::socket & socket_for_name, ip::tcp::socket & socket_for_message)
{
    bool flag_2 = false;
    
    while(flag_2 == false)
    {
        bool flag = false;
        
        streambuf buffer_for_name;
        
        read_until(socket_for_name, buffer_for_name, ':');
        
        std::string user_name;
        
        std::istream i_stream(& buffer_for_name);
        
        std::getline(i_stream, user_name, ':');
        
        if(own_name != user_name)
        {
            flag = true;
        }
        
        if(flag == true)
        {
            std::scoped_lock < std::mutex > lock(g_mutex);
            
            streambuf buffer;
            
            read_until(socket_for_message, buffer, '#');
            
            std::string message;
            
            std::istream i_stream(& buffer);
            
            std::getline(i_stream, message, '#');
            
            if(message == "/exit")
            {
                flag_2 = true;
            }
            else
            {
                std::cout << user_name + ": " + message << std::endl;
            }
            flag = false;
        }
    }
}

int main(int argc, const char * argv[])
{
    const std::size_t size = 30;

    auto port = 7711;

    ip::tcp::endpoint endpoint(ip::address_v4::any(), port);

    io_service io_service;
    
    std::cout << "Enter your name: ";
    std::string user_name = "name";
    std::cin >> user_name;

    try
    {
        ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

        acceptor.bind(endpoint);

        acceptor.listen(size);

        ip::tcp::socket socket_for_name(io_service);
        ip::tcp::socket socket_for_message(io_service);

        acceptor.accept(socket_for_name);
        acceptor.accept(socket_for_message);
        
        std::thread write_thread(write_data, user_name, std::ref(socket_for_name), std::ref(socket_for_message));
        std::thread read_thread(read_data, user_name, std::ref(socket_for_name), std::ref(socket_for_message));
            
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
