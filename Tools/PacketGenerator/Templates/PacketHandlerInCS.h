using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Assets.Scripts.Network;
using UnityEngine;


namespace Assets.Scripts.Network.Handler
{
	public partial class PacketHandler
	{
		public void ProcessHandler( ushort protocolId, byte[] data )
		{
			EPacketId packetId = (EPacketId)( protocolId );
			switch ( packetId )
			{
			{%- for pkt in parser.send_pkt %}
			case EPacketId.PKT_{{pkt.name}}: { _Process_{{pkt.name}}_Handler( protocolId, data ); } break;
			{%- endfor %}
			default:
				{
					Debug.LogWarning( $"Invalid Protocol ID : {protocolId}" );
				}
				break;
			}
		}
	}
}
