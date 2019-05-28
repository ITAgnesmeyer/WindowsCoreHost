#include "stdafx.h"
#include "Parameter.h"
#include "ResourceLoader.h"
#include "StringFunctions.h"



Parameter::Parameter(HINSTANCE hInstance)
{
    this->Instance = hInstance;
}

Parameter::~Parameter()
= default;

std::wstring Parameter::GetLastError() const
{
    return this->LastError;
}

bool Parameter::TestResData()
{
    if (this->AssemblyName.length() == 0)
    {
        this->LastError = _T("Missing AssemblyName in Resource!");
        return false;
    }

    if (this->MethodName.length() == 0)
    {
        this->LastError = _T("Missing MethodName in Resource!");
        return false;
    }

    if (this->Version.length() == 0)
    {
        this->LastError = _T("Missing Version in Resource!");
        return false;
    }

    if (this->NamespaceName.length() == 0)
    {
        this->LastError = _T("Missing NamespaceName in Resource!");
        return false;
    }

    return true;
}

bool Parameter::TestIds()
{
    if (this->IdAssemblyName == 0)
    {
        this->LastError = _T("IdAssemblyName not set!");
        return true;
    }

    if (this->IdMethodName == 0)
    {
        this->LastError = _T("IdMethodName not set!");
        return true;
    }

    if (this->IdNamespaceName == 0)
    {
        this->LastError = _T("IdNamespaceName not set!");
        return true;
    }

    if (this->IdVersion == 0)
    {
        this->LastError = _T("IdVersion not set!");
        return true;
    }

    if (this->IdWithArgs == 0)
    {
        this->LastError = _T("IdWithArgs not set!");
        return true;
    }

    if (this->IdWithReturn == 0)
    {
        this->LastError = _T("IdWithReturn not set!");
        return true;
    }
    return true;
}

bool Parameter::FillData()
{
    if (!TestIds()) 
        return false;

    this->UseArgs = false;
    this->UseReturn = false;
    this->AssemblyName = ResourceLoader::LoadStringToWString(this->Instance, this->IdAssemblyName);
    this->MethodName = ResourceLoader::LoadStringToWString(this->Instance, this->IdMethodName);
    this->Version = ResourceLoader::LoadStringToWString(this->Instance, this->IdVersion);
    this->NamespaceName = ResourceLoader::LoadStringToWString(this->Instance, this->IdNamespaceName);
    this->WithArgs = ResourceLoader::LoadStringToWString(this->Instance, this->IdWithArgs);
    this->WithReturn = ResourceLoader::LoadStringToWString(this->Instance, this->IdWithArgs);
    this->WithArgs = StringFunctions::StringToUpper(this->WithArgs);
    this->WithReturn = StringFunctions::StringToUpper(this->WithReturn);
    if (!TestResData())
        return false;
    this->AssemblyNameBase = this->AssemblyName;
    const auto pos = this->AssemblyName.find_first_of(L'.');
    if(pos>0)
    {
        this->AssemblyNameBase = this->AssemblyName.substr(0,pos);
    }

    if (this->WithArgs == this->TURE_TEST)
        this->UseArgs = true;
    if (this->WithReturn == this->TURE_TEST)
        this->UseReturn = true;

    return true;
}


