/*********************************************************************************
    LbXmlParse.h
    Simple recursive descent parser for XML and DTD files

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

#ifndef __LBXMLPARSE__
#define __LBXMLPARSE__

class LbXmlParseBase
{
public:

LbXmlParseBase();
~LbXmlParseBase();

bool Open(const string &fname);
void Close();

protected:

bool SkipWhitespace();
bool ReadName(string &name);
bool ExpectChar(char c);
bool ExpectString(const string &str);
bool ReadContents(string &content);
bool ReadAttrName(string &name);
bool ReadAttrValue(string &value);
bool Eof();

private:

bool IsXmlSpace(char c)
	{
    //work around warning (force int to bool)
	return isspace(c) ? true : false;
	}
bool IsXmlAlpha(char c)
	{
	return isalnum(c) ? true : false;
	}
bool IsXmlNameChar(char c)
	{
	if(IsXmlAlpha(c) || 
		(c=='.') || (c=='-') ||
		(c=='_') || (c==':'))
		return true;
	else return false;
	}
bool IsXmlAttrChar(char c)
    {
    /*
    ** TODO: extend this to cover escape sequences
    */
    if(c=='"')
        return false;
    else
        return true;
    }
bool IsXmlContentChar(char c)
    {
    /*
    ** TODO: make sure this is correct
    */
    if(c=='<')
        return false;
    else
        return true;
    }

ifstream in_file;
};

struct LbXmlAttribute
{
string name;
string value;
};
class LbXmlTag;

typedef vector<LbXmlAttribute> LbXmlAttributeVect;
typedef vector<LbXmlTag *> LbXmlTagVect;

/*
**NOTE:
** is_tag==true if this is an actual tag
** is_tag==false if this is part of the value of a tag
**         (name is the contents of the tag).
*/
class LbXmlTag
{
public:
LbXmlTag()
    {
    is_tag=true;
    }

~LbXmlTag();

bool IsTag()
    {
    return is_tag;
    }
void SetIsTag(bool it)
    {
    is_tag=it;
    }

void SetName(const string &s)
	{
	name=s;
	}
const string &GetName()
    {
    return name;
    }

void AddAttribute(LbXmlAttribute &a)
	{
	attrs.push_back(a);
	}

void AddChild(LbXmlTag *nc)
    {
    children.push_back(nc);
    }

private:

bool is_tag;

LbXmlAttributeVect attrs;
LbXmlTagVect children;

string name;
};

class LbXmlParser : public LbXmlParseBase
{
public:

void ParseDocument();

private:
void ReadTagRec(LbXmlTag *&base_tag);

bool ReadStartTag(LbXmlTag *tag);
bool ReadEndTag(LbXmlTag *tag);
bool ReadAttributes(LbXmlTag *tag);
};

#endif