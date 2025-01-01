using System;
using UnityEngine;


namespace Assets.Scripts.Network
{
    enum EPacketId
    {
    {%- for pkt in parser.total_pkt %}
        PKT_{{pkt.name}} = {{pkt.id}},
    {%- endfor %}
    }

    public class PacketIdFactory
    {
        public static ushort GetPacketId( string packetName )
        {
            try
            {
                packetName = "PKT_" + packetName;

                if ( Enum.TryParse< EPacketId >( packetName, true, out var result ) )
                    return (ushort)( result );

                // Debug.LogError( $"Invalid packet name: {packetName}" );
                return 0;
            }
            catch ( Exception ex )
            {
                Debug.LogError( $"Error parsing packet name: {packetName}. Exception: {ex}" );
                return 0;
            }
        }
    }
}
