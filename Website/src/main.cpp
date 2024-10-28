#include <fast_io.h>
#include <fast_io_device.h>
#include <vector>

int main()
try
{
    fast_io::net_service service;
    std::vector<fast_io::native_socket_file> sockets;

    for (auto i : std::views::iota(0, 65535))
    {
        try
        {
            fast_io::native_socket_file socket(fast_io::tcp_listen(i));
        }
        catch (fast_io::error error)
        {
            fast_io::io::perrln("Failed to listen on port ", i, ", ", error);
        }
    }

    while (true)
    {
        fast_io::io::scan("");
    }
}
catch (fast_io::error error)
{
    fast_io::io::perrln(error);
}