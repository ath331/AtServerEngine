////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief {{ClassName}}InfoManagerManagerTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"
#include "Logic/Utils/Log/AtLog.h"
#include "{{ClassName}}Info.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief {{ClassName}}InfoManagerTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////
class {{ClassName}}InfoManagerTemplate
{
protected:
	/// {{ClassName}}InfoMap
	using {{ClassName}}InfoMap = std::map< {{KeyType}}, {{ClassName}}Info >;

protected:
	/// {{ClassName}}InfoMap
	{{ClassName}}InfoMap m_infoMap;

public:
	/// GetInfo
	const {{ClassName}}Info* GetInfo( {{KeyType}} {{KeyName}} );

private:
	/// AddInfo
	AtBool _AddInfo(
{%- for member in memberList %}
	{%- if loop.last %}
	    {{member.valueType}} {{member.name}} );
	{%- else %}
	    {{member.valueType}} {{member.name}},
	{%- endif -%}
{%- endfor %}

protected:
	/// Initialize
	AtBool _Initialize();
};