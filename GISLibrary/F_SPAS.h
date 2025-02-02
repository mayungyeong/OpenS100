#pragma once
#include "Field.h"
#include "RecordName.h"

struct SPAS;
class F_SPAS : Field
{
public:
	F_SPAS();
	F_SPAS(const F_SPAS& other);
	virtual ~F_SPAS();

public:
	std::vector<SPAS*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);

	int GetFieldLength();

	int getCount() const;
	SPAS* getSPAS(int index) const;
	void addSPAS(SPAS* item);
};