#include "AndroidInputObject.h"

void AndroidInputObject::WriteHeader()
{
	dataStream << "AndroidInputObject=0" << EOL;
}

void AndroidInputObject::WriteSignature()
{
	dataStream << "version=" << version << EOL;
	dataStream << "major=" << major << EOL;
	dataStream << "minor=" << minor << EOL;
	dataStream << EOS;
}

AndroidInputObject::AndroidInputObject()
{
	version = 1;
	major = 0;
	minor = 0;
	dataStream.str("");

	type = GENERIC;
}

AndroidInputObject::~AndroidInputObject()
{
}

AndroidObjectType AndroidInputObject::getType() const
{
	return type;
}

const wchar_t * AndroidInputObject::getSerialization()
{
	dataStream.str("");

	WriteHeader();

	dataStream << "version=" << version << EOL;
	dataStream << "major=" << major << EOL;
	dataStream << "minor=" << minor << EOL;

	const size_t cSize = strlen(dataStream.str().c_str()) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs_s(nullptr, wc, cSize, dataStream.str().c_str(), cSize);

	return wc;

	
}