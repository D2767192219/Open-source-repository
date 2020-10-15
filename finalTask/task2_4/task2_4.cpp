#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace std ;
using namespace boost :: asio ;

int main (){
 io_service iosev;
 boost::system::error_code ec;
 serial_port sp(iosev);
 sp.open("/dev/pts/2", ec);
 

 
 sp.set_option(serial_port::baud_rate(9600));
 sp.set_option(serial_port::flow_control(serial_port::flow_control::none));
 sp.set_option(serial_port::parity(serial_port::parity::none));
 sp.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
 sp.set_option(serial_port::character_size(8));
 
 write (sp, buffer ( "Hello world", 12 ) ) ;     
 iosev. run ( ) ;
 return 0;
}





