#include "Precompiled.h"
#include "ServiceManager.h"

#include "ClodderService.h"

using namespace boost;

ServiceManager::ServiceManager(void)
: _clodderService(shared_ptr<ClodderService>( new ClodderService() ))
{
}

ServiceManager::~ServiceManager(void)
{
}

// Singleton impl
juce_ImplementSingleton(ServiceManager)
