using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Assets.Scripts.Network.Handler;
using Protocol;
using UnityEngine;


namespace Assets.Scripts.Network.Handler
{
	public partial class PacketHandler
	{
		private void _Process_{{pkt.name}}_Handler( ushort protocolId, byte[] data )
		{
			{{pkt.name}} message = {{pkt.name}}.Parser.ParseFrom( data );
		}
	}
}
