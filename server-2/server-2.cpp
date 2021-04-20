//
//  server-2.cpp
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

std::string read_name(ip::tcp::socket & socket)
{
    streambuf buffer;

    read_until(socket, buffer, ':');

    std::string message;

    std::istream i_stream(& buffer);
    
    std::getline(i_stream, message, ':');

    return message;
}

std::string read_data(ip::tcp::socket & socket)
{
    streambuf buffer;
    
    read(socket, buffer);
    
    std::string message;
    
    std::istream i_stream(& buffer);
    
    std::getline(i_stream, message);

    if(message == "/exit")
    {
        flag = true;
    }

    return message;
}

void write_data(ip::tcp::socket & socket)
{
    std::string message;
    
    std::cin >> message;
    
    if(message == "/exit")
    {
        flag = true;
    }

    write(socket, buffer(message));
}

void thread_func_for_write(ip::tcp::acceptor acceptor, ip::tcp::socket write_socket)
{
    while (flag == false)
    {
        acceptor.accept(write_socket);
        
        write_data(write_socket);
    }
}


void thread_func_for_read(ip::tcp::acceptor acceptor, ip::tcp::socket read_socket, std::string user_name)
{
    while (flag == false)
    {
        acceptor.accept(read_socket);
        
        std::cout << user_name << " " << read_data(read_socket) << std::endl;
    }
}

int main(int argc, const char * argv[])
{
    const std::size_t size = 30;

    auto port = 7711;
    
    std::cout << "Enter IP: ";
    
    std::string raw_ip_address;
    
    std::cin >> raw_ip_address;

    ip::tcp::endpoint endpoint(ip::address::from_string(raw_ip_address), port);

    io_service io_service;

    try
    {
        ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

        acceptor.bind(endpoint);

        acceptor.listen(size);

        ip::tcp::socket write_socket(io_service);
        ip::tcp::socket read_socket(io_service);

        acceptor.accept(read_socket);
        std::string user_name = read_name(read_socket);
        
        std::thread write_thread(thread_func_for_write, acceptor, write_socket);
        std::thread read_thread(thread_func_for_read, acceptor, read_socket);

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
