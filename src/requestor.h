#ifndef REQUESTOR_H
#define REQUESTOR_H

#include "systemcomponent.h"
#include "dbquery.h"
#include "extracted_data.h"

#include <string>

class Requestor : public SystemComponent
{
public:	
	inline explicit Requestor()
	{
	}

	virtual ~Requestor()
	{
	}

	virtual bool run();
	virtual bool run(ExtractedData *in, std::string *appServerID,
					 DBQuery *out);
};

#endif