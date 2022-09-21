#include "stdafx.h"
#include "S101Layer.h"
#include "S100Utilities.h"
#include "R_FeatureRecord.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include "../S100Geometry/SGeometry.h"

S101Layer::S101Layer(FeatureCatalogue* fc, PortrayalCatalogue* pc) : S100Layer(fc, pc)
{
	SetProductNumber(101);
}

S101Layer::~S101Layer()
{
}

void S101Layer::SetSpatialObject(S101Cell* cell)
{
	m_spatialObject = cell;
	m_spatialObject->m_pLayer = this;
}

bool S101Layer::Open(CString _filepath)
{
	auto level = S100Utilities::GetLevel(std::wstring(_filepath));

	m_spatialObject = new S101Cell();
	m_spatialObject->m_pLayer = this;

	if (!m_spatialObject->Open(_filepath))
	{
		delete m_spatialObject;
		return false;
	}	

	SetAllNumericCode();

	return true;
}

void S101Layer::Draw(GISLibrary::D2D1Resources* D2, Scaler* scaler)
{
	m_spatialObject->Draw(D2, scaler);
}

std::wstring S101Layer::GetChartName()
{	
	// return filename 
	// ex : 101GB005X01SW.000
	
	return std::wstring(GetLayerName());
}

std::wstring S101Layer::GetEditionNumberAsWstring()
{
	if (GetS100SpatialObject()==nullptr)
	{
		return L"NULL";
	}


	CString Ened=((S101Cell*)GetS100SpatialObject())->m_dsgir.m_dsid.m_ened;
	auto index= Ened.Find(L".");
	CString result;
	AfxExtractSubString(result, Ened,0,'.');

	// The ENED of the DSID contains values in the form of n.m. such as 1.0, 1.1, etc.
	// n is Edition Number.
	// m is Update Number.
	// I'll return the Edition Number here.
	// e.g. If it's 1.4, return 1
	return std::wstring(result);
}

std::wstring S101Layer::GetUpdateNumberAsWstring()
{
	if (GetS100SpatialObject() == nullptr)
	{
		return L"NULL";
	}

	auto Ened = ((S101Cell*)GetS100SpatialObject())->m_dsgir.m_dsid.m_dsed;

	CString result;
	AfxExtractSubString(result, Ened, 1, '.');

	// The ENED of the DSID contains values in the form of n.m. such as 1.0, 1.1, etc.
	// n is Edition Number.
	// m is Update Number.
	// I'll return the Edition Number here.
	// e.g. If it's 1.4, return 4
	return std::wstring(result); 
}

std::wstring S101Layer::GetIssueDateAsWstring()
{
	if (GetS100SpatialObject() == nullptr)
	{
		return L"NULL"; 
	}

	CString Dsrd = ((S101Cell*)GetS100SpatialObject())->m_dsgir.m_dsid.m_dsrd;
	// Output DSRD of DSID.
	// DSRD is in the form of YYYMMDD (e.g., 20210324).
	// Return Example: 2012-03-24
	auto index =Dsrd.Find(_T("-"));
	if (index==-1)
	{
		CString year=Dsrd.Left(4);
		CString month =Dsrd.Mid(4,2);
		CString day=Dsrd.Right(2);

		std::wstring result = year + L"-" + month + L"-" + day;
		return result;

	}
	else
	{
		return std::wstring(Dsrd);
	}
}

void S101Layer::SetAllNumericCode()
{
	auto enc = (S101Cell*)m_spatialObject;

	if (featureCatalogue)
	{
		// Feature Type
		auto featureTypes = featureCatalogue->GetFeatureTypes().GetVecFeatureType();
		for (auto i = featureTypes.begin(); i != featureTypes.end(); i++)
		{
			auto code = (*i)->GetCodeAsWString();
			enc->m_dsgir.GetFeatureTypeCode(code);
		}

		// Information Type
		auto informationTypes = featureCatalogue->GetInformationTypesPointer().GetInformationTypePointer();;
		for (auto i = informationTypes.begin(); i != informationTypes.end(); i++)
		{
			auto code = i->second->GetCodeAsWString();
			enc->m_dsgir.GetInformationTypeCode(code);
		}

		// Simple Attribute
		auto simpleAttributes = featureCatalogue->GetSimpleAttributesPointer().GetSimpleAttributePointer();;
		for (auto i = simpleAttributes.begin(); i != simpleAttributes.end(); i++)
		{
			auto code = i->second->GetCodeAsWString();
			enc->m_dsgir.GetAttributeCode(code);
		}

		// Complex Attribute
		auto complexAttributes = featureCatalogue->GetComplexAttributesPointer().GetComplexAttributePointer();
		for (auto i = complexAttributes.begin(); i != complexAttributes.end(); i++)
		{
			auto code = i->second->GetCodeAsWString();
			enc->m_dsgir.GetAttributeCode(code);
		}

		// Role
		auto roles = featureCatalogue->GetRolesPointer().GetRolePointer();
		for (auto i = roles.begin(); i != roles.end(); i++)
		{
			auto code = i->second->GetCodeAsWString();
			enc->m_dsgir.GetAssociationRoleCode(code);
		}

		// Information Association
		auto informationAssociations = featureCatalogue->GetInformationAssociationsPointer().GetInformationAssociationPointer();
		for (auto i = informationAssociations.begin(); i != informationAssociations.end(); i++)
		{
			auto code = i->second->GetCodeAsWString();
			enc->m_dsgir.GetInformationAssociationCode(code);
		}

		// Feature Association
		auto featureAssociations = featureCatalogue->GetFeatureAssociationsPointer().GetFeatureAssociationPointer();
		for (auto i = featureAssociations.begin(); i != featureAssociations.end(); i++)
		{
			auto code = i->second->GetCodeAsWString();
			enc->m_dsgir.GetFeatureAssociationCode(code);
		}
	}
}