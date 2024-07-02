////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief {{ClassName}}InfoTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief {{ClassName}}InfoTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////
class {{ClassName}}InfoTemplate
{
public:
	/// Constructor
	{{ClassName}}InfoTemplate();

	/// Constructor
	{{ClassName}}InfoTemplate(
{%- for member in memberList %}
	{%- if loop.last %}
	    {{member.valueType}} {{member.name}} );
	{%- else %}
	    {{member.valueType}} {{member.name}},
	{%- endif -%}
{%- endfor %}

	/// Destructor
	~{{ClassName}}InfoTemplate();

public:
{%- for member in memberList +%}
	/// Get{{member.Name}}.
	{{member.valueType}} Get{{member.Name}}() { return m_{{member.name}}; }
{% endfor %}

{%- for member in memberList +%}
	/// Set{{member.Name}}.
	AtVoid Set{{member.Name }}( {{member.valueType}} {{member.name}} ) { m_{{member.name}} = {{member.name}}; }
{% endfor %}

private:
{%- for member in memberList +%}
	/// {{member.name}}
	{{member.valueType}} m_{{member.name}};
{% endfor -%}
};

/// SharedPointer
using {{ClassName}}InfoPtr = std::shared_ptr< {{ClassName}} >;
