#pragma once
#include "Field.h"
#include "RecordName.h"

#include <list>
#include <unordered_map>

struct MASK;
class F_MASK : Field
{
public:
	F_MASK();
	F_MASK(const F_MASK& other);
	virtual ~F_MASK();

public:
	std::vector<MASK*> listMask;

private:
	// Key : RCNM + RCID
	std::unordered_map<__int64, MASK*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);
	int GetFieldLength();

	void AddMask(MASK* mask);
	void DeleteMask(RecordName recordName);
	MASK* GetMask(RecordName recordName);

	int getCount() const;
	MASK* getMASKbyIndex(int index) const;
};