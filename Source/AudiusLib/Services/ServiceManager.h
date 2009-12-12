#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: ServiceManager.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Manages all supported remote services.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

//class ClodderService;
#include "ClodderService.h"

class ServiceManager : public DeletedAtShutdown
{
private:
	ServiceManager(void);
	~ServiceManager(void);

public:
	juce_DeclareSingleton(ServiceManager, true)

	boost::shared_ptr<ClodderService> getClodder() { return _clodderService; }

private:
	boost::shared_ptr<ClodderService> _clodderService;
};
