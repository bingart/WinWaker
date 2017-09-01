
#ifndef WORK_CONFIG
#define WORK_CONFIG

#include <Windows.h>
#include <string>
#include <list>

class IWorkConfigListener
{
public:
	virtual void Notify() = 0;
};

class CWorkConfig
{
public:
	static CWorkConfig* m_Instance;

	static const int DEFAULT_WORKING_MODE = 1;

	static const int DEFAULT_AUTO_RUN = 1;
	static const int DEFAULT_STEP = 5;
	static const int DEFAULT_INTER_TIME = 10;	
	static const int DEFAULT_RUN_COUNT = 0;
	static const int DEFAULT_FILTER_APPLICATION = 0;

	static const int MAX_STEP = 5;
	static const int MIN_STEP = 1;

	static const int MAX_INTER_TIME = 30;
	static const int MIN_INTER_TIME = 2;

	static const int MAX_APPLICATION_LIST_SIZE = 32;

private:
	std::list<IWorkConfigListener *> m_Listeners;

private:
	int m_iWorkingMode;

	int m_iAutoRun;
	int m_iStep;
	int m_iInterTime;
	int m_iRunCount;

	int m_iFilterApplication;
	std::list<std::string> m_ApplicationList;

private:
	CWorkConfig() { Reset(); }
	~CWorkConfig() { if (m_Instance) delete m_Instance; }

public:
	void Notify();
	void Reset();
	void Save();
	void Load();

public:
	static CWorkConfig* Instance();

public:
	void AddListener(IWorkConfigListener* listener);

	int GetWorkingMode() { return m_iWorkingMode; }
	void SetWorkingMode(int workingMode) { m_iWorkingMode = workingMode; }

	int GetAutoRun() { return m_iAutoRun; }
	void SetAutoRun(int autoRun) { m_iAutoRun = autoRun; }

	int GetStep() { return m_iStep; }
	void SetStep(int step) { m_iStep = step; }

	int GetInterTime() { return m_iInterTime; }
	void SetInterTime(int time) { m_iInterTime = time; }

	int GetRunCount() { return m_iRunCount; }
	void SetRunCount(int runCount) { m_iRunCount = runCount; }

	void IncreaseRunCount() { m_iRunCount++; }

	int GetFilterApplication() { return m_iFilterApplication; }
	void SetFilterApplication(int filterApplication) { m_iFilterApplication = filterApplication; }

	void GetApplicationList(std::list<std::string>& list);
	void ResetApplicationList() { m_ApplicationList.clear(); }
	BOOL AddApplication(std::string szApplication);
};

#endif