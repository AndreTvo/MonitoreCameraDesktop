#pragma once

#define EOL "\r\n"
#define EOS ";\r\n"

#include <sstream>
using std::stringstream;

enum AndroidObjectType
{
	TOUCH,
	KEY,
	GENERIC
};

class AndroidInputObject
{
protected:
	int version;
	int major;
	int minor;
	bool serialized;

	stringstream dataStream;
	AndroidObjectType type;

	void WriteHeader();
	void WriteSignature();

public:
	AndroidInputObject();
	~AndroidInputObject();

	AndroidObjectType getType() const;
	virtual const wchar_t * getSerialization();

};