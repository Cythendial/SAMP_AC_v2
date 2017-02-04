#include <boost\thread.hpp>
#include <tchar.h>

#include "Network\CRakClientHandler.h"
#include "s0beit\samp.h"
#include "CLoader.h"
#include "CPacketIntegrity.h"
#include "Misc.h"
#include "CLog.h"

const int MAX_TIME_DIFFERENCE = 1500;

std::vector<CPacketIntegrity*> CPacketIntegrity::m_instances;
boost::shared_mutex CPacketIntegrity::m_access;

std::vector<HMODULE> CPacketIntegrity::m_allowedModules;

CPacketIntegrity::CPacketIntegrity(BitStream *bitStream)
{
	m_time = GetTickCount();
	m_md5 = Misc::MD5_Memory(reinterpret_cast<int>(bitStream->GetData()), BITS_TO_BYTES(bitStream->GetNumberOfBitsUsed()));

	boost::unique_lock<boost::shared_mutex> lock(m_access);
	m_instances.push_back(this);
}

CPacketIntegrity::~CPacketIntegrity()
{
	boost::unique_lock<boost::shared_mutex> lock(m_access);
	m_instances.erase(std::remove(m_instances.begin(), m_instances.end(), this), m_instances.end());
}

bool CPacketIntegrity::Compare(std::string const& md5)
{
	return md5 == m_md5;
}

void CPacketIntegrity::GlobalInitialize()
{
	TCHAR 
		windowsPath[MAX_PATH],
		modulePath[MAX_PATH];

	GetWindowsDirectory(windowsPath, sizeof(windowsPath));
	m_allowedModules.clear();

	_stprintf_s(modulePath, TEXT("%s\\System32\\kernel32.dll"), windowsPath);
	m_allowedModules.push_back(GetModuleHandle(modulePath));
	_stprintf_s(modulePath, TEXT("%s\\System32\\ntdll.dll"), windowsPath);
	m_allowedModules.push_back(GetModuleHandle(modulePath));
	_stprintf_s(modulePath, TEXT("%s\\System32\\user32.dll"), windowsPath);
	m_allowedModules.push_back(GetModuleHandle(modulePath));

	m_allowedModules.push_back(CLoader::ThishMod);
	m_allowedModules.push_back((HMODULE)getSampBaseAddress());
	m_allowedModules.push_back(GetModuleHandle(NULL)); // add gta_sa.exe to allowed modules.. but this might be a bad move

	std::sort(m_allowedModules.begin(), m_allowedModules.end());
}

// If FramesToSkip = 0, first BackTrace entry points to this function (?)
USHORT WINAPI FIX_RtlCaptureStackBackTrace(ULONG FramesToSkip, ULONG FramesToCapture, PVOID *BackTrace, PULONG BackTraceHash)
{
	typedef USHORT(WINAPI *CaptureStackBackTraceType)(ULONG, ULONG, PVOID*, PULONG);
	static CaptureStackBackTraceType func = NULL;
	
	if (func == NULL)
	{
		TCHAR kernelPath[MAX_PATH];
		GetWindowsDirectory(kernelPath, sizeof(kernelPath));
		_tcscat_s(kernelPath, TEXT("\\System32\\kernel32.dll"));

		func = (CaptureStackBackTraceType)(GetProcAddress(GetModuleHandle(kernelPath), "RtlCaptureStackBackTrace"));
	}
	return func(FramesToSkip, FramesToCapture, BackTrace, BackTraceHash);
}

bool CPacketIntegrity::Check(const char *data, int size_in_bits)
{
	// FIRST PART: Packet integrity checks

	// Maybe this check should be done in another thread?
	// Is MD5'ing every packet slowing down sending process? or causing fps drops?

	unsigned int tick = GetTickCount();
	std::string md5 = Misc::MD5_Memory(reinterpret_cast<int>(data), BITS_TO_BYTES(size_in_bits));
	std::vector<std::pair<CPacketIntegrity*, bool>> packets_to_remove; // bool = was packet lost?

	{
		boost::shared_lock<boost::shared_mutex> lock(m_access);
		for (auto& p : m_instances)
		{
			if (tick - p->m_time > MAX_TIME_DIFFERENCE)
				packets_to_remove.push_back(std::make_pair(p, true));
			else if (p->Compare(md5))
				packets_to_remove.push_back(std::make_pair(p, false));
		}
	}

	for (auto& p : packets_to_remove)
	{
		if (p.second)
		{
			// PACKET LOST!
			// Force crash?
		}

		delete p.first;
	}

	// SECOND PART: Outside calls checks

	MEMORY_BASIC_INFORMATION mbi;
	HMODULE hCallerModule;
	const int kMaxCallers = 62; // 62 is maximum allowed value on Windows XP
	void *callers[kMaxCallers];
	int callCount = FIX_RtlCaptureStackBackTrace(0, kMaxCallers, callers, NULL);

	for (int i = 0; i < callCount; i++)
	{
		if (VirtualQuery(callers[i], &mbi, sizeof(mbi)))
		{
			// sometimes, deep in call trace (sampac <- sampdll <- systemdlls <- 0), callers[i] is being assigned address: 0, so does hCallerModule
			hCallerModule = (HMODULE)mbi.AllocationBase;
		}
		else
		{
			// if VirtualQuery fails for some reason, make sure client won't get reported
			// this might not be the best approach, though
			hCallerModule = 0;
		}

		if (hCallerModule != 0 && callers[i] != 0 && !std::binary_search(m_allowedModules.begin(), m_allowedModules.end(), hCallerModule))
		{
			// module not found in CPacketIntegrity::m_allowedModules list, so we need to take action.
			
			CLog("sendpacket_callstack.log").Write("Modbase: %x not found, caller: %x", hCallerModule, (unsigned)callers[i]);
			return 0; // don't send!
		}
	}

	return 1;
}