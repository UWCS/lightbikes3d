/*********************************************************************************
    LbXmlParse.cpp
    Simple recursive descent parser for XML files.

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*********************************************************************************/
#include "LbStandard.h"
#include "LbPublic.h"



LbXmlParseBase::LbXmlParseBase()
{
}

LbXmlParseBase::~LbXmlParseBase()
{
}

bool LbXmlParseBase::Open(const string &fname)
{
if(in_file.is_open())Close();

in_file.open(fname.c_str(),ios::in);
return in_file.is_open();
}

void LbXmlParseBase::Close()
{
in_file.close();
}

bool LbXmlParseBase::SkipWhitespace()
{
bool found_ws=false;
char c;

while(!in_file.eof())
	{
	in_file.get(c);
	if(!IsXmlSpace(c))
		{
		in_file.putback(c);
		break;
		}
	found_ws=true;
	}

return found_ws;
}

bool LbXmlParseBase::ReadName(string &name)
{
bool found_name=false;
char c;

name="";

while(!in_file.eof())
	{
	in_file.get(c);
	if(!IsXmlNameChar(c))
		{
		in_file.putback(c);
		break;
		}

	name+=c;
	found_name=true;
	}
return found_name;
}

bool LbXmlParseBase::ExpectChar(char c)
{
char d;

if(in_file.eof())
	return false;

in_file.get(d);
if(c==d)
	return true;

in_file.putback(d);
return false;
}

bool LbXmlParseBase::ExpectString(const string &str)
{
for(int i=0;i<str.size();i++)
    {
    if(!ExpectChar(str[i]))
        return false;
    }
return true;
}

bool LbXmlParseBase::Eof()
{
return in_file.eof();
}

bool LbXmlParseBase::ReadAttrName(string &name)
{
return ReadName(name);
}

bool LbXmlParseBase::ReadAttrValue(string &value)
{
char c;

if(!ExpectChar('"'))
    return false;

value="";

while(!in_file.eof())
	{
	in_file.get(c);
	if(!IsXmlAttrChar(c))
		{
		in_file.putback(c);
		break;
		}

	value+=c;
	}

if(!ExpectChar('"'))
    return false;

return true;
}

bool LbXmlParseBase::ReadContents(string &content)
{
bool found_con=false;
char c;

content="";

while(!in_file.eof())
	{
	in_file.get(c);
	if(!IsXmlContentChar(c))
		{
		in_file.putback(c);
		break;
		}

	content+=c;
	found_con=true;
	}
return found_con;
}

void LbXmlParser::ReadTagRec(LbXmlTag *&base_tag)
{
LbXmlTag *curr_tag=NULL;
string contents;

try
    {
    base_tag=new LbXmlTag;
    if(base_tag==NULL)
        throw LbError("Cannot alloc mem for base tag");

	if(!ReadStartTag(base_tag))
	    throw LbError("Expecting XML tag");

    while(!Eof())
        {
        if(ExpectChar('<'))
            {
            if(ExpectChar('/'))
                break;

            ReadTagRec(curr_tag);
    
            base_tag->AddChild(curr_tag);
            curr_tag=NULL;
            }
        else if(ReadContents(contents))
            {
            curr_tag=new LbXmlTag;
            if(curr_tag==NULL)
                throw LbError("Cannot alloc mem for tag"); 
        
            curr_tag->SetIsTag(false);
            curr_tag->SetName(contents);

            base_tag->AddChild(curr_tag);
            curr_tag=NULL;
            }
        else
            throw LbError("Expecting tag or contents");
        }
    if(base_tag==NULL)
        throw LbError("Not expecting end tag");
    
    if(!ReadEndTag(base_tag))
	    throw LbError("Expecting XML end tag");
    }
catch(...)
    {
    if(curr_tag!=NULL)
        delete curr_tag;

    throw;
    }
}

void LbXmlParser::ParseDocument()
{
LbXmlTag *curr_tag=NULL;

try
	{
	while(!Eof())
		{
        if(!ExpectChar('<'))
            throw LbError("Expecting XML tag");

        ReadTagRec(curr_tag);
		}
	}
catch(...)
	{
	if(curr_tag!=NULL)
		delete curr_tag;

	throw;
	}
}


bool LbXmlParser::ReadStartTag(LbXmlTag *tag)
{
string name;
tag=NULL;

if(!ReadName(name))
	throw LbError("Expecting tag name");

tag->SetName(name);

ReadAttributes(tag);

if(!ExpectChar('>'))
	throw LbError("Expecting >");

return true;
}

bool LbXmlParser::ReadAttributes(LbXmlTag *tag)
{
bool read_attr=false;
LbXmlAttribute new_attr;

while(!Eof())
	{
	if(!SkipWhitespace())
		return read_attr;//  need whitespace before attribute

	if(!ReadAttrName(new_attr.name))
		return read_attr;

	read_attr=true;
	
	if(!ExpectChar('='))// can we have attributes without values?
		{
		tag->AddAttribute(new_attr);
		continue;
		}
	
	if(!ReadAttrValue(new_attr.value))
		throw LbError("Expecting attribute value");
	
	tag->AddAttribute(new_attr);
	}

return read_attr;
}

bool LbXmlParser::ReadEndTag(LbXmlTag *tag)
{
if(!ExpectString(tag->GetName()))
	throw LbError("Expecting different end tag");
if(!ExpectChar('>'))
	throw LbError("Expecting end of tag >");

return true;
}

LbXmlTag::~LbXmlTag()
{
for(int i=0;i<children.size();i++)
    if(children[i]!=NULL)
        {
        delete children[i];
        children[i]=NULL;
        }    
}