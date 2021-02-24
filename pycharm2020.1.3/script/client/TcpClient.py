import asyncio

from TcpConn import TcpConn
from client.Puppet import Puppet
from client.PuppetBindEntity import PuppetBindEntity
from common import gr
from core.common import MsgpackSupport


async def tcp_echo_client(message):
    reader, writer = await asyncio.open_connection(
        '127.0.0.1', 8888)

    _ppt = Puppet()
    _tcp_conn = TcpConn(writer.get_extra_info('peername'), writer, reader)
    _tcp_conn.set_entity(_ppt)
    _pbe = PuppetBindEntity()
    _pbe.set_connection(_tcp_conn)

    _ppt.init_from_dict({})

    _ppt.set_bind_entity(_pbe)
    gr.bind_entity = _pbe

    _ppt.CompPuppetTest.puppet_chat_to_channel({'content: test_chat_msg'})

    return

    print('return')

    print(f'Send: {message!r}')
    # writer.write(message.encode())
    writer.write(MsgpackSupport.encode(message))

    while True:
        data = await reader.read(100)
        # print(f'Received: {data.decode()!r}')
        print(f'Received: {MsgpackSupport.decode(data)!r}')

    print('Close the connection')
    writer.close()

asyncio.run(tcp_echo_client('Hello World!'))
