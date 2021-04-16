//
//  main.cpp
//  HW10-1
//
//  Created by Alyaev Roman on 15.04.2021.
//

#include <iostream>
#include <string>
#include <atomic>

#include <boost/asio.hpp>

#define BOOST_DATE_TIME_NO_LIB

std::string read_data_until(boost::asio::ip::tcp::socket & socket, bool flag)
{
    boost::asio::streambuf buffer;

    boost::asio::read_until(socket, buffer, '!');

    std::string message;

    std::istream input_stream(&buffer);
    
    std::getline(input_stream, message, '!');
    
    if(message == "/exit")
    {
        flag = true;
    }

    return message;
}

int main(int argc, char ** argv)
{
    const std::size_t size = 30;

    auto port = 7711;

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

    boost::asio::io_service io_service;

    try
    {
        boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

        acceptor.bind(endpoint);

        acceptor.listen(size);

        boost::asio::ip::tcp::socket socket(io_service);

        acceptor.accept(socket);

        bool my_flag = false;
        
        while (my_flag != true)
        {
            std::cout << read_data_until(socket, my_flag) << std::endl;
        }
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
