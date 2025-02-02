#include "stdafx.h"
#include "R_MultiPointRecord.h"
#include "DRDirectoryInfo.h"
#include "F_C2IL.h"
#include "F_COCC.h"
#include "F_C3IL.h"
#include "F_INAS.h"
#include "C2IL.h"
#include "C3IL.h"

R_MultiPointRecord::R_MultiPointRecord(void)
{
	m_mrid.m_name.RCNM = 115;
}

R_MultiPointRecord::~R_MultiPointRecord(void)
{
	delete m_cocc;
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		delete *itor;
	}

	for (auto itor = m_c3il.begin(); itor != m_c3il.end(); itor++)
	{
		delete *itor;
	}

	for (auto itor = m_c2il.begin(); itor != m_c2il.end(); itor++)
	{
		delete *itor;
	}
}

#pragma warning(disable:4018)
BOOL R_MultiPointRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	int i = 0, j = 0, cnt;
	for(unsigned i = 0; i < dir->m_count; i++)
	{
		if(strcmp(dir->GetDirectory(i)->tag, "MRID") == 0)
		{
			m_mrid.ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "INAS") == 0)
		{
			F_INAS *inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "COCC") == 0)
		{
			if (!m_cocc)
			{
				m_cocc = new F_COCC();
			}

			cnt = (dir->GetDirectory(i)->length - 1) / F_INAS::GetSize();
			m_cocc->ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "C2IL") == 0)
		{
			F_C2IL* c2il = new F_C2IL();
			cnt = (dir->GetDirectory(i)->length - 1) / C2IL::GetSize();
			c2il->ReadField(buf, cnt);
			m_c2il.push_back(c2il);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "C3IL") == 0)
		{
			F_C3IL* c3il = new F_C3IL();
			cnt = (dir->GetDirectory(i)->length - 1) / C3IL::GetSize();
			c3il->ReadField(buf, cnt);
			m_c3il.push_back(c3il);
		}
		else
		{
			buf += dir->GetDirectory(i)->length;
		}
		
		if (*(buf++) != 0x1E)
		{

		}
	}
	return TRUE;
}

bool R_MultiPointRecord::WriteRecord(CFile* file)
{
	directory.clear();

	// Set directory
	int fieldOffset = 0;
	int fieldLength = m_mrid.GetFieldLength();
	Directory dir("MRID", fieldLength, fieldOffset);
	directory.push_back(dir);
	fieldOffset += fieldLength;

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("INAS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	for (auto i = m_c3il.begin(); i != m_c3il.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("C3IL", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}


	int totalFieldSize = fieldOffset;

	// Set leader
	SetLeader(totalFieldSize, false);
	leader.SetAsDR();
	leader.WriteLeader(file);

	// Write directory
	WriteDirectory(file);

	// Write field area
	m_mrid.WriteField(file);

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		(*i)->WriteField(file);
	}

	for (auto i = m_c3il.begin(); i != m_c3il.end(); i++)
	{
		(*i)->WriteField(file);
	}

	return true;
}

RecordName R_MultiPointRecord::GetRecordName()
{
	return m_mrid.m_name;
}

int R_MultiPointRecord::GetRCID()
{
	return m_mrid.m_name.RCID;
}

std::string R_MultiPointRecord::GetRCIDasString(std::string prefix)
{
	return prefix + pugi::as_utf8(GetRCIDasWstring());
}

std::wstring R_MultiPointRecord::GetRCIDasWstring()
{
	return std::to_wstring(GetRCID());
}

void R_MultiPointRecord::SetRCID(int rcid)
{
	m_mrid.m_name.RCID = rcid;
}

void R_MultiPointRecord::InsertC3IL(int x, int y, int z)
{
	if (m_c3il.size() == 0)
	{
		m_c3il.push_back(new F_C3IL());
	}

	m_c3il.front()->m_arr.push_back(new C3IL(x, y, z));
}

std::vector<C2IL*> R_MultiPointRecord::GetAllC2IL()
{
	std::vector<C2IL*> result;

	for (auto i = m_c2il.begin(); i != m_c2il.end(); i++) {
		auto c2il = *i;
		for (auto j = c2il->m_arr.begin(); j != c2il->m_arr.end(); j++) {
			result.push_back(*j);
		}
	}

	return result;
}

std::vector<C3IL*> R_MultiPointRecord::GetAll3CIL()
{
	std::vector<C3IL*> result;
	
	for (auto i = m_c3il.begin(); i != m_c3il.end(); i++) {
		auto c3il = *i;
		for (auto j = c3il->m_arr.begin(); j != c3il->m_arr.end(); j++) {
			result.push_back(*j);
		}
	}

	return result;
}