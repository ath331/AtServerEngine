using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;
using Assets.Scripts.Network;
using Protocol;
using Google.Protobuf;


namespace Assets.Scripts.Network.Packet.{{pkt.path}}
{
	public class Packet_{{pkt.name}} : Network
	{
		// base.Start() block
		public void Start() {}

		public void Send_{{pkt.name}}()
		{
			{{pkt.name}} pkt = new {{pkt.name}}();

			_Send( pkt );
		}
	}
}
