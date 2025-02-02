#pragma once
#include "Field.h"
#include "RecordName.h"

class F_SRID : Field
{
public:
	F_SRID();
	F_SRID(RecordName recordName, int RVER = 1, int RUIN = 1);
	virtual ~F_SRID();

public:
	RecordName m_name;
	int m_rver;
	int m_ruin;

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file);

	int GetFieldLength();
};