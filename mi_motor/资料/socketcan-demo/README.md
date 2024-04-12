# SocketCAN Demo Programs

These programs demonstrate how to use the SocketCAN API on Linux. You are free
to use these as a starting point for writing your own SocketCAN enabled
applications.

## Raw Interface Demo

This program demonstrates reading and writing to a CAN bus using SocketCAN's
Raw interface. The intended behavior of this program is to read in any CAN
message from the bus, add one to the value of each byte in the received
message, and then write that message back out on to the bus with the message ID
defined by the macro MSGID.

## Broadcast Manager Interface Demo

This program demonstrates reading and writing to a CAN bus using SocketCAN's
Broadcast Manager interface. The intended behavior of this program is to read
in CAN messages which have an ID of 0x123, add one to the value of each data
byte in the received message, and then write that message back out on to the
bus with the message ID defined by the macro MSGID.

## Broadcast Manager Cyclic Demo

This program demonstrates sending a set of cyclic messages out on to the CAN
bus using SocketCAN's Broadcast Manager interface. The intended behavior of
this program is to send four cyclic messages out on to the CAN bus. These
messages have IDs ranging from 0x0C0 to 0x0C3. These messages will be sent out
one at a time every 1200 milliseconds. Once all messages have been sent,
transmission will begin again with message 0x0C0.

#SocketCAN演示程序



这些程序演示了如何在Linux上使用SocketCAN API。您可以自由使用这些作为编写自己的SocketCAN应用程序的起点。



##原始界面演示



该程序演示如何使用SocketCAN的Raw接口读取和写入CAN总线。
该程序的预期行为是从总线读取任何CAN消息，在接收到的消息中的每个字节的值上加一，然后用宏MSGID定义的消息ID将该消息写回总线。



##广播管理器界面演示



本程序演示使用SocketCAN的广播管理器接口读取和写入CAN总线。
该程序的预期行为是读入ID为0x123的CAN消息，在接收到的消息中的每个数据字节的值上加一，然后用宏MSGID定义的消息ID将该消息写回总线。



##广播管理器循环演示



该程序演示了使用SocketCAN的广播管理器接口将一组循环消息发送到CAN总线。
该程序的预期行为是向CAN总线发送四个循环消息。这些消息的ID范围从0x0C0到0x0C3。
这些消息将每1200毫秒一次发送一条。发送完所有消息后，将再次开始发送消息0x0C0。
