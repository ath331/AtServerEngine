////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief {{ClassName}}InfoManagerTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "pch.h"
#include <iostream>
#include "{{ClassName}}InfoManagerTemplate.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief GetInfo
////////////////////////////////////////////////////////////////////////////////////////////////////
const {{ClassName}}Info* {{ClassName}}InfoManagerTemplate::GetInfo( AtInt32 id )
{
	auto iter = m_infoMap.find( id );
	if ( iter == m_infoMap.end() )
		return nullptr;

	return &(iter->second);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief AddInfo
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool {{ClassName}}InfoManagerTemplate::_AddInfo(
{%- for member in memberList %}
	{%- if loop.last %}
	    {{member.valueType}} {{member.name}} )
	{%- else %}
	    {{member.valueType}} {{member.name}},
	{%- endif -%}
{%- endfor %}
{
	auto iter = m_infoMap.find( {{KeyName}} );
	if ( iter != m_infoMap.end() )
		return false;

	{{ClassName}}Info info = {{ClassName}}Info( 
{%- for member in memberList %}
	{%- if loop.last %}
	    {{member.name}} );
	{%- else %}
	    {{member.name}},
	{%- endif -%}
{%- endfor %}

	m_infoMap[ {{KeyName}} ] = info;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief Initialize
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool {{ClassName}}InfoManagerTemplate::_Initialize()
{
	std::cout << "{{ClassName}}InfoManager Initialize()" << std::endl;

{%- for member in rows %}
	if ( !_AddInfo( {{ member | join(', ') }} ) ) return false;
{%- endfor %}

	return true;
}
