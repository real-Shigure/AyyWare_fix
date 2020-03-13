/*
Syn's AyyWare Framework 2015
*/

#include "Interfaces.h"
#include "Utilities.h"

//SDK Specific Definitions
typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

//Some globals for later
CreateInterface_t EngineFactory = nullptr; // These are used to store the individual
CreateInterface_t ClientFactory = nullptr; //  CreateInterface functions for each game
CreateInterface_t VGUISurfaceFactory = nullptr; //  dll that we need access to. Can call
CreateInterface_t VGUI2Factory = nullptr; //  them to recieve pointers to game classes.
CreateInterface_t MatFactory = nullptr;
CreateInterface_t PhysFactory = nullptr;
CreateInterface_t StdFactory = nullptr;
CreateInterface_t InputSystemPointer = nullptr;

void Interfaces::Initialise()
{
	//Get function pointers to the CreateInterface function of each module
	EngineFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Engine, "CreateInterface");
	ClientFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Client, "CreateInterface");
	VGUI2Factory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUI2, "CreateInterface");
	VGUISurfaceFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUISurface, "CreateInterface");
	MatFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Material, "CreateInterface");
	PhysFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VPhysics, "CreateInterface");
	StdFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Stdlib, "CreateInterface");
	InputSystemPointer = (CreateInterface_t)GetProcAddress(
	(HMODULE)Utilities::Memory::WaitOnModuleHandle("inputsystem.dll"), "CreateInterface");

	//Get the interface names regardless of their version number by scanning for each string
	char* CHLClientInterfaceName = (char*)Utilities::Memory::FindTextPattern("client_panorama.dll", "VClient0");
	char* VGUI2PanelsInterfaceName = (char*)Utilities::Memory::FindTextPattern("vgui2.dll", "VGUI_Panel0");
	char* VGUISurfaceInterfaceName = (char*)Utilities::Memory::FindTextPattern("vguimatsurface.dll", "VGUI_Surface0");
	char* EntityListInterfaceName = (char*)Utilities::Memory::FindTextPattern(
		"client_panorama.dll", "VClientEntityList0");
	char* EngineDebugThingInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VDebugOverlay0");
	char* EngineClientInterfaceName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineClient0");
	char* ClientPredictionInterface = (char*)Utilities::Memory::FindTextPattern(
		"client_panorama.dll", "VClientPrediction0");
	char* MatSystemInterfaceName = (char*)Utilities::Memory::FindTextPattern("materialsystem.dll", "VMaterialSystem0");
	char* EngineRenderViewInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineRenderView0");
	char* EngineModelRenderInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineModel0");
	char* EngineModelInfoInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VModelInfoClient0");
	char* EngineTraceInterfaceName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "EngineTraceClient0");
	char* PhysPropsInterfaces = (char*)Utilities::Memory::FindTextPattern(
		"client_panorama.dll", "VPhysicsSurfaceProps0");
	char* VEngineCvarName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineCvar00");

	// Use the factory function pointers along with the interface versions to grab
	//  pointers to the interfaces
	Client = static_cast<IBaseClientDLL*>(ClientFactory(CHLClientInterfaceName, nullptr));
	Engine = static_cast<IVEngineClient*>(EngineFactory(EngineClientInterfaceName, nullptr));
	Panels = static_cast<IPanel*>(VGUI2Factory(VGUI2PanelsInterfaceName, nullptr));
	Surface = static_cast<ISurface*>(VGUISurfaceFactory(VGUISurfaceInterfaceName, nullptr));
	EntList = static_cast<IClientEntityList*>(ClientFactory(EntityListInterfaceName, nullptr));
	DebugOverlay = static_cast<IVDebugOverlay*>(EngineFactory(EngineDebugThingInterface, nullptr));
	Prediction = static_cast<DWORD*>(ClientFactory(ClientPredictionInterface, nullptr));
	MaterialSystem = static_cast<CMaterialSystem*>(MatFactory(MatSystemInterfaceName, nullptr));
	RenderView = static_cast<CVRenderView*>(EngineFactory(EngineRenderViewInterface, nullptr));
	ModelRender = static_cast<IVModelRender*>(EngineFactory(EngineModelRenderInterface, nullptr));
	ModelInfo = static_cast<CModelInfo*>(EngineFactory(EngineModelInfoInterface, nullptr));
	Trace = static_cast<IEngineTrace*>(EngineFactory(EngineTraceInterfaceName, nullptr));
	PhysProps = static_cast<IPhysicsSurfaceProps*>(PhysFactory(PhysPropsInterfaces, nullptr));
	CVar = static_cast<ICVar*>(StdFactory(VEngineCvarName, nullptr));
	ClientMode = **(IClientModeShared***)((*(DWORD**)Client)[10] + 0x5);
	// Get ClientMode Pointer
	DWORD p = Utilities::Memory::FindPattern("client_panorama.dll",
		(BYTE*)
		"\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xA8\x01\x75\x1A\x83\xC8\x01\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56",
		"xx????????xxxxxxxx????x????x????x????xxxx????x????x");
	InputSystem = static_cast<IInputSystem*>(InputSystemPointer("InputSystemVersion001", nullptr));

	// Search through the first entry of the Client VTable
	// The initializer contains a pointer to the 'GlobalsVariables' Table

	Globals = **(CGlobalVarsBase***)((*(DWORD**)Client)[0] + 0x1B); //psilent fix

	PDWORD pdwClientVMT = *(PDWORD*)Client;
	pInput = *(CInput**)(Utilities::Memory::FindPatternV2("client_panorama.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") +
		1);

	Utilities::Log("Interfaces Ready");
}

// Namespace to contain all the valve interfaces
namespace Interfaces
{
	IBaseClientDLL* Client;
	IVEngineClient* Engine;
	IPanel* Panels;
	IClientEntityList* EntList;
	ISurface* Surface;
	IVDebugOverlay* DebugOverlay;
	IClientModeShared* ClientMode;
	CGlobalVarsBase* Globals;
	DWORD* Prediction;
	CMaterialSystem* MaterialSystem;
	CVRenderView* RenderView;
	IVModelRender* ModelRender;
	CModelInfo* ModelInfo;
	IEngineTrace* Trace;
	IPhysicsSurfaceProps* PhysProps;
	ICVar* CVar;
	CInput* pInput;
	IInputSystem* InputSystem;
};