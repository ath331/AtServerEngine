import argparse
import jinja2
import ProtoParser

def main():

	arg_parser = argparse.ArgumentParser(description = 'PacketGenerator')
	arg_parser.add_argument('--path', type=str, default='C:/Users/ath331/Desktop/a/VisualStudio/C++/AtServerEngine/Common/Protobuf/bin/Protocol.proto', help='proto path')
	arg_parser.add_argument('--isRecvHandler', type=bool, default=False, help='isRecvHandler')
	arg_parser.add_argument('--recvHandlerPath', type=str, default='C:/Users/ath331/Desktop/a/VisualStudio/C++/AtServerEngine/GameServer/Packet/Handler', help='recvHandler path')
	arg_parser.add_argument('--sendHandlerPath', type=str, default='C:/Users/ath331/Desktop/a/VisualStudio/C++/AtServerEngine/DummyClient/Packet/Handler', help='sendHandler path')
	arg_parser.add_argument('--output', type=str, default='TestPacketHandler', help='output file')
	arg_parser.add_argument('--recv', type=str, default='C_', help='recv convention')
	arg_parser.add_argument('--send', type=str, default='S_', help='send convention')
	args = arg_parser.parse_args()

	parser = ProtoParser.ProtoParser(1000, args.recv, args.send)
	parser.parse_proto(args.path)

	file_loader = jinja2.FileSystemLoader('Templates')
	env = jinja2.Environment(loader=file_loader)

	template = env.get_template('PacketHandler.h')
	output = template.render(parser=parser, output=args.output)

	f = open(args.output+'.h', 'w+')
	f.write(output)
	f.close()

	print(output)

	# Each RecvPacket Handler Make
	if args.isRecvHandler:
		for index, recvPacket in enumerate(parser.recv_pkt):
			recvPacketHandlerTemplate = env.get_template('ClientPacketHandlerTemplate.h')
			eachHandler = recvPacketHandlerTemplate.render(pkt=recvPacket, output=args.recvHandlerPath)

			f = open(args.recvHandlerPath + '/' + recvPacket.name  +'HandlerTemplate.cpp', 'w+')
			f.write(eachHandler)
			f.close()

			print(eachHandler)

	# Each SendPacket Handler Make
	if args.isRecvHandler == False:
		# sendPacket. but for-each recv_pkt list. ( client and server change )
		for index, sendPacket in enumerate(parser.recv_pkt):
			sendPacketHandlerTemplate = env.get_template('ServerPacketHandlerTemplate.h')
			eachHandler = sendPacketHandlerTemplate.render(pkt=sendPacket, output=args.sendHandlerPath)

			f = open(args.sendHandlerPath + '/' + sendPacket.name  +'HandlerTemplate.cpp', 'w+')
			f.write(eachHandler)
			f.close()

			print(eachHandler)

	return

if __name__ == '__main__':
	main()