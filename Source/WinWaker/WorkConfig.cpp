
#include <Windows.h>
#include "WorkConfig.h"
#include "WorkReg.h"
#include "WorkLog.h"
#include "argument.h"

const std::string WORK_KEY_PATH = "Software\\WinWaker";

const std::string NAME_WORKING_MODE = "WorkingMode";
const std::string NAME_AUTO_RUN = "AutoRun";
const std::string NAME_STEP = "Step";
const std::string NAME_INTER_TIME = "InterTime";
const std::string NAME_RUN_COUNT = "RunCount";

const std::string AUTO_RUN_PATH = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
const std::string AUTO_RUN_KEY = "WinWaker";

const std::string NAME_FILTER_APPLICATION = "FilterApplication";
const std::string NAME_APPLICATION_LIST = "ApplicationList";

CWorkConfig* CWorkConfig::m_Instance = NULL;

CWorkConfig* CWorkConfig::Instance()
{
	if (m_Instance == NULL)
	{
		m_Instance = new CWorkConfig();
	}

	return m_Instance;
}

void CWorkConfig::AddListener(IWorkConfigListener* listener)
{
	m_Listeners.push_back(listener);
}

void CWorkConfig::Reset()
{
	m_iWorkingMode = DEFAULT_WORKING_MODE;
	m_iAutoRun = DEFAULT_AUTO_RUN;
	m_iStep = DEFAULT_STEP;
	m_iInterTime = DEFAULT_INTER_TIME;
	m_iRunCount = DEFAULT_RUN_COUNT;
	m_iFilterApplication = DEFAULT_FILTER_APPLICATION;

	m_ApplicationList.push_back("EXCEL.EXE");
	m_ApplicationList.push_back("POWERPNT.EXE");
	m_ApplicationList.push_back("WINWORD.EXE");
	m_ApplicationList.push_back("putty.exe");
	m_ApplicationList.push_back("psftp.exe");
	m_ApplicationList.push_back("eclipse.exe");
	m_ApplicationList.push_back("devenv.exe");
}

void CWorkConfig::Save()
{
	// Save to register table
	if (true)
	{
		CWorkReg reg(HKEY_CURRENT_USER, WORK_KEY_PATH.c_str(), FALSE);

		reg.WriteIntegerValue(NAME_WORKING_MODE.c_str(), m_iWorkingMode);
		reg.WriteIntegerValue(NAME_AUTO_RUN.c_str(), m_iAutoRun);
		reg.WriteIntegerValue(NAME_STEP.c_str(), m_iStep);
		reg.WriteIntegerValue(NAME_INTER_TIME.c_str(), m_iInterTime);
		reg.WriteIntegerValue(NAME_RUN_COUNT.c_str(), m_iRunCount);
		reg.WriteIntegerValue(NAME_FILTER_APPLICATION.c_str(), m_iFilterApplication);
		if (true)
		{
			std::string value;
			bool first = true;
			for (std::list<std::string>::iterator it = m_ApplicationList.begin(); it != m_ApplicationList.end(); it++)
			{
				if (first)
				{
					first = false;
					value += *it;
				}
				else
				{
					value += ',';
					value += *it;
				}
			}
			reg.WriteStringValue(NAME_APPLICATION_LIST.c_str(), value.c_str());
		}
	}

	// Set auto run
	if (m_iAutoRun)
	{
		CWorkReg reg(HKEY_LOCAL_MACHINE, AUTO_RUN_PATH.c_str(), FALSE);
		
		char szModulePath[MAX_PATH] = {0};
		// Will contain exe path
		HMODULE hModule = GetModuleHandle(NULL);
		if (hModule != NULL)
		{
			// When passing NULL to GetModuleHandle, it returns handle of exe itself
			GetModuleFileName(hModule, szModulePath, (sizeof(szModulePath)));
			reg.WriteStringValue(AUTO_RUN_KEY.c_str(), szModulePath);
		}
	}
	else
	{
		CWorkReg reg(HKEY_LOCAL_MACHINE, AUTO_RUN_PATH.c_str(), FALSE);
		reg.RemoveValue(AUTO_RUN_KEY.c_str());
	}

	// Notify
	Notify();
}

void CWorkConfig::Load()
{
	// Load from register table
	if (true)
	{
		CWorkReg reg(HKEY_CURRENT_USER, WORK_KEY_PATH.c_str());

		// The first one will fails !!!!!
		reg.ReadIntegerValue(NAME_WORKING_MODE.c_str(), m_iWorkingMode, DEFAULT_WORKING_MODE);
		reg.ReadIntegerValue(NAME_AUTO_RUN.c_str(), m_iAutoRun, DEFAULT_AUTO_RUN);
		reg.ReadIntegerValue(NAME_STEP.c_str(), m_iStep, DEFAULT_STEP);
		reg.ReadIntegerValue(NAME_INTER_TIME.c_str(), m_iInterTime, DEFAULT_INTER_TIME);
		reg.ReadIntegerValue(NAME_RUN_COUNT.c_str(), m_iRunCount, DEFAULT_RUN_COUNT);
		reg.ReadIntegerValue(NAME_FILTER_APPLICATION.c_str(), m_iFilterApplication, DEFAULT_FILTER_APPLICATION);
		if (true)
		{
			std::string value;
			BOOL rc = reg.ReadStringValue(NAME_APPLICATION_LIST.c_str(), value);
			if (rc)
			{
				// Reset since no duplication checking
				m_ApplicationList.clear();

				std::vector<std::string> arguments;
				rc = GetArguments(value, ',', arguments);
				if (rc)
				{
					for (std::vector<std::string>::iterator it = arguments.begin(); it != arguments.end(); it++)
					{
						m_ApplicationList.push_back(*it);
					}
				}
				else
				{
					WorkLog::Format("GetArguments for %s fails\n", value.c_str());
				}
			}
			else
			{
				WorkLog::Format("read string %s from reg fails\n", NAME_APPLICATION_LIST.c_str());
			}
		}
	}

	// Notify
	Notify();
}

void CWorkConfig::Notify()
{
	for (std::list<IWorkConfigListener *>::iterator it = m_Listeners.begin(); it != m_Listeners.end(); it++)
	{
		(*it)->Notify();
	}
}

void CWorkConfig::GetApplicationList(std::list<std::string>& list)
{
	list.clear();
	for (std::list<std::string>::iterator it = m_ApplicationList.begin(); it != m_ApplicationList.end(); it++)
	{
		list.push_back(*it);
	}
}

BOOL CWorkConfig::AddApplication(std::string szApplication)
{
	if (m_ApplicationList.size() < MAX_APPLICATION_LIST_SIZE)
	{
		BOOL bFound = FALSE;
		for (std::list<std::string>::iterator it = m_ApplicationList.begin(); it != m_ApplicationList.end(); it++)
		{
			if ((*it) == szApplication)
			{
				bFound = TRUE;
				break;
			}
		}

		if (!bFound)
		{
			WorkLog::Format("application not found, add new application %s\n", szApplication.c_str());
			m_ApplicationList.push_back(szApplication);
			return TRUE;
		}
		else
		{
			WorkLog::Format("application found, add ignored\n");
			return FALSE;
		}
	}
	else
	{
		WorkLog::Format("application list size exceeds limit, ignored\n");
		return FALSE;
	}
}
